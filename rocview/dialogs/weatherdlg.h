/*
 Rocrail - Model Railroad Software

 Copyright (c) 2002 Robert Jan Versluis, Rocrail.net

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef __weatherdlg__
#define __weatherdlg__

/**
@file
Subclass of WeatherDlgGen, which is generated by wxFormBuilder.
*/

#include "weatherdlggen.h"

//// end generated include
#include "rocs/public/node.h"
#include "rocview/public/listener.h"

/** Implementing WeatherDlgGen */
class WeatherDlg : public WeatherDlgGen, public Listener
{
  iONode m_PropsList;
  iONode m_Props;
  int m_SelectedRow;
  void initLabels();
  void initIndex();
  void initValues();
  void initThemeValues();
  void initThemeIndex();
  bool evaluate();
  bool evaluateTheme();
  void initColorGrid();
  void ShowCol(int col);
  void HideCol(int col);
	protected:
		// Handlers for WeatherDlgGen events.
		void onCancel( wxCommandEvent& event );
		void onOK( wxCommandEvent& event );
		void onApply( wxCommandEvent& event );
		void onActivateWeather( wxCommandEvent& event );
    void onDeactivateWeather( wxCommandEvent& event );
		void onHelp( wxCommandEvent& event );
    void onThemeSelected( wxCommandEvent& event );
    void onThemeAdd( wxCommandEvent& event );
    void onThemeModify( wxCommandEvent& event );
    void onThemeDelete( wxCommandEvent& event );
    void onIndexList( wxCommandEvent& event );
    void onAddWeather( wxCommandEvent& event );
    void onDeleteWeather( wxCommandEvent& event );
    void onActions( wxCommandEvent& event );
    void onColorCellChanged( wxGridEvent& event );
    void onColorCellSelect( wxGridEvent& event );
    void onColorLabelClick( wxGridEvent& event );
    void onColorImport( wxCommandEvent& event );
    void onColorExport( wxCommandEvent& event );
    void onColorCellLeftDClick( wxGridEvent& event );
    void onColorLabelDClick( wxGridEvent& event );
    void onColorWhite( wxCommandEvent& event );
    void onColorWhite2( wxCommandEvent& event );
    void onColorBrightness( wxCommandEvent& event );
    void onColorSaturation( wxCommandEvent& event );
    void onColorPickerWhite1( wxColourPickerEvent& event );
    void onColorPickerWhite2( wxColourPickerEvent& event );
    void onColorPickerBrightness( wxColourPickerEvent& event );
    void onColorPickerSaturation( wxColourPickerEvent& event );

	public:
		/** Constructor */
		WeatherDlg( wxWindow* parent, iONode props );
		void handleEvent( iONode node );
	//// end generated class members
	
};

#endif // __weatherdlg__
