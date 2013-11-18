///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __HelpPanel__
#define __HelpPanel__

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/html/htmlwin.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/notebook.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class HelpPanel
///////////////////////////////////////////////////////////////////////////////
class HelpPanel : public wxPanel 
{
	private:
	
	protected:
		wxNotebook* m_notebook2;
		wxPanel* m_panel5;
		
		wxPanel* m_panel6;
		
		wxPanel* m_panel7;
		
	
	public:
		wxStaticBitmap* programImageA;
		wxHtmlWindow* aboutHtml;
		wxStaticBitmap* programImageD;
		wxHtmlWindow* docHtml;
		wxStaticBitmap* programImageC;
		wxHtmlWindow* creditsHtml;
		
		HelpPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,400 ), long style = wxTAB_TRAVERSAL );
		~HelpPanel();
	
};

#endif //__HelpPanel__
