///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 12 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "xmlscriptdlggen.h"

///////////////////////////////////////////////////////////////////////////

xmlscriptdlggen::xmlscriptdlggen( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_XML = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxTE_DONTWRAP|wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB );
	m_XML->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 76, 90, 90, false, wxEmptyString ) );
	m_XML->SetMinSize( wxSize( 500,350 ) );
	
	bSizer9->Add( m_XML, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	m_Validate = new wxButton( this, wxID_ANY, wxT("Validate"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer10->Add( m_Validate, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer9->Add( bSizer10, 0, 0, 5 );
	
	m_stdButton = new wxStdDialogButtonSizer();
	m_stdButtonSave = new wxButton( this, wxID_SAVE );
	m_stdButton->AddButton( m_stdButtonSave );
	m_stdButtonCancel = new wxButton( this, wxID_CANCEL );
	m_stdButton->AddButton( m_stdButtonCancel );
	m_stdButtonHelp = new wxButton( this, wxID_HELP );
	m_stdButton->AddButton( m_stdButtonHelp );
	m_stdButton->Realize();
	bSizer9->Add( m_stdButton, 0, wxALIGN_RIGHT|wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer9 );
	this->Layout();
	bSizer9->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( xmlscriptdlggen::onCancel ) );
	m_Validate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( xmlscriptdlggen::onValidate ), NULL, this );
	m_stdButtonCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( xmlscriptdlggen::onCancel ), NULL, this );
	m_stdButtonHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( xmlscriptdlggen::onHelp ), NULL, this );
	m_stdButtonSave->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( xmlscriptdlggen::onSave ), NULL, this );
}

xmlscriptdlggen::~xmlscriptdlggen()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( xmlscriptdlggen::onCancel ) );
	m_Validate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( xmlscriptdlggen::onValidate ), NULL, this );
	m_stdButtonCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( xmlscriptdlggen::onCancel ), NULL, this );
	m_stdButtonHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( xmlscriptdlggen::onHelp ), NULL, this );
	m_stdButtonSave->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( xmlscriptdlggen::onSave ), NULL, this );
	
}
