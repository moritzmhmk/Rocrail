#!/bin/sh
if [ ! -e ~/rocrail ] ; then
	mkdir ~/rocrail
fi

if [ ! -e ~/rocrail/rocrail.htb ] ; then
	cp /opt/rocrail/rocrail.htb ~/rocrail
fi

if [ ! -e ~/rocrail/rocgui.ini ] ; then
	cp /opt/rocrail/default/rocgui.ini ~/rocrail
	cp /opt/rocrail/default/plan.xml ~/rocrail
	cp /opt/rocrail/default/neustadt.xml ~/rocrail
fi

cd ~/rocrail

if [ ! -e ~/rocrail/icons ] ; then
	ln -s /opt/rocrail/icons ~/rocrail/icons
fi

/opt/rocrail/rocgui $1 $2 $3 -modview

