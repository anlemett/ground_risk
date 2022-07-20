#include "main.h"
#include "MapPanel.h"

#include <wx/image.h>
#include <wx/file.h>
#include <wx/bitmap.h>

// initialize the application
IMPLEMENT_APP(MainApp);

////////////////////////////////////////////////////////////////////////////////
// application class implementation 
////////////////////////////////////////////////////////////////////////////////

// In Project Settings - Post Build:
//COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data $<TARGET_FILE_DIR:${PROJECT_NAME}>/data

bool MainApp::OnInit()
{
    //support all available image formats
	wxInitAllImageHandlers();

	SetTopWindow( new MainFrame( NULL ) );
	GetTopWindow()->Show();
	
	// true = enter the main loop
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// main application frame implementation 
////////////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame(wxWindow *parent) : MainFrameBase( parent )
{
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnCloseFrame(wxCloseEvent& event)
{
	Destroy();
}

void MainFrame::OnExitClick(wxCommandEvent& event)
{
	Destroy();
}
