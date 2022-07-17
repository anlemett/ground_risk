///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  5 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI_H__
#define __GUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/frame.h>


#include "wx/wxprec.h"
#include "risks.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/image.h>
#include <wx/file.h>
#include <wx/bitmap.h>

#include <filesystem>

#include "canvas.h"
#include "risks.h"

#include <string>
#include <map>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <chrono>


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
enum
{
    ID_QUIT = 1,
	ID_LOAD
};

class MyCanvas;

class MainFrameBase : public wxFrame 
{
    private:

        std::vector<std::vector<int>> LoadMapFromImage(wxImage& image, ColorsMapType& colors);
                
	protected:
		wxMenuBar* m_menuBar;
		wxMenu* m_menuFile;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseFrame( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnExitClick( wxCommandEvent& event ) { event.Skip(); }
        
        //new
        void OnQuit(wxCommandEvent& event);
        void OnOpenImage(wxCommandEvent& WXUNUSED(event) ) ;
        void OnClose(wxCloseEvent& event) ;

        MyCanvas *m_canvas; // the canvas inside the main frame
        bool m_imageLoaded ;
        DECLARE_EVENT_TABLE()
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrameBase();
	
};

#endif //__GUI_H__
