/*
 Rocrail - Model Railroad Software
 Copyright (c) 2002-2015 Robert Jan Versluis, Rocrail.net
 All rights reserved.
*/
#ifndef __sensorevents__
#define __sensorevents__

/**
@file
Subclass of SensorEventsGen, which is generated by wxFormBuilder.
*/

#include "sensoreventsgen.h"

//// end generated include
#include "rocs/public/node.h"

/** Implementing SensorEventsGen */
class SensorEventsDlg : public SensorEventsGen
{
  void initLabels();
  void initValues();

  iONode m_FbEvent;
  int    m_SortCol;

	public:
		/** Constructor */
		SensorEventsDlg( wxWindow* parent );
	//// end generated class members
		void onRefresh( wxCommandEvent& event );
		void onOK( wxCommandEvent& event );
		void onClose( wxCloseEvent& event );
		void onReset( wxCommandEvent& event );
		void onListSelected( wxListEvent& event );
		void onColClick( wxListEvent& event );
		void onDrag( wxListEvent& event );
		bool Validate();
		void onHelp( wxCommandEvent& event );
		void onAssign( wxCommandEvent& event ) ;
		void onFindIdent( wxCommandEvent& event );
		void onListActivated( wxListEvent& event );
};

#endif // __sensorevents__
