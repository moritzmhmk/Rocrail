/*
 Rocrail - Model Railroad Software

 Copyright (C) 2002-2011 - Rob Versluis <r.j.versluis@rocrail.net>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "cbusdlg.h"
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/defs.h"
#endif


#include "rocview/public/guiapp.h"
#include "rocrail/wrapper/public/DigInt.h"
#include "rocrail/wrapper/public/CBus.h"

CbusDlg::CbusDlg( wxWindow* parent, iONode props ):cbusdlggen( parent ) {
  m_Props = props;

  iONode cbusini = wDigInt.getcbus(m_Props);

  if( cbusini == NULL ) {
    cbusini = NodeOp.inst( wCBus.name(), m_Props, ELEMENT_NODE );
    NodeOp.addChild( m_Props, cbusini );
  }



  initLabels();
  initValues();

  GetSizer()->Layout();
  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);

}

void CbusDlg::initLabels() {
  m_labIID->SetLabel( wxGetApp().getMsg( "iid" ) );
  m_labDevice->SetLabel( wxGetApp().getMsg( "device" ) );
  m_labHost->SetLabel( wxGetApp().getMsg( "host" ) );
  m_labPort->SetLabel( wxGetApp().getMsg( "port" ) );
  m_Sublib->SetLabel(wxGetApp().getMsg( "sublib" ));

  // Options
  m_labSwTime->SetLabel( wxGetApp().getMsg( "switchtime" ) );
  m_labPurgetime->SetLabel( wxGetApp().getMsg( "purgetime" ) );

  // Buttons
  m_StdButtonOK->SetLabel( wxGetApp().getMsg( "ok" ) );
  m_StdButtonCancel->SetLabel( wxGetApp().getMsg( "cancel" ) );


}

void CbusDlg::initValues() {
  iONode cbusini = wDigInt.getcbus(m_Props);

  // General
  m_IID->SetValue( wxString( wDigInt.getiid( m_Props ), wxConvUTF8 ) );
  m_CANID->SetValue( wCBus.getcid( cbusini ) );
  m_SOD->SetValue( wCBus.getsodaddr( cbusini ) );
  m_SwTime->SetValue( wDigInt.getswtime( m_Props ) );
  m_Purgetime->SetValue( wCBus.getpurgetime( cbusini ) );
  m_LoaderTime->SetValue( wCBus.getloadertime( cbusini ) );
  m_Device->SetValue( wxString( wDigInt.getdevice( m_Props ), wxConvUTF8 ) );
  m_Host->SetValue( wxString( wDigInt.gethost( m_Props ), wxConvUTF8 ) );
  m_Port->SetValue( wDigInt.getport( m_Props ) );

  if( StrOp.equals( wDigInt.sublib_usb, wDigInt.getsublib(m_Props) )) {
    // USB
    m_Sublib->SetSelection(0);
  }
  else if( StrOp.equals( wDigInt.sublib_tcp, wDigInt.getsublib(m_Props) )) {
    // TCP
    m_Sublib->SetSelection(2);
  }
  else {
    // Serial
    m_Sublib->SetSelection(1);
  }

  wxCommandEvent event( 0, 0 );
  OnSublib(event);

  m_ShortEvents->SetValue( wCBus.isshortevents(cbusini) ? true:false);
  m_FonFof->SetValue( wCBus.isfonfof(cbusini) ? true:false);
}

void CbusDlg::evaluate() {
  iONode cbusini = wDigInt.getcbus(m_Props);

  wDigInt.setiid( m_Props, m_IID->GetValue().mb_str(wxConvUTF8) );
  wCBus.setcid( cbusini, m_CANID->GetValue() );
  wCBus.setsodaddr( cbusini, m_SOD->GetValue() );
  wDigInt.setdevice( m_Props, m_Device->GetValue().mb_str(wxConvUTF8) );
  wDigInt.setswtime( m_Props, m_SwTime->GetValue() );
  wCBus.setpurgetime( cbusini, m_Purgetime->GetValue() );
  wCBus.setloadertime( cbusini, m_LoaderTime->GetValue() );
  wDigInt.sethost( m_Props, m_Host->GetValue().mb_str(wxConvUTF8) );
  wDigInt.setport( m_Props, m_Port->GetValue() );

  if( m_Sublib->GetSelection() == 0 )
    wDigInt.setsublib(m_Props, wDigInt.sublib_usb );
  else if( m_Sublib->GetSelection() == 2 )
    wDigInt.setsublib(m_Props, wDigInt.sublib_tcp );
  else
    wDigInt.setsublib(m_Props, wDigInt.sublib_serial );

  wCBus.setshortevents(cbusini, m_ShortEvents->IsChecked()?True:False);
  wCBus.setfonfof(cbusini, m_FonFof->IsChecked()?True:False);

}


void CbusDlg::OnSublib( wxCommandEvent& event ) {
  if( m_Sublib->GetSelection() == 2 ) {
    m_Host->Enable(true);
    m_Port->Enable(true);
    m_Device->Enable(false);
  }
  else {
    m_Host->Enable(false);
    m_Port->Enable(false);
    m_Device->Enable(true);
  }
}

void CbusDlg::OnCancel( wxCommandEvent& event ) {
  EndModal( wxID_CANCEL );
}

void CbusDlg::OnOK( wxCommandEvent& event ) {
  evaluate();
  EndModal( wxID_OK );
}
