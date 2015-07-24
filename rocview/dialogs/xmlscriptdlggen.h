///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 12 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __xmlscriptdlggen__
#define __xmlscriptdlggen__

#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class xmlscriptdlggen
///////////////////////////////////////////////////////////////////////////////
class xmlscriptdlggen : public wxDialog 
{
	private:
	
	protected:
		wxTextCtrl* m_XML;
		wxButton* m_Validate;
		wxStdDialogButtonSizer* m_stdButton;
		wxButton* m_stdButtonSave;
		wxButton* m_stdButtonCancel;
		wxButton* m_stdButtonHelp;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onCancel( wxCloseEvent& event ) { event.Skip(); }
		virtual void onValidate( wxCommandEvent& event ) { event.Skip(); }
		virtual void onCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void onHelp( wxCommandEvent& event ) { event.Skip(); }
		virtual void onSave( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		xmlscriptdlggen( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("XmlScript"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER );
		~xmlscriptdlggen();
	
};

#endif //__xmlscriptdlggen__
