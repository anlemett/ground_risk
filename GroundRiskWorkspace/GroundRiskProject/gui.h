///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  5 2013)
// http://www.wxformbuilder.org/
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

//class MyCanvas;
class MyBitmapPanel;

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

        //MyCanvas *m_canvas; // the canvas inside the main frame
        bool m_imageLoaded ;
        DECLARE_EVENT_TABLE()
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrameBase();
	

/************************/

public:
 /*   MyFrame() : wxFrame(NULL, wxID_ANY, "Test")
    {
        const int borderAround = FromDIP(10);
        
        wxBoxSizer* bSizer = new wxBoxSizer(wxVERTICAL);

        wxButton* button = new wxButton(this, wxID_ANY, "Load image file...");
        bSizer->Add(button, wxSizerFlags().Expand().Border(wxALL, borderAround));
        button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::OnLoadImageFile, this);

        m_bitmapPanel = new MyBitmapPanel(this);
        bSizer->Add(m_bitmapPanel, wxSizerFlags().Expand().Proportion(1).Border(wxALL, borderAround));
        
        SetSizer(bSizer);
    }	*/
private:
    MyBitmapPanel* m_bitmapPanel;
    
    void OnLoadImageFile(wxCommandEvent&)
    {
        static wxString fileName;
        wxImage image;
        
        char result[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        const char *path;
        if (count != -1) {
            path = dirname(result);
        }
    
        std::string full_path = path;
        std::string png_full_path = full_path;
        png_full_path = png_full_path.append("/data/density_fixed_scaled.png");
        fileName = png_full_path;


        if ( fileName.empty() )
            return;
                
        if ( image.LoadFile(fileName) )
        {   
                        
            Coord from = {517, 412};
            Coord to = {567, 462};
            for (int i= from.x-5; i<from.x+5;i++)
                for (int j=from.y-5; j<from.y+5;j++) 
                    image.SetRGB(i, j, 255,0,0);
                    
            for (int i= to.x-5; i<to.x+5;i++)
                for (int j=to.y-5; j<to.y+5;j++) 
                    image.SetRGB(i, j, 255,0,0);

            m_bitmapPanel->SetBitmap(wxBitmap(image));
            SetTitle(wxString::Format("Simple Image Viewer (%s: %d x %d pixels)", 
                wxFileName(fileName).GetFullName(), image.GetWidth(), image.GetHeight()));                
        }                
    }
};


 /********************************/
#endif //__GUI_H__
