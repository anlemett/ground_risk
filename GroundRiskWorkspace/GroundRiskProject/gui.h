///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  5 2013)
// http://www.wxformbuilder.org/
///////////////////////////////////////////////////////////////////////////

#ifndef __GUI_H__
#define __GUI_H__

#include "MapPanel.h"
#include "risks.h"
#include "RiskMap.h"

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/file.h>
#include "wx/wxprec.h"

#include <filesystem>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <chrono>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MapPanel;

class MainFrameBase : public wxFrame 
{
    public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, 
                       const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,600 ), 
                       long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrameBase();
                
	protected:
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseFrame( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnExitClick( wxCommandEvent& event ) { event.Skip(); }
        
        void OnClose(wxCloseEvent& event) ;
        void OnCalculatePath(wxCommandEvent& WXUNUSED(event) ) ;
        
        DECLARE_EVENT_TABLE()
			
    private:
    
        void loadImage();
        
        void createRiskMap();

        std::vector<std::vector<int>> loadMapFromImage(wxImage& image, ColorsMapType& colors);
    
        MapPanel* m_bitmapPanel;
        std::string m_image_full_path;
        
        RiskMap m_risk_map;
};

#endif //__GUI_H__
