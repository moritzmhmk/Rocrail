/*
 Rocrail - Model Railroad Software
 Copyright (c) 2002-2015 Robert Jan Versluis, Rocrail.net
 All rights reserved.
*/

#include "zoomdlg.h"

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/defs.h"
#endif

#include "rocview/public/guiapp.h"


ZoomDlg::ZoomDlg( wxWindow* parent, int percent ):ZoomDlgGen( parent )
{
  m_ZoomValue->Append(_T("25"));
  m_ZoomValue->Append(_T("30"));
  m_ZoomValue->Append(_T("40"));
  m_ZoomValue->Append(_T("50"));
  m_ZoomValue->Append(_T("60"));
  m_ZoomValue->Append(_T("70"));
  m_ZoomValue->Append(_T("75"));
  m_ZoomValue->Append(_T("80"));
  m_ZoomValue->Append(_T("90"));
  m_ZoomValue->Append(_T("100"));
  m_ZoomValue->Append(_T("110"));
  m_ZoomValue->Append(_T("120"));
  m_ZoomValue->Append(_T("130"));
  m_ZoomValue->Append(_T("140"));
  m_ZoomValue->Append(_T("150"));
  m_ZoomValue->Append(_T("160"));
  m_ZoomValue->Append(_T("170"));
  m_ZoomValue->Append(_T("180"));
  m_ZoomValue->Append(_T("190"));
  m_ZoomValue->Append(_T("200"));

  m_ZoomValue->SetValue( wxString::Format( _T("%d"), percent) );
  m_ZoomSlider->SetValue(percent);
  SetTitle(wxGetApp().getMsg( "zoom" ));
}

void ZoomDlg::onCancel( wxCommandEvent& event )
{
  EndModal( 0 );
}

void ZoomDlg::onOK( wxCommandEvent& event )
{
  EndModal( wxID_OK );
}

void ZoomDlg::onZoomSelect( wxCommandEvent& event ) {
  int percent = atoi( (const char*)m_ZoomValue->GetValue().mb_str(wxConvUTF8) );
  m_ZoomSlider->SetValue( percent );
  wxGetApp().getFrame()->Zoom(percent);
}


void ZoomDlg::onZoomEnter( wxCommandEvent& event ) {
  m_ZoomSlider->SetValue( atoi( (const char*)m_ZoomValue->GetValue().mb_str(wxConvUTF8) ) );
}


void ZoomDlg::onZoomThumb( wxScrollEvent& event ) {
  int percent = m_ZoomSlider->GetValue();
  m_ZoomValue->SetValue( wxString::Format( _T("%d"), percent ) );
  wxGetApp().getFrame()->Zoom(percent);
  event.Skip();
}


void ZoomDlg::onZoomRelease( wxScrollEvent& event ) {
  int percent = m_ZoomSlider->GetValue();
  int rest = percent % 5;
  percent -= percent % 5;
  if( rest > 2 )
    percent += 5;
  m_ZoomValue->SetValue( wxString::Format( _T("%d"), percent) );
  m_ZoomSlider->SetValue(percent);
  wxGetApp().getFrame()->Zoom(percent);
  event.Skip();
}


int ZoomDlg::GetValue() {
  return atoi( (const char*)m_ZoomValue->GetValue().mb_str(wxConvUTF8) );
}

void ZoomDlg::onHelp( wxCommandEvent& event ) {
  wxGetApp().openLink( "rocgui-menu", "zoom" );
}

