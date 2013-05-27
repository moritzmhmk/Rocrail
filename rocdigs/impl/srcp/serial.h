/*
 Rocrail - Model Railroad Software

 Copyright (C) 2002-2013 Rob Versluis, Rocrail.net

 Without an official permission commercial use is not permitted.
 Forking this project is not permitted.

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
#ifndef SRCPSERIAL_H_
#define SRCPSERIAL_H_

Boolean serialInit( obj inst );
int serialConnect( obj inst, Boolean info );
void  serialDisconnect( obj inst, Boolean info );

int serialRead ( obj inst, char *cmd, Boolean info );
int serialWrite( obj inst, const char *cmd, char* rsp, Boolean info );
Boolean serialAvailable( obj inst );

#endif /*SRCPSERIAL_H_*/
