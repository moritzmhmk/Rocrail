/* Rocrail - Model Railroad Software Copyright (C) 2002-2013 Rob Versluis, Rocrail.net Without an official permission commercial use is not permitted. Forking this project is not permitted. This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*/#ifndef WX_PRECOMP    #include "wx/wx.h"#endif#include "throttledlg.h"#include "rocs/public/str.h"#include "rocs/public/trace.h"#include "rocs/public/system.h"#include "rocview/public/guiapp.h"#include "rocview/public/base.h"#include "rocview/dialogs/locseldlg.h"#include "rocrail/wrapper/public/Plan.h"#include "rocrail/wrapper/public/Loc.h"#include "rocrail/wrapper/public/Car.h"#include "rocrail/wrapper/public/SysCmd.h"#include "rocrail/wrapper/public/FunCmd.h"#include "rocrail/wrapper/public/FunDef.h"#include "rocrail/wrapper/public/DataReq.h"#include "rocview/wrapper/public/Gui.h"#include "rocview/xpm/nopict.xpm"ThrottleDlg::ThrottleDlg( wxWindow* parent )  :ThrottleDlgGen( parent ){  this->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ThrottleDlg::onButton ) );  this->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ThrottleDlg::onSlider ) );  m_LocoImage->SetBitmapLabel( wxBitmap(nopict_xpm) );  GetSizer()->Fit(this);  GetSizer()->Layout();  m_Props = NULL;  m_iFnGroup = 0;  m_bDir = true;  SetTitle(wxGetApp().getMsg( "locctrl" ));  if( StrOp.len( wGui.getdirimagefwd(wxGetApp().getIni()) ) > 0 && StrOp.len( wGui.getdirimagerev(wxGetApp().getIni()) )) {    m_Dir->SetIcon( m_bDir ? getIcon(wGui.getdirimagefwd(wxGetApp().getIni())):getIcon(wGui.getdirimagerev(wxGetApp().getIni())) );  }}void ThrottleDlg::onSlider(wxScrollEvent& event) {  TraceOp.trc( "throttledlg", TRCLEVEL_INFO, __LINE__, 9999, "slider %d", m_SpeedSlider->GetValue() );  m_iSpeed = m_SpeedSlider->GetValue();  speedCmd( event.GetEventType() != wxEVT_SCROLL_THUMBTRACK );}wxBitmap* ThrottleDlg::getIcon(const char* icon) {  wxBitmap* bitmap = NULL;  wxBitmapType bmptype = wxBITMAP_TYPE_XPM;  if( StrOp.endsWithi( icon, ".gif" ) )    bmptype = wxBITMAP_TYPE_GIF;  else if( StrOp.endsWithi( icon, ".png" ) )    bmptype = wxBITMAP_TYPE_PNG;  TraceOp.trc( "frame", TRCLEVEL_INFO, __LINE__, 9999, "get icon %s", icon );  const char* imagepath = wGui.getimagepath(wxGetApp().m_Ini);  static char pixpath[256];  StrOp.fmtb( pixpath, "%s%c%s", imagepath, SystemOp.getFileSeparator(), FileOp.ripPath( icon ) );  if( FileOp.exist(pixpath))    bitmap = new wxBitmap(wxString(pixpath,wxConvUTF8), bmptype);  else {    // request the image from server:    iONode node = NodeOp.inst( wDataReq.name(), NULL, ELEMENT_NODE );    wDataReq.setid( node, wLoc.getid(m_Props) );    wDataReq.setcmd( node, wDataReq.get );    wDataReq.settype( node, wDataReq.image );    wDataReq.setfilename( node, icon );    wxGetApp().sendToRocrail( node );  }  return bitmap;}void ThrottleDlg::setFLabels() {  LEDButton* m_F[15] = {m_F0,m_F1,m_F2,m_F3,m_F4,m_F5,m_F6,m_F7,m_F8,m_F9,m_F10,m_F11,m_F12,m_F13,m_F14};  TraceOp.trc( "throttledlg", TRCLEVEL_INFO, __LINE__, 9999, "setFLabels group=%d", m_iFnGroup );  m_F0->SetLabel( _T("lights") );  m_F0->SetIcon(NULL);  for( int i = 1; i < 15; i++ ) {    m_F[i]->SetLabel( wxString::Format(_T("F%d"), i + m_iFnGroup * 14) );    m_F[i]->SetIcon(NULL);  }  m_F0->setLED(wLoc.isfn( m_Props )?true:false);  int fx = wLoc.getfx(m_Props);  if( m_iFnGroup > 1 )    m_iFnGroup = 0;  fx = fx >> (m_iFnGroup * 14 );  for(int i = 0; i < 14; i++ ) {    m_F[i+1]->setLED ((fx & (0x0001 << i) )?true:false );  }  iONode fundef = wLoc.getfundef( m_Props );  while( fundef != NULL ) {    wxString fntxt = wxString(wFunDef.gettext( fundef ),wxConvUTF8);    int fn = wFunDef.getfn( fundef );    int idx = fn - m_iFnGroup*14;    if( fn == 0 ) {      if( wxGetApp().getFrame()->isTooltip())        m_F0->SetToolTip( fntxt );      if( wFunDef.geticon(fundef) != NULL && StrOp.len( wFunDef.geticon(fundef) ) > 0 ) {        m_F0->SetIcon(getIcon(wFunDef.geticon(fundef)));      }      else {        m_F0->SetIcon(NULL);        m_F0->SetLabel(fntxt);      }    }    else if( idx > 0 && idx < 15 ) {      TraceOp.trc( "throttledlg", TRCLEVEL_INFO, __LINE__, 9999, "funtion[%d] index=%d group=%d", fn, idx, m_iFnGroup );      if( wxGetApp().getFrame()->isTooltip() )        m_F[idx]->SetToolTip( fntxt );      if( wFunDef.geticon(fundef) != NULL && StrOp.len( wFunDef.geticon(fundef) ) > 0 ) {        m_F[idx]->SetIcon(getIcon(wFunDef.geticon(fundef)));      }      else {        m_F[idx]->SetIcon(NULL);        m_F[idx]->SetLabel(fntxt);      }    }    fundef = wLoc.nextfundef( m_Props, fundef );  }}void ThrottleDlg::updateImage() {  iONode lc = m_Props;  if( lc != NULL && wLoc.getimage( lc ) != NULL && StrOp.len(wLoc.getimage( lc )) > 0 ) {    wxBitmapType bmptype = wxBITMAP_TYPE_XPM;    if( StrOp.endsWithi( wLoc.getimage( lc ), ".gif" ) )      bmptype = wxBITMAP_TYPE_GIF;    else if( StrOp.endsWithi( wLoc.getimage( lc ), ".png" ) )      bmptype = wxBITMAP_TYPE_PNG;    const char* imagepath = wGui.getimagepath(wxGetApp().getIni());    static char pixpath[256];    StrOp.fmtb( pixpath, "%s%c%s", imagepath, SystemOp.getFileSeparator(), FileOp.ripPath( wLoc.getimage( lc ) ) );    if( FileOp.exist(pixpath)) {      TraceOp.trc( "throttledlg", TRCLEVEL_INFO, __LINE__, 9999, "picture [%s]", pixpath );      m_LocoImage->SetBitmapLabel( wxBitmap(wxString(pixpath,wxConvUTF8), bmptype) );    }    else {      TraceOp.trc( "throttledlg", TRCLEVEL_WARNING, __LINE__, 9999, "picture [%s] not found", pixpath );      m_LocoImage->SetBitmapLabel( wxBitmap(nopict_xpm) );    }    m_LocoImage->SetToolTip(wxString(wLoc.getdesc( lc ),wxConvUTF8));  }  else {    m_LocoImage->SetBitmapLabel( wxBitmap(nopict_xpm) );  }  m_LocoImage->Refresh();  SetTitle(wxGetApp().getMsg( "locctrl" ) + _T(" ") + wxString(wLoc.getid( lc ),wxConvUTF8) );}void ThrottleDlg::speedCmd(bool sendCmd){  if( !sendCmd || m_Props == NULL ) {    return;  }  TraceOp.trc( "throttledlg", TRCLEVEL_INFO, __LINE__, 9999, "speedCmd %d", m_iSpeed );  m_Speed->SetValue( wxString::Format(wxT("%d"), m_iSpeed) );  iONode cmd = NodeOp.inst( wLoc.name(), NULL, ELEMENT_NODE );  wLoc.setid( cmd, wLoc.getid( m_Props ) );  wLoc.setV( cmd, m_iSpeed );  wLoc.setfn( cmd, m_bFn?True:False );  wLoc.setdir( cmd, m_bDir?True:False );  wxGetApp().sendToRocrail( cmd );  cmd->base.del(cmd);}void ThrottleDlg::funCmd(int fidx, bool on){  if( m_Props == NULL )    return;  TraceOp.trc( "throttledlg", TRCLEVEL_INFO, __LINE__, 9999, "funCmd %d %s", fidx, on?"ON":"OFF" );  iONode cmd = NodeOp.inst( wFunCmd.name(), NULL, ELEMENT_NODE );  wFunCmd.setfnchanged ( cmd, fidx );  wFunCmd.setid ( cmd, wLoc.getid( m_Props ) );  char f[32];  StrOp.fmtb(f, "f%d", fidx);  NodeOp.setBool(cmd, f, on?True:False);  wxGetApp().sendToRocrail( cmd );  cmd->base.del(cmd);}void ThrottleDlg::onButton(wxCommandEvent& event) {  TraceOp.trc( "throttledlg", TRCLEVEL_INFO, __LINE__, 9999, "OnButton event..." );  if ( event.GetEventObject() == m_LocoImage ) {    TraceOp.trc( "throttledlg", TRCLEVEL_INFO, __LINE__, 9999, "LocoImage" );    LocSelDlg*  dlg = new LocSelDlg(this, NULL, false );    if( wxID_OK == dlg->ShowModal() ) {      m_Props = dlg->getProperties();      if( m_Props != NULL ) {        m_iFnGroup = 0;        updateImage();        setFLabels();      }    }    dlg->Destroy();    Raise();  }  else if ( event.GetEventObject() == m_Dir ) {    TraceOp.trc( "throttledlg", TRCLEVEL_INFO, __LINE__, 9999, "Direction" );    m_bDir = ! m_bDir;    m_Dir->SetLabel( m_bDir?_T(">>"):_T("<<") );    if( StrOp.len( wGui.getdirimagefwd(wxGetApp().getIni()) ) > 0 && StrOp.len( wGui.getdirimagerev(wxGetApp().getIni()) )) {      m_Dir->SetIcon( m_bDir ? getIcon(wGui.getdirimagefwd(wxGetApp().getIni())):getIcon(wGui.getdirimagerev(wxGetApp().getIni())) );    }    if( wxGetApp().getFrame()->isTooltip())      m_Dir->SetToolTip( m_bDir?wxGetApp().getMsg( "forwards" ):wxGetApp().getMsg( "reverse" ) );    speedCmd(true);  }  else if ( event.GetEventObject() == m_Stop ) {    TraceOp.trc( "throttledlg", TRCLEVEL_INFO, __LINE__, 9999, "Stop" );    m_iSpeed = 0;    m_SpeedSlider->SetValue( m_iSpeed, true );    speedCmd(true);  }  else if ( event.GetEventObject() == m_Break ) {    TraceOp.trc( "throttledlg", TRCLEVEL_INFO, __LINE__, 9999, "Break" );    iONode cmd = NodeOp.inst( wSysCmd.name(), NULL, ELEMENT_NODE );    wLoc.setcmd( cmd, wSysCmd.stop );    wSysCmd.setinformall(cmd, True);    wxGetApp().sendToRocrail( cmd );    cmd->base.del(cmd);  }  else if ( event.GetEventObject() == m_FG ) {    TraceOp.trc( "throttledlg", TRCLEVEL_INFO, __LINE__, 9999, "FG" );    m_iFnGroup++;    if( m_iFnGroup > 1 )      m_iFnGroup = 0;    setFLabels();  }  else {    LEDButton* m_F[15] = {m_F0,m_F1,m_F2,m_F3,m_F4,m_F5,m_F6,m_F7,m_F8,m_F9,m_F10,m_F11,m_F12,m_F13,m_F14};    for( int i = 0; i < 15; i++ ) {      if( event.GetEventObject() == m_F[i] ) {        TraceOp.trc( "throttledlg", TRCLEVEL_INFO, __LINE__, 9999, "F%d", i );        m_F[i]->setLED(!m_F[i]->ON);        funCmd(i+m_iFnGroup*14, m_F[i]->ON);        break;      }    }  }}void ThrottleDlg::onClose( wxCloseEvent& event ) {  Destroy();}