#include "gui.h"
#include "BicriteriaDijkstraInstance.h"
#include "risks.h"

#include <libgen.h>         // dirname
#include <unistd.h>         // readlink
#include <linux/limits.h>   // PATH_MAX

BEGIN_EVENT_TABLE(MainFrameBase, wxFrame)
	EVT_CLOSE(MainFrameBase::OnClose)
END_EVENT_TABLE()

std::istream& operator>>(std::istream& is, std::vector<int>& vec)
{
    //#1 - check if it starts from '['
    char c;
    is >> c;
    if (c != '[')
        throw std::runtime_error(std::string("Invalid character : ") + c + 
                                   " when parsing vector of double");
    //#2 - get the line till ']'
    std::string line;
    if (!std::getline(is, line, ']'))
        throw std::runtime_error("Error parsing vector of double");

    //#3 - parse values inside '[' and ']'
    std::istringstream lstr(line);
    std::string value;
    while (std::getline(lstr, value, ','))
        vec.push_back(stod(value));
    return is;
}

std::istream& operator>>(std::istream& is, std::vector<std::vector<int>>& m)
{
    //#1 - check if it starts from '['
    char c;
    is >> c;
    if (c != '[')
        throw std::runtime_error(std::string("Invalid character : ") + c + 
                                   " when parsing json file");

    // parse matrix line-by-line
    while(true)
    {
        std::vector<int> tmp;
        is >> tmp;
        m.push_back(std::move(tmp));
        // if matrix finihed, c should contain ']', else - ','
        is >> c;
        if (c == ']')
            return is;
        if (c != ',')
            throw std::runtime_error(std::string("Invalid character : ") + c + 
                                       " when parsing json file");
    }
}

// display vector
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& t)
{
    os << "[";
    for (auto it = t.begin(); it != t.end(); ++it)
        os << (it != t.begin() ? ", " : "") << *it;
    os << "]";
    return os;
}

///////////////////////////////////////////////////////////////////////////

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, 
                              const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
   
    const int borderAround = FromDIP(10);
        
    wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);

    wxButton* button = new wxButton(this, wxID_ANY, "Calculate path");
    bSizer->Add(button, wxSizerFlags().Expand().Border(wxALL, borderAround));

    button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrameBase::OnCalculatePath, this);

    m_bitmapPanel = new MapPanel(this);

    bSizer->Add(m_bitmapPanel, wxSizerFlags().Expand().Proportion(1).Border(wxALL, borderAround));
        
    SetSizer(bSizer);
    
    loadImage();
    
    createRiskMap();
}


//------------------------------------------------------------------------
void MainFrameBase::OnClose(wxCloseEvent& event)
//------------------------------------------------------------------------
{
	delete m_bitmapPanel;
	event.Skip() ;
}


void MainFrameBase::loadImage() {
    // Load image (population density)
    
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    const char *path;
    if (count != -1) {
        path = dirname(result);
    }
    
    std::string full_path = path;
    std::string png_full_path = full_path;
    m_image_full_path = png_full_path.append("/data/density_fixed_scaled.png");
       
    // Load the image
    wxImage image(m_image_full_path, wxBITMAP_TYPE_PNG);
    
    m_bitmapPanel->SetBitmap(wxBitmap(image));
    SetTitle(wxString::Format("Population density (%d x %d pixels)", 
             image.GetWidth(), image.GetHeight()));                
}

void MainFrameBase::createRiskMap() {
    // Create risk map from population density image
    wxImage image(m_image_full_path, wxBITMAP_TYPE_PNG);
    
    std::vector<unsigned char> color1 = {255, 255, 255, 255};
    std::vector<unsigned char> color2 = {214, 214, 214, 255};
    std::vector<unsigned char> color3 = {180, 209, 82, 255};
    std::vector<unsigned char> color4 = {183, 103, 26, 255};
    std::vector<unsigned char> color5 = {109, 0, 65, 255};
    std::vector<unsigned char> color6 = {27, 0, 31, 255};
    
    ColorsMapType colors;

    colors.insert(std::make_pair(color1, 1));
    colors.insert(std::make_pair(color2, 4));
    colors.insert(std::make_pair(color3, 19));
    colors.insert(std::make_pair(color4, 199));
    colors.insert(std::make_pair(color5, 499));
    colors.insert(std::make_pair(color6, 1000));

    std::vector<std::vector<int>> map = loadMapFromImage(image, colors);
    
    m_risk_map.map = map;
    m_risk_map.m_per_pixel = 1000.0/(131.0/2.0);
    //m_risk_map.offset = 25;
    m_risk_map.offset = 0;
}

//------------------------------------------------------------------------
void MainFrameBase::OnCalculatePath(wxCommandEvent& WXUNUSED(event) )
//------------------------------------------------------------------------
{    
    // Set from/to coordinates
    
    //Coord<int> from = {500, 500};
    //Coord<int> to = {600, 600};
    Coord<int> from = {517, 412};
    Coord<int>to = {527, 422};
    int search_limit = 2;
    
    // Display population density with from/to points
  
    wxImage image_from_to(m_image_full_path, wxBITMAP_TYPE_PNG);
    
    for (int i= from.x-5; i<from.x+5;i++)
        for (int j=from.y-5; j<from.y+5;j++) 
            image_from_to.SetRGB(i, j, 255,0,0);
                    
    for (int i= to.x-5; i<to.x+5;i++)
        for (int j=to.y-5; j<to.y+5;j++) 
            image_from_to.SetRGB(i, j, 255,0,0);

    m_bitmapPanel->SetBitmap(wxBitmap(image_from_to));
    SetTitle(wxString::Format("Population density (%d x %d pixels)", 
             image_from_to.GetWidth(), image_from_to.GetHeight()));                
        
    // Calculate paths
    
    auto start = std::chrono::high_resolution_clock::now();
    
    BicriteriaDijkstraInstance* inst = new BicriteriaDijkstraInstance(m_risk_map, from, to, search_limit, 150);

    std::vector<Path> paths = inst->computeParetoApxPaths();
    //std::cout << paths << std::endl;

    delete inst;
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Time elapsed is: " << duration.count() << "s\n";

    //savePathsToJson("./results/res_nk.json", res_routes);
}


//------------------------------------------------------------------------
std::vector<std::vector<int>> MainFrameBase::loadMapFromImage(wxImage& image, ColorsMapType& colors)
//------------------------------------------------------------------------
{   
    std::vector<std::vector<int>> map;

    unsigned char *rgb = image.GetData(), *alpha = image.GetAlpha();
    
    int image_height = image.GetHeight();
    int image_width = image.GetWidth();

    for (int y = 0; y < image_height; y++) {
        std::vector<int> line;
        for (int x = 0; x < image_width; x++) {
            int offs = (y * image_width) + x;   //Alpha offset
            unsigned char pAlpha = 255;
            if (image.HasAlpha()) {
                pAlpha = *(alpha + offs);
            }
            
            offs = offs*3; //RGB offset
            // RGB might be ordered in reverse
            unsigned char pRed = *(rgb + offs);
            unsigned char pGreen = *(rgb + offs + 1);
            unsigned char pBlue = *(rgb + offs + 2);
           
            std::vector<unsigned char> color = {pRed, pGreen, pBlue, pAlpha};
            int risk = colors.at(color);
            
            line.push_back(risk);
        }
        map.push_back(line);
    }
    return map;
}


MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	
}
