/*
 Rocrail - Model Railroad Software

Copyright (c) 2002-2015 Robert Jan Versluis, Rocrail.net

 


 All rights reserved.
*/

#include "rocdigs/impl/xpressnet_impl.h"
#include "rocdigs/impl/xpressnet/liusb.h"
#include "rocdigs/impl/xpressnet/li101.h"
#include "rocdigs/impl/xpressnet/common.h"
#include "rocrail/wrapper/public/DigInt.h"


Boolean liusbConnect(obj xpressnet) {
  iOXpressNetData data = Data(xpressnet);
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "init serial port for LI-USB..." );
  /*
    - Baudrate: immer 57600 Bit pro Sekunde
    - 8 Datenbits, 1 Startbit, 1 Stopbit, kein Paritybit
    - kein Handshake
   */
  if( !data->enablecom ) {
    return False;
  }
  data->serial = SerialOp.inst( wDigInt.getdevice( data->ini ) );
  SerialOp.setFlow( data->serial, StrOp.equals( wDigInt.cts, wDigInt.getflow( data->ini ) ) ? cts:0 );
  SerialOp.setTimeout( data->serial, wDigInt.gettimeout( data->ini ), wDigInt.gettimeout( data->ini ) );
  SerialOp.setLine( data->serial, 57600, 8, 1, none, wDigInt.isrtsdisabled( data->ini ) );
  return SerialOp.open( data->serial );
}

void liusbDisConnect(obj xpressnet) {
  li101DisConnect(xpressnet);
}

Boolean liusbAvail(obj xpressnet) {
  return li101Avail(xpressnet);
}

void liusbInit(obj xpressnet) {
  li101Init(xpressnet);
}

int liusbRead(obj xpressnet, byte* buffer, Boolean* rspreceived) {
  iOXpressNetData data = Data(xpressnet);
  int len = 0;
  Boolean ok = False;

  if( !data->enablecom ) {
    return 0;
  }
  if( data->dummyio )
    return 0;

  if( MutexOp.wait( data->serialmux ) ) {
    TraceOp.trc( name, TRCLEVEL_BYTE, __LINE__, 9999, "reading bytes from LI-USB..." );
    if( SerialOp.read( data->serial, (char*)buffer, 2 ) ) {
      /* TODO: check if it is the expected frame */
      TraceOp.dump( NULL, TRCLEVEL_BYTE, (char*)buffer, 2 );
      if( SerialOp.read( data->serial, (char*)buffer, 1 ) ) {
        len = (buffer[0] & 0x0f) + 1;
        ok = SerialOp.read( data->serial, (char*)buffer+1, len );
        TraceOp.dump( NULL, TRCLEVEL_BYTE, (char*)buffer, len + 1 );
      }
      else {
        TraceOp.trc( name, TRCLEVEL_BYTE, __LINE__, 9999, "could not read header byte from LI-USB..." );
      }
    }
    else {
      TraceOp.trc( name, TRCLEVEL_BYTE, __LINE__, 9999, "could not read frame from LI-USB..." );
    }
    MutexOp.post( data->serialmux );
  }

  return ok ? len:0;
}

Boolean liusbWrite(obj xpressnet, byte* outin, Boolean* rspexpected) {
  iOXpressNetData data = Data(xpressnet);

  ThreadOp.sleep( 50 );

  int len = 0;
  Boolean rc = False;
  unsigned char out[256];

  *rspexpected = True; /* LIUSB or CS will confirm every command */

  len = makeChecksum(outin);

  if( len == 0 ) {
    TraceOp.trc( name, TRCLEVEL_WARNING, __LINE__, 9999, "zero bytes to write LI-USB" );
    return False;
  }

  /* make extra header for LI-USB*/
  MemOp.copy( out+2, outin, len );

  len = len+2;
  out[0] = 0xFF;
  out[1] = 0xFE;

  if( data->dummyio || !data->enablecom ) {
    TraceOp.dump( NULL, TRCLEVEL_BYTE, (char*)out, len );
    *rspexpected = False;
    return True;
  }

  if( MutexOp.wait( data->serialmux ) ) {
    TraceOp.trc( name, TRCLEVEL_BYTE, __LINE__, 9999, "writing bytes to LI-USB" );
    TraceOp.dump( NULL, TRCLEVEL_BYTE, (char*)out, len );
    rc = SerialOp.write( data->serial, (char*)out, len );
    MutexOp.post( data->serialmux );
  }

  return rc;
}

