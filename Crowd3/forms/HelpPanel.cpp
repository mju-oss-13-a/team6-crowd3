///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "HelpPanel.h"

///////////////////////////////////////////////////////////////////////////

HelpPanel::HelpPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	this->SetMinSize( wxSize( 400,400 ) );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panel5 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel5->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	programImageA = new wxStaticBitmap( m_panel5, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 64,64 ), 0 );
	bSizer11->Add( programImageA, 0, wxALIGN_CENTER|wxALL, 1 );
	
	
	bSizer11->Add( 0, 0, 0, wxALL|wxEXPAND, 1 );
	
	aboutHtml = new wxHtmlWindow( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO );
	aboutHtml->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	bSizer11->Add( aboutHtml, 1, wxALL|wxEXPAND, 1 );
	
	m_panel5->SetSizer( bSizer11 );
	m_panel5->Layout();
	bSizer11->Fit( m_panel5 );
	m_notebook2->AddPage( m_panel5, wxT("About"), true );
	m_panel6 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel6->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	programImageD = new wxStaticBitmap( m_panel6, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 64,64 ), 0 );
	bSizer12->Add( programImageD, 0, wxALIGN_CENTER|wxALL, 1 );
	
	
	bSizer12->Add( 0, 0, 0, wxALL|wxEXPAND, 1 );
	
	docHtml = new wxHtmlWindow( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO );
	docHtml->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	bSizer12->Add( docHtml, 1, wxALL|wxEXPAND, 1 );
	
	m_panel6->SetSizer( bSizer12 );
	m_panel6->Layout();
	bSizer12->Fit( m_panel6 );
	m_notebook2->AddPage( m_panel6, wxT("Documentation"), false );
	m_panel7 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel7->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	programImageC = new wxStaticBitmap( m_panel7, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 64,64 ), 0 );
	bSizer13->Add( programImageC, 0, wxALIGN_CENTER|wxALL, 1 );
	
	
	bSizer13->Add( 0, 0, 0, wxALL|wxEXPAND, 1 );
	
	creditsHtml = new wxHtmlWindow( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO );
	creditsHtml->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	
	bSizer13->Add( creditsHtml, 1, wxALL|wxEXPAND, 1 );
	
	m_panel7->SetSizer( bSizer13 );
	m_panel7->Layout();
	bSizer13->Fit( m_panel7 );
	m_notebook2->AddPage( m_panel7, wxT("Credits"), false );
	
	bSizer9->Add( m_notebook2, 1, wxALL|wxEXPAND, 1 );
	
	this->SetSizer( bSizer9 );
	this->Layout();
}

HelpPanel::~HelpPanel()
{
}
