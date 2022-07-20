#include "MapPanel.h"

MapPanel::MapPanel(wxWindow* parent, const wxBitmap& bitmap)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
          m_bitmap(bitmap)
    {                
        SetBackgroundStyle(wxBG_STYLE_PAINT);
        Bind(wxEVT_PAINT, &MapPanel::OnPaint, this);
    }
    

MapPanel::~MapPanel() {
    }


void MapPanel::SetBitmap(const wxBitmap& bitmap)
    {
        m_bitmap = bitmap;
        m_scaledBitmap = wxNullBitmap;
        Update(); 
        Refresh();
    }


void MapPanel::OnPaint(wxPaintEvent&) 
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

