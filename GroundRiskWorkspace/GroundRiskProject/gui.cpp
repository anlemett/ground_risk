///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  5 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

#include <string.h>

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

// old

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
	wxBitmap bitmap;

	//wxString filename = wxFileSelector(_T("Select file"),_T(""),_T(""),_T(""), _T("All files (*.*)|*.*") );
    //std::cout << filename;
     
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    const char *path;
    if (count != -1) {
        path = dirname(result);
    }
    
    std::string full_path = path;
    full_path = full_path.append("/data/density_fixed_scaled.png");
    wxString filename = full_path;
	if ( !filename.empty() )
	{
		m_canvas->LoadImage(filename) ;
		m_imageLoaded = true ;
	}
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
