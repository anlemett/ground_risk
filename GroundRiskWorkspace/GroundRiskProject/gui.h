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

#include <string>
#include <map>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <assert.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
enum
{
    ID_QUIT = 1,
	ID_LOAD,
	ID_SAVE,
	ID_PROCESS,
	ID_BEST_SIZE
};

class MyCanvas;

class MainFrameBase : public wxFrame 
{
    private:
        typedef std::map<std::vector<unsigned char>, int> ColorsMapType;
        std::vector<std::vector<int>> LoadMapFromImage(wxImage& image, ColorsMapType& colors);
        
        struct AirRiskInstance {
            std::vector<std::vector<int>> map;
            int total_time_s;
        };
        AirRiskInstance LoadAirRiskMap(std::string json_full_path, int total_time);
        
        struct Path {
            std::vector<std::vector<int>> path;
            float linear_combination_weight;
            int risk;
            float length_m;
            float alpha;
        };

        float ComputeAirRisk(AirRiskInstance& air_risk_instance, Path& path);

	protected:
		wxMenuBar* m_menuBar;
		wxMenu* m_menuFile;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseFrame( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnExitClick( wxCommandEvent& event ) { event.Skip(); }
        
        //new
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnOpenImage(wxCommandEvent& WXUNUSED(event) ) ;
        void OnSaveImage(wxCommandEvent & WXUNUSED(event)) ;
        void OnProcessImage(wxCommandEvent& WXUNUSED(event) ) ;
        void OnClose(wxCloseEvent& event) ;
        void OnBestSize(wxCommandEvent& WXUNUSED(event)) ;

        MyCanvas *m_canvas; // the canvas inside the main frame
        bool m_imageLoaded ;
        DECLARE_EVENT_TABLE()
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrameBase();
	
};

#endif //__GUI_H__
