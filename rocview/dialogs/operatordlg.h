/*
 Rocrail - Model Railroad Software

Copyright (c) 2002-2015 Robert Jan Versluis, Rocrail.net

 


 All rights reserved.
*/
#ifndef __operatordlg__
#define __operatordlg__

/**
@file
Subclass of operatordlggen, which is generated by wxFormBuilder.
*/

#include "operatordlggen.h"
#include "basenotebook.h"

#include "rocs/public/node.h"

/** Implementing operatordlggen */
class OperatorDlg : public operatordlggen
{
  iONode m_Props;
  iONode m_CarProps;
  bool   m_bSave;
  int    m_TabAlign;
  int    m_SetPage;
  const char* m_BlockID;

  void initLabels();
  void evaluate();
  void initIndex();
  void initValues();
  void initConsist();
  void initLocos();
  void initLocationCombo();
  int findCarID( const char* ID );
  void addCarToConsistList( int idx, iONode car );
  void resizeConsistColumns();
  int getVMax( iONode props );
  int getCargoNr(iONode props);

public:
	/** Constructor */
	OperatorDlg( wxWindow* parent, iONode p_Props, bool save=true, const char* blockid=NULL );

	void onNewOperator( wxCommandEvent& event );
    void onDelOperator( wxCommandEvent& event );
    void onLocoImage( wxCommandEvent& event );
    void onLocomotiveCombo( wxCommandEvent& event );
    void onReserve( wxCommandEvent& event );
    void onRun( wxCommandEvent& event );
    void onGotoMan( wxCommandEvent& event );
    void onCarImage( wxCommandEvent& event );
    void onAddCar( wxCommandEvent& event );
    void onLeaveCar( wxCommandEvent& event );
    void onCarCard( wxCommandEvent& event );
    void onWayBill( wxCommandEvent& event );
    void onApply( wxCommandEvent& event );
    void onCancel( wxCommandEvent& event );
    void onOK( wxCommandEvent& event );
    void onSetPage( wxCommandEvent& event );
    void onOperatorlist( wxListEvent& event );
    void onCarListSelected( wxListEvent& event );
    void OnCopy( wxCommandEvent& event );
    void onSetLocation( wxCommandEvent& event ) ;
    iONode getSelected();
    void setSelection(const char* ID);
    int findID( const char* ID );
    void onTabChanged( wxNotebookEvent& event );
    void onShowAllOperators( wxCommandEvent& event );
    void onHelp( wxCommandEvent& event );

};

#endif // __operatordlg__
