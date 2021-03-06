/*
 Rocrail - Model Railroad Software

Copyright (c) 2002-2015 Robert Jan Versluis, Rocrail.net

 


 All rights reserved.
*/

#include "rocutils/impl/addr_impl.h"

#include "rocs/public/mem.h"

static int instCnt = 0;

/** ----- OBase ----- */
static void __del( void* inst ) {
}

static const char* __name( void ) {
  return name;
}

static unsigned char* __serialize( void* inst, long* size ) {
  return NULL;
}

static void __deserialize( void* inst,unsigned char* bytestream ) {
  return;
}

static char* __toString( void* inst ) {
  return NULL;
}

static int __count( void ) {
  return instCnt;
}

static struct OBase* __clone( void* inst ) {
  return NULL;
}

static Boolean __equals( void* inst1, void* inst2 ) {
  return False;
}

static void* __properties( void* inst ) {
  return NULL;
}

static const char* __id( void* inst ) {
  return NULL;
}

static void* __event( void* inst, const void* evt ) {
  return NULL;
}

/** ----- OAddr ----- */


/**  */
static void _fromFADA( int fada ,int* module ,int* port ,int* gate ) {
  int addr = fada;
  *module = addr / 8 + 1;
  *port = (addr % 8) / 2 + 1;
  *gate = (addr % 8) % 2;
}


/**  */
static void _fromPADA( int pada ,int* module ,int* port ) {
  *module = (pada-1) / 4 + 1;
  *port   = (pada-1) % 4 + 1;
}


/**  */
static int _toFADA( int module ,int port ,int gate ) {
  return (module-1) * 8 + (port-1) * 2 + gate;
}


/**  */
static int _toPADA( int module ,int port ) {
  if( module == 0 ) /* already in PADA format */
    return port;
  return (module-1) * 4 + port;
}


/* ----- DO NOT REMOVE OR EDIT THIS INCLUDE LINE! -----*/
#include "rocutils/impl/addr.fm"
/* ----- DO NOT REMOVE OR EDIT THIS INCLUDE LINE! -----*/
