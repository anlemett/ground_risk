///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  5 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "Bresenham.h"
#include "BicriteriaDijkstraInstance.h"

#include <libgen.h>         // dirname
#include <unistd.h>         // readlink
#include <linux/limits.h>   // PATH_MAX

BEGIN_EVENT_TABLE(MainFrameBase, wxFrame)
	EVT_MENU(ID_LOAD,  MainFrameBase::OnOpenImage)
	EVT_MENU(ID_SAVE,  MainFrameBase::OnSaveImage)
	EVT_MENU(ID_PROCESS,  MainFrameBase::OnProcessImage)
	EVT_MENU(ID_BEST_SIZE,  MainFrameBase::OnBestSize)
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
    
    // old:
	
	//m_menuBar = new wxMenuBar( 0 );
	//m_menuFile = new wxMenu();
	//wxMenuItem* menuFileExit;
	//menuFileExit = new wxMenuItem( m_menuFile, wxID_EXIT, wxString( _("E&xit") ) + wxT('\t') + wxT("Alt+X"), wxEmptyString, wxITEM_NORMAL );
	//m_menuFile->Append( menuFileExit );
	
	//m_menuBar->Append( m_menuFile, _("&File") ); 
	
	//this->SetMenuBar( m_menuBar );
	
	//wxBoxSizer* mainSizer;
	//mainSizer = new wxBoxSizer( wxVERTICAL );
	
	//this->SetSizer( mainSizer );
	//this->Layout();
	//m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	//this->Centre( wxBOTH );
	
	// Connect Events
	//this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	//this->Connect( menuFileExit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
    
    // new
    wxMenu *file_menu = new wxMenu();
	file_menu->Append(ID_LOAD, _T("&Open image..."));
	file_menu->Append(ID_PROCESS, _T("&Process image"));
	file_menu->Append(ID_SAVE, _T("&Save image as..."));
	file_menu->Append(ID_BEST_SIZE, _T("&Best size"));
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
void MainFrameBase::OnProcessImage(wxCommandEvent& WXUNUSED(event))
//------------------------------------------------------------------------
{
	if (m_imageLoaded)
	    m_canvas->ProcessImage() ;
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
    
    AirRiskInstance air_risk_instance = LoadAirRiskMap(json_full_path, total_time);
    
    //std::cout << "Json file: " << air_risk_instance.map << std::endl;
    
    assert(map.at(0).size()==air_risk_instance.map.size());
    assert(map.size()==air_risk_instance.map.at(0).size());

    RiskMap risk_map;
    risk_map.map = map;
    risk_map.m_per_pixel = 1000.0/(131.0/2.0);
    risk_map.offset = 25;
    
    //BicriteriaDijkstra
    
    std::vector<int> from = {517, 412};
    std::vector<int> to = {765, 600};
    BicriteriaDijkstraInstance* inst = new BicriteriaDijkstraInstance(risk_map, from, to, 5, 150);
    
    auto start = std::chrono::high_resolution_clock::now();

    //let paths = inst.compute_pareto_apx_paths();

    //println!("{:?}", paths);

    //let mut res_routes = vec![];

    //for path in paths {
        //let air_risk = air_risk_instance.compute_air_risk(&path);
        //res_routes.push(HFRMPath{
            //route: path.path,
            //air_risk: air_risk,
            //ground_risk: path.risk as f64,
            //length_m: path.length_m,
            //alpha: path.alpha
        //})
    //}

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << std::endl;

    //save_paths_to_json("./results/res_nk.json", &res_routes);

}


//------------------------------------------------------------------------
MainFrameBase::AirRiskInstance MainFrameBase::LoadAirRiskMap(std::string json_full_path, int total_time)
//------------------------------------------------------------------------
{
    std::ifstream json_file;
    json_file.open (json_full_path);
    
    AirRiskInstance air_risk_instance;
    
    json_file >> air_risk_instance.map;
    json_file.close();
    
    air_risk_instance.total_time_s = total_time;
    return air_risk_instance;
}

//------------------------------------------------------------------------
 float MainFrameBase::ComputeAirRisk(MainFrameBase::AirRiskInstance& air_risk_instance, MainFrameBase::Path& path)
//------------------------------------------------------------------------
{
    float air_risk = 0.0;
    int length_px = 0;
    
    for (int i = 0; i < path.path.size(); i++) {
        std::vector<int> s = path.path.at(i);
        std::vector<int> e = path.path.at(i+1);       

        std::vector<std::vector<int>> points;
        
        Bresenham* br = new Bresenham(s.at(0), s.at(1), e.at(0), e.at(1));
        points = br->getPoints();
        delete br;
        
        for(int i=0; i < points.size(); i++){
            int x=points.at(i).at(0);
            int y=points.at(i).at(1);
            air_risk += (float)air_risk_instance.map.at(x).at(y);
            length_px += 1;
        }
        
        //for (x, y) in Bresenham::new((s.x as isize, s.y as isize), (e.x as isize, e.y as isize)) {
        //        air_risk += self.map[x as usize][y as usize] as f64;
        //        length_px += 1;
        //}
    }
    return (air_risk/(float)length_px)/((float)air_risk_instance.total_time_s);
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


//------------------------------------------------------------------------
void MainFrameBase::OnSaveImage(wxCommandEvent & WXUNUSED(event))
//------------------------------------------------------------------------
{
//	char str[128] = "" ; // proposed file name

	if (!m_imageLoaded)
		return ;

	wxString filename = wxFileSelector(_T("Save image as"),_T(""),_T(""),_T("*.bmp"), _T("BMP files (*.bmp)|*.bmp|GIF files (*gif)|*.gif|JPEG files (*jpg)|*.jpg|PNG files (*png)|*.png|TIFF files (*tif)|*.tif|XPM files (*xpm)|*.xpm|All files (*.*)|*.*"), wxFD_SAVE );
	if ( !filename.empty() )
		m_canvas->SaveImage(filename) ;
}

//------------------------------------------------------------------------
void MainFrameBase::OnBestSize(wxCommandEvent& WXUNUSED(event))
//------------------------------------------------------------------------
{
    m_canvas->BestSize() ;
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrameBase::OnCloseFrame ) );
	this->Disconnect( wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrameBase::OnExitClick ) );
	
}
