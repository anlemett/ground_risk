#ifndef MAPPANEL_H
#define MAPPANEL_H

#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/msgdlg.h> 

#include <filesystem>
#include <libgen.h>         // dirname
#include <unistd.h>         // readlink
#include <linux/limits.h>   // PATH_MAX

class MapPanel: public wxPanel
{
public:
    MapPanel(wxWindow* parent, const wxBitmap& bitmap = wxNullBitmap);

    ~MapPanel();
    
    void SetBitmap(const wxBitmap& bitmap);

private:

    static wxBitmap RescaleBitmap(const wxBitmap& bitmap, const wxSize& newSize)
    {        
        if ( !bitmap.IsOk() )
            return wxNullBitmap;
                
        const wxImage image = bitmap.ConvertToImage();

        return wxBitmap(image.Scale(newSize.GetWidth(), newSize.GetHeight(), wxIMAGE_QUALITY_HIGH));
    }

    void OnPaint(wxPaintEvent&);
    
 	int m_imageWidth;
	int m_imageHeight;

    wxBitmap m_bitmap, m_scaledBitmap;
};

#endif // MAPPANEL_H
