#ifndef __BITMAPPANEL_H__
#define __BITMAPPANEL_H__

#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/msgdlg.h> 

#include <filesystem>
#include <libgen.h>         // dirname
#include <unistd.h>         // readlink
#include <linux/limits.h>   // PATH_MAX

//------------------------------------------------------------------------
//class MyCanvas: public wxPanel
class MyBitmapPanel: public wxPanel
//------------------------------------------------------------------------
{
public:
    //MyCanvas( wxWindow *parent, wxWindowID, const wxPoint &pos, const wxSize &size ) ;
    //~MyCanvas() ;
    //~BitmapPanel() ;
	//void LoadImage(wxString fileName) ;
	//void SaveImage(wxString fileName) ;
	//void ProcessImage() ;
	//void BestSize() ;

/******************************************************/
MyBitmapPanel(wxWindow* parent, const wxBitmap& bitmap = wxNullBitmap)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
          m_bitmap(bitmap)
    {                
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &MyBitmapPanel::OnPaint, this);                        
    }

    void SetBitmap(const wxBitmap& bitmap)
    {
        m_bitmap = bitmap;
        m_scaledBitmap = wxNullBitmap;
        Update(); Refresh();
    }

private:
    wxBitmap m_bitmap, m_scaledBitmap;

    static wxBitmap RescaleBitmap(const wxBitmap& bitmap, const wxSize& newSize)
    {        
        if ( !bitmap.IsOk() )
            return wxNullBitmap;
                
        const wxImage image = bitmap.ConvertToImage();

        return wxBitmap(image.Scale(newSize.GetWidth(), newSize.GetHeight(), wxIMAGE_QUALITY_HIGH));
    }

    void OnPaint(wxPaintEvent&) 
    { 
        wxPaintDC dc(this);
        
        dc.SetBackground(*wxBLACK_BRUSH);
        dc.Clear();

        if ( !m_bitmap.IsOk() )
            return; 

        const wxSize dcSize = dc.GetSize();
        
        if ( !m_scaledBitmap.IsOk() || m_scaledBitmap.GetSize() != dcSize )
            m_scaledBitmap = RescaleBitmap(m_bitmap, dcSize);
                    
        dc.DrawBitmap(m_scaledBitmap, 0, 0, true);
    }

private:
	int m_imageWidth ;
	int m_imageHeight ;
    wxBitmap m_imageBitmap ;	// used to display the image
	wxImage *m_imageRGB ;		// used to load the image
    unsigned char* m_myImage ;	// used to process the image

    //void OnPaint(wxPaintEvent &event) ;

//    DECLARE_EVENT_TABLE()       
};

/**********************************************************/
#endif __BITMAPPANEL_H__