///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  5 2013)
// http://www.wxformbuilder.org/
///////////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "BicriteriaDijkstraInstance.h"
#include "risks.h"

#include <libgen.h>         // dirname
#include <unistd.h>         // readlink
#include <linux/limits.h>   // PATH_MAX

BEGIN_EVENT_TABLE(MainFrameBase, wxFrame)
	EVT_MENU(ID_LOAD,  MainFrameBase::OnOpenImage)
    EVT_MENU(ID_QUIT,  MainFrameBase::OnQuit)
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

// for result json ???
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

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
    wxMenu *file_menu = new wxMenu();
	file_menu->Append(ID_LOAD, _T("&Open image..."));
	file_menu->AppendSeparator();
	file_menu->Append(ID_QUIT, _T("&Exit"));

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(file_menu, _T("&File"));
	SetMenuBar( menuBar );

// create the canvas that will manage the image
	m_canvas = new MyCanvas( this, -1, wxDefaultPosition, wxDefaultSize);
	m_imageLoaded = false ;
	Centre() ;

}

//------------------------------------------------------------------------
void MainFrameBase::OnQuit(wxCommandEvent& WXUNUSED(event))
//------------------------------------------------------------------------
{
    Close(true) ;
}

//------------------------------------------------------------------------
void MainFrameBase::OnClose(wxCloseEvent& event)
//------------------------------------------------------------------------
{
	delete m_canvas ;
	event.Skip() ;
}

//------------------------------------------------------------------------
void MainFrameBase::OnOpenImage(wxCommandEvent& WXUNUSED(event) )
//------------------------------------------------------------------------
{
    std::vector<unsigned char> color1 = {255, 255, 255, 255};
    std::vector<unsigned char> color2 = {214, 214, 214, 255};
    std::vector<unsigned char> color3 = {180, 209, 82, 255};
    std::vector<unsigned char> color4 = {183, 103, 26, 255};
    std::vector<unsigned char> color5 = {109, 0, 65, 255};
    std::vector<unsigned char> color6 = {27, 0, 31, 255};
    
    ColorsMapType colors;

    std::pair<ColorsMapType::iterator, bool> result1 = colors.insert(std::make_pair(color1, 1));
    std::pair<ColorsMapType::iterator, bool> result2 = colors.insert(std::make_pair(color2, 4));
    std::pair<ColorsMapType::iterator, bool> result3 = colors.insert(std::make_pair(color3, 19));
    std::pair<ColorsMapType::iterator, bool> result4 = colors.insert(std::make_pair(color4, 199));
    std::pair<ColorsMapType::iterator, bool> result5 = colors.insert(std::make_pair(color5, 499));
    std::pair<ColorsMapType::iterator, bool> result6 = colors.insert(std::make_pair(color6, 1000));

	//wxString filename = wxFileSelector(_T("Select file"),_T(""),_T(""),_T(""), _T("All files (*.*)|*.*") );
    //std::cout << filename;
     
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    const char *path;
    if (count != -1) {
        path = dirname(result);
    }
    
    std::string full_path = path;
    std::string png_full_path = full_path;
    png_full_path = png_full_path.append("/data/density_fixed_scaled.png");
    wxString png_filename = png_full_path;
	if ( !png_filename.empty() )
	{
		m_canvas->LoadImage(png_filename) ;
		m_imageLoaded = true ;
	}
       
    // Load the image.
    wxImage image(png_full_path, wxBITMAP_TYPE_PNG);
    //wxBitmap bitmap(image);
    std::vector<std::vector<int>> map = LoadMapFromImage(image, colors);
    
    // Displaying map
    /*for (int i = 0; i < map.size(); i++) {
        for (auto it = map[i].begin(); it != map[i].end(); it++)
            std::cout << *it << " ";
        std::cout << std::endl;
    }*/
    
    int total_time = 4*7*24;
    
    std::string json_full_path = full_path.append("/data/map.json");
    std::cout << json_full_path << "\n";
    
    RiskMap risk_map;
    risk_map.map = map;
    risk_map.m_per_pixel = 1000.0/(131.0/2.0);
    //risk_map.offset = 25;
    risk_map.offset = 0;
    
    //BicriteriaDijkstra
    
    //Coord from = {517, 412};
    //Coord to = {765, 600};
    Coord from = {517, 412};
    Coord to = {520, 415};
    
    auto start = std::chrono::high_resolution_clock::now();
    
    BicriteriaDijkstraInstance* inst = new BicriteriaDijkstraInstance(risk_map, from, to, 5, 150);

    std::vector<Path> paths = inst->computeParetoApxPaths();
    
    std::cout << paths << std::endl;

    delete inst;
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Duration: " << duration.count() << std::endl;

    //savePathsToJson("./results/res_nk.json", res_routes);

}


//------------------------------------------------------------------------
std::vector<std::vector<int>> MainFrameBase::LoadMapFromImage(wxImage& image, ColorsMapType& colors)
//------------------------------------------------------------------------
{
   
    std::vector<std::vector<int>> map;

    unsigned char *rgb = image.GetData(), *alpha = image.GetAlpha();
    
    int image_height = image.GetHeight();
    int image_width = image.GetWidth();
    //std::cout << image_height << " " << image_width << "\n";

    for (int y = 0; y < image_height; y++) {
        std::vector<int> line;
        for (int x = 0; x < image_width; x++) {
            int offs = (y * image_width) + x;   ////Alpha offset
            unsigned char pAlpha = 255;
            if (image.HasAlpha()) {
                pAlpha = *(alpha + offs);
            }
            
            offs = offs*3; //RGB offset
            // RGB might be ordered in reverse
            unsigned char pRed = *(rgb + offs);
            unsigned char pGreen = *(rgb + offs + 1);
            unsigned char pBlue = *(rgb + offs + 2);
           
            //std::cout << (int)pAlpha <<" "<<(int)pRed <<" "<< (int)pGreen << " "<< (int)pBlue << "\n";
            std::vector<unsigned char> color = {pRed, pGreen, pBlue, pAlpha};
            int risk = colors.at(color);
            //std::cout << risk <<"\n";
            
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
