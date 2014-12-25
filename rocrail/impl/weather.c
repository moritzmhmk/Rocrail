/*
 Rocrail - Model Railroad Software

 Copyright (C) 2002-2014 Rob Versluis, Rocrail.net

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


#include "rocrail/impl/weather_impl.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "rocs/public/mem.h"
#include "rocs/public/trace.h"
#include "rocs/public/node.h"
#include "rocs/public/thread.h"
#include "rocs/public/strtok.h"
#include "rocs/public/list.h"
#include "rocs/public/system.h"

#include "rocrail/public/app.h"
#include "rocrail/public/control.h"
#include "rocrail/public/output.h"
#include "rocrail/public/model.h"

#include "rocrail/wrapper/public/RocRail.h"
#include "rocrail/wrapper/public/Weather.h"
#include "rocrail/wrapper/public/Sunrise.h"
#include "rocrail/wrapper/public/Noon.h"
#include "rocrail/wrapper/public/Sunset.h"
#include "rocrail/wrapper/public/Night.h"
#include "rocrail/wrapper/public/Output.h"
#include "rocrail/wrapper/public/Color.h"
#include "rocrail/wrapper/public/WeatherTheme.h"


static int instCnt = 0;

/** ----- OBase ----- */
static void __del( void* inst ) {
  if( inst != NULL ) {
    iOWeatherData data = Data(inst);
    /* Cleanup data->xxx members...*/
    
    freeMem( data );
    freeMem( inst );
    instCnt--;
  }
  return;
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

/** ----- OWeather ----- */
static void __doInitialize(iOWeather weather, Boolean day, Boolean night) {
  iOWeatherData data = Data(weather);
  iOModel model = AppOp.getModel();

  if( data->props != NULL ) {
    iONode nightProps = wWeather.getnight(data->props);


    if( day ) {
      TraceOp.trc( name, TRCLEVEL_BYTE, __LINE__, 9999, "initialize all day lamps" );
      iOStrTok tok = StrTokOp.inst( wWeather.getoutputs(data->props), ',' );
      while( StrTokOp.hasMoreTokens(tok) ) {
        const char* id = StrTokOp.nextToken(tok);
        iOOutput output = ModelOp.getOutput(model, id);
        if( output != NULL ) {
          iONode cmd = NodeOp.inst( wOutput.name(), NULL, ELEMENT_NODE);
          wOutput.setaddr(cmd, wOutput.getaddr(OutputOp.base.properties(output)));
          wOutput.setvalue(cmd, 0);
          wOutput.setcmd(cmd, wOutput.off);
          OutputOp.cmd(output, cmd, True);
        }
      };
      StrTokOp.base.del(tok);
    }

    if( night && nightProps != NULL ) {
      TraceOp.trc( name, TRCLEVEL_BYTE, __LINE__, 9999, "initialize all night lamps" );
      iOStrTok tok = StrTokOp.inst( wNight.getoutputs(nightProps), ',' );
      while( StrTokOp.hasMoreTokens(tok) ) {
        const char* id = StrTokOp.nextToken(tok);
        iOOutput output = ModelOp.getOutput(model, id);
        if( output != NULL ) {
          iONode cmd = NodeOp.inst( wOutput.name(), NULL, ELEMENT_NODE);
          wOutput.setaddr(cmd, wOutput.getaddr(OutputOp.base.properties(output)));
          wOutput.setvalue(cmd, 0);
          wOutput.setcmd(cmd, wOutput.off);
          OutputOp.cmd(output, cmd, True);
        }
      };
      StrTokOp.base.del(tok);
    }
  }
}


static void __doDaylight(iOWeather weather, int hour, int min, Boolean shutdown, Boolean clearTheme ) {
  iOWeatherData data = Data(weather);
  iOModel model = AppOp.getModel();
  iOList list = ListOp.inst();
  iOList nightList = ListOp.inst();
  int minutes   = hour * 60 + min;

  if( data->props == NULL ) {
    return;
  }

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "do daylight at %02d:%02d (%d) on %s", hour, min, minutes, wWeather.getoutputs(data->props) );

  iOStrTok tok = StrTokOp.inst( wWeather.getoutputs(data->props), ',' );
  while( StrTokOp.hasMoreTokens(tok) ) {
    const char* id = StrTokOp.nextToken(tok);
    iOOutput output = ModelOp.getOutput(model, id);
    if( output != NULL ) {
      ListOp.add(list, (obj)output);
    }
  };
  StrTokOp.base.del(tok);

  if( ListOp.size(list) > 0 ) {
    iONode sunriseProps = wWeather.getsunrise(data->props);
    iONode noonProps    = wWeather.getnoon(data->props);
    iONode sunsetProps  = wWeather.getsunset(data->props);
    iONode nightProps   = wWeather.getnight(data->props);

    if( sunriseProps == NULL ) {
      sunriseProps = NodeOp.inst(wSunrise.name(), data->props, ELEMENT_NODE );
      NodeOp.addChild(data->props, sunriseProps);
      wSunrise.sethour(sunriseProps, 6) ;
    }
    if( noonProps == NULL ) {
      noonProps = NodeOp.inst(wNoon.name(), data->props, ELEMENT_NODE );
      NodeOp.addChild(data->props, noonProps);
      wNoon.sethour(sunsetProps, 12) ;
    }
    if( sunsetProps == NULL ) {
      sunsetProps = NodeOp.inst(wSunset.name(), data->props, ELEMENT_NODE );
      NodeOp.addChild(data->props, sunsetProps);
      wSunset.sethour(sunsetProps, 18) ;
    }
    if( nightProps == NULL ) {
      nightProps = NodeOp.inst(wNight.name(), data->props, ELEMENT_NODE );
      NodeOp.addChild(data->props, nightProps);
    }

    iOStrTok tok = StrTokOp.inst( wNight.getoutputs(nightProps), ',' );
    while( StrTokOp.hasMoreTokens(tok) ) {
      const char* id = StrTokOp.nextToken(tok);
      iOOutput output = ModelOp.getOutput(model, id);
      if( output != NULL ) {
        ListOp.add(nightList, (obj)output);
      }
    };
    StrTokOp.base.del(tok);



    int sunrise = wSunrise.gethour(sunriseProps) * 60 + wSunrise.getminute(sunriseProps);
    int noon    = wNoon.gethour(noonProps) * 60 + wNoon.getminute(noonProps);
    int sunset  = wSunset.gethour(sunsetProps) * 60 + wSunset.getminute(sunsetProps);

    int sunriseRed   = wSunrise.getred(sunriseProps);
    int sunriseGreen = wSunrise.getgreen(sunriseProps);
    int sunriseBlue  = wSunrise.getblue(sunriseProps);

    int sunsetRed   = wSunset.getred(sunsetProps);
    int sunsetGreen = wSunset.getgreen(sunsetProps);
    int sunsetBlue  = wSunset.getblue(sunsetProps);

    float maxbri       = wWeather.getmaxbri(data->props);
    int   minbri       = wWeather.getminbri(data->props);
    float percent      = 0.0;
    float brightness   = 0.0;
    float red          = wNoon.getred(noonProps);
    float green        = wNoon.getgreen(noonProps);
    float blue         = wNoon.getblue(noonProps);
    float noonRed      = wNoon.getred(noonProps);
    float noonGreen    = wNoon.getgreen(noonProps);
    float noonBlue     = wNoon.getblue(noonProps);
    float colorsliding = wWeather.getcolorsliding(data->props);

    int daylight  = sunset - sunrise;

    Boolean adjustBri = (data->themedim > 0 || clearTheme) ? True:False;

    /* AM */
    if( minutes <= noon && minutes >= sunrise) {
      float range = noon - sunrise;
      percent = (100.0 / range) * (float)(minutes - sunrise);
      float l_brightness = (percent * maxbri) / 100.0;
      brightness = l_brightness;
      adjustBri = True;

      if( minutes <= sunrise + colorsliding ) {
        float pMinutes = minutes - sunrise;
        float redDif = (noonRed - sunriseRed) / colorsliding;
        red = sunriseRed + redDif * pMinutes;
        float greenDif = (noonGreen - sunriseGreen) / colorsliding;
        green = sunriseGreen + greenDif * pMinutes;
        float blueDif = (noonBlue - sunriseBlue) / colorsliding;
        blue = sunriseBlue + blueDif * pMinutes;
        TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "pMinutes=%d sunrise=%d minutes=%d redDif=%d greenDif=%d blueDif=%d",
            (int)pMinutes, sunrise, minutes, (int)redDif, (int)greenDif, (int)blueDif);
      }
    }

    /* PM */
    if( minutes > noon && minutes <= sunset) {
      float range = sunset - noon;
      percent = 100.0 - ((100.0 / range) * (float)(minutes - noon));
      float l_brightness = (percent * maxbri) / 100.0;
      brightness = l_brightness;
      adjustBri = True;

      if( minutes >= sunset - colorsliding ) {
        float pMinutes = sunset - minutes;
        float redDif = (noonRed - sunsetRed) / colorsliding;
        red = sunsetRed + redDif * pMinutes;
        float greenDif = (noonGreen - sunsetGreen) / colorsliding;
        green = sunsetGreen + greenDif * pMinutes;
        float blueDif = (noonBlue - sunsetBlue) / colorsliding;
        blue = sunsetBlue + blueDif * pMinutes;
        TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "pMinutes=%d sunset=%d minutes=%d redDif=%d greenDif=%d blueDif=%d",
            (int)pMinutes, sunset, minutes, (int)redDif, (int)greenDif, (int)blueDif);
      }
    }

    if( data->themedim > 0 ) {
      float l_brightness = brightness;
      float l_percentDim = data->themedim;
      float l_briPercent = l_brightness / 100.0;
      brightness = l_brightness - (l_briPercent * l_percentDim);
    }

    if(adjustBri || shutdown) {
      int LAMPS = ListOp.size(list);
      float segment = 180.0 / (float)(LAMPS-1);
      int n = 0;
      float angle = (180.0 / (float)(sunset - sunrise)) * (minutes-sunrise);
      for( n = 0; n < LAMPS; n++) {
        iOOutput output = (iOOutput)ListOp.get(list, n);
        int lampBri = 0;

        float lampangle = segment * n;
        if( lampangle + segment > angle && lampangle < angle + segment ) {
          float lampPercent = 0.0;

          if( angle < lampangle )
            lampPercent = 100.0 - ((100.0 / segment) * (lampangle - angle));
          else
            lampPercent = 100.0 - ((100.0 / segment) * (angle - lampangle));

          lampBri = (lampPercent * brightness) / 100.0;
        }
        else {
          lampBri = 0;
        }

        if( !wWeather.isslidingdaylight(data->props) ) {
          lampBri = brightness;
          if( lampBri < minbri )
            lampBri = minbri;
        }

        iONode cmd = NodeOp.inst( wOutput.name(), NULL, ELEMENT_NODE);
        iONode color = NodeOp.inst( wColor.name(), cmd, ELEMENT_NODE);
        NodeOp.addChild( cmd, color );
        wColor.setred(color, red);
        wColor.setgreen(color, green);
        wColor.setblue(color, blue);
        wOutput.setaddr(cmd, wOutput.getaddr(OutputOp.base.properties(output)));
        TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999,
            "lamp %s brightness=%d(of %.2f), lampAngle=%.2f sunAngle=%.2f dayminutes=%d sunrise=%d sunset=%d",
            wOutput.getid(OutputOp.base.properties(output)), lampBri, brightness, lampangle, angle, minutes, sunrise, sunset );
        wOutput.setvalue(cmd, lampBri);
        wOutput.setcmd(cmd, shutdown?wOutput.off:wOutput.value);
        OutputOp.cmd(output, cmd, True);

      }
      adjustBri = False;
    }
    else {
      __doInitialize(weather, True, False );
    }

    if( minutes+30 > sunset || minutes-30 < sunrise || shutdown ) {
      /* Night. */
      TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "do nightlight at %02d:%02d (%d) on %s", hour, min, minutes, wNight.getoutputs(nightProps) );
      if( ListOp.size(nightList) > 0 ) {
        int LAMPS = ListOp.size(nightList);
        int bri = wNight.getbri(nightProps);
        int n = 0;

        if( minutes+30 > sunset ) {
          if( sunset - minutes > 0 ) {
            float l_bri = bri;
            l_bri = l_bri - ((l_bri / 30.0) * (sunset - minutes));
            bri = (int)l_bri;
          }
        }

        if( minutes-30 < sunrise ) {
          if( minutes - sunrise > 0 ) {
            float l_bri = bri;
            l_bri = l_bri - ((l_bri / 30.0) * (minutes - sunrise));
            bri = (int)l_bri;
          }
        }

        TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "night brightness=%d of %d", bri, wNight.getbri(nightProps));

        for( n = 0; n < LAMPS; n++) {
          iOOutput output = (iOOutput)ListOp.get(nightList, n);
          iONode cmd   = NodeOp.inst( wOutput.name(), NULL, ELEMENT_NODE);
          iONode color = NodeOp.inst( wColor.name(), cmd, ELEMENT_NODE);
          NodeOp.addChild( cmd, color );
          wOutput.setaddr(cmd, wOutput.getaddr(OutputOp.base.properties(output)));
          wOutput.setvalue(cmd, bri);
          wColor.setred(color, wNight.getred(nightProps));
          wColor.setgreen(color, wNight.getgreen(nightProps));
          wColor.setblue(color, wNight.getblue(nightProps));
          wOutput.setcmd(cmd, shutdown?wOutput.off:wOutput.value);
          OutputOp.cmd(output, cmd, True);
        }
      }
    }
    else {
      __doInitialize(weather, False, True );
    }

  }
  ListOp.base.del(list);
  ListOp.base.del(nightList);

}


static void __checkWeatherThemes(iOWeather weather, int hour, int min ) {
  iOWeatherData data = Data(weather);
  iOModel model = AppOp.getModel();

  if( data->props == NULL ) {
    return;
  }

  TraceOp.trc( name, TRCLEVEL_DEBUG, __LINE__, 9999, "check weather themes at %02d:%02d theme=%X %d", hour, min, data->theme, data->themetimerrand );

  if( data->theme == NULL ) {
    /* select a theme... */
    iONode theme = wWeather.getweathertheme(data->props);
    while( theme != NULL ) {
      if( (hour == wWeatherTheme.gethour(theme) && min == wWeatherTheme.getminute(theme)) ||
          (data->themetimerrand <= 0 && wWeatherTheme.israndom(theme)) || (data->requestedTheme != NULL && StrOp.equals(data->requestedTheme, wWeatherTheme.getid(theme) )) ) {
        data->theme = theme;
        data->themeduration = 0;
        data->themetimer1 = 0;
        data->themedim = 0;
        data->themestartup = True;
        data->themeshutdown = False;
        if( data->requestedTheme != NULL && StrOp.equals(data->requestedTheme, wWeatherTheme.getid(theme) ) ) {
          StrOp.free(data->requestedTheme);
          data->requestedTheme = NULL;
        }
        TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "activating theme [%s]", wWeatherTheme.getid(theme) );
        break;
      }
      theme = wWeather.nextweathertheme(data->props, theme);
    }
  }
  else {
    data->themeduration++;
  }

  if( data->theme != NULL &&  data->themeduration > (wWeatherTheme.getduration(data->theme)*10) ) {
    /*data->theme = NULL;
    data->themedim = 0;*/
    data->themetimer1 = 0;
    data->themeshutdown = True;
    TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "deactivate theme [%s]", wWeatherTheme.getid(data->theme) );
  }

  if( data->theme != NULL ) {
    if( data->themestartup && data->themedim < wWeatherTheme.getdim(data->theme) ) {
      float dim = wWeatherTheme.getdim(data->theme);
      dim /= 25.0;
      if( dim < 1 )
        dim = 1;
      data->themedim += (int)dim;
      TraceOp.trc( name, TRCLEVEL_DEBUG, __LINE__, 9999, "***** themedim=%d", data->themedim );
      __doDaylight(weather, hour, min, False, False);
    }
    else if( data->themestartup ) {
      data->themestartup = False;
    }

    if( data->themeshutdown && data->themedim > 0 ) {
      float dim = wWeatherTheme.getdim(data->theme);
      dim /= 25.0;
      if( dim < 1 )
        dim = 1;
      data->themedim -= (int)dim;
      if( data->themedim <= 0) {
        data->themedim = 0;
        data->themeshutdown = False;
        data->theme = NULL;
      }
      TraceOp.trc( name, TRCLEVEL_DEBUG, __LINE__, 9999, "***** themedim=%d", data->themedim );
      __doDaylight(weather, hour, min, False, False);
    }
    else if( data->themeshutdown ) {
      data->themeshutdown = False;
      data->theme = NULL;
    }

    if( data->theme != NULL && !data->themestartup && !data->themeshutdown && data->themetimer1 <= 0 ) {
      /* play it... */
      iOStrTok tok = StrTokOp.inst( wWeatherTheme.getoutputs(data->theme), ',' );
      while( StrTokOp.hasMoreTokens(tok) ) {
        const char* id = StrTokOp.nextToken(tok);
        iOOutput output = ModelOp.getOutput(model, id);
        if( output != NULL ) {
          int i = 0;
          int cnt = rand() % 10;
          for( i = 0; i < cnt; i++ ) {
            iONode cmd = NodeOp.inst( wOutput.name(), NULL, ELEMENT_NODE);
            wOutput.setaddr(cmd, wOutput.getaddr(OutputOp.base.properties(output)));
            wOutput.setvalue(cmd, 255);
            wOutput.setcmd(cmd, wOutput.value);
            OutputOp.cmd(output, cmd, False);
            ThreadOp.sleep(rand()%100);

            cmd = NodeOp.inst( wOutput.name(), NULL, ELEMENT_NODE);
            wOutput.setaddr(cmd, wOutput.getaddr(OutputOp.base.properties(output)));
            wOutput.setvalue(cmd, 0);
            wOutput.setcmd(cmd, wOutput.value);
            OutputOp.cmd(output, cmd, False);
            ThreadOp.sleep(rand()%100);
          }

          iONode cmd = NodeOp.inst( wOutput.name(), NULL, ELEMENT_NODE);
          wOutput.setaddr(cmd, wOutput.getaddr(OutputOp.base.properties(output)));
          wOutput.setvalue(cmd, 255);
          wOutput.setcmd(cmd, wOutput.value);
          OutputOp.cmd(output, cmd, False);

          if( cnt == wWeatherTheme.getsoundrandomrate(data->theme) && StrOp.len(wWeatherTheme.getsound(data->theme)) > 0 ) {
            if( data->themesoundtimer <= 0 ) {
              char* s = StrOp.fmt("%s \"%s%c%s\"", wRocRail.getsoundplayer(AppOp.getIni()),
                  wRocRail.getsoundpath(AppOp.getIni()), SystemOp.getFileSeparator(), wWeatherTheme.getsound(data->theme) );
              SystemOp.system( s, True, False );
              StrOp.free(s);
              data->themesoundtimer = 10;
            }
          }


          cmd = NodeOp.inst( wOutput.name(), NULL, ELEMENT_NODE);
          wOutput.setaddr(cmd, wOutput.getaddr(OutputOp.base.properties(output)));
          wOutput.setvalue(cmd, 0);
          wOutput.setcmd(cmd, wOutput.value);
          OutputOp.cmd(output, cmd, False);
        }
      };
      StrTokOp.base.del(tok);

      data->themetimer1 = rand()%wWeatherTheme.getduration(data->theme);
      data->themesoundtimer--;
    }
    else {
      data->themetimer1--;
      data->themesoundtimer--;
    }

  }

  if( data->themetimerrand <= 0 )
    data->themetimerrand = rand()%600;
  else
    data->themetimerrand--;

}


static void __makeWeather( void* threadinst ) {
  iOThread        th = (iOThread)threadinst;
  iOWeather  weather = (iOWeather)ThreadOp.getParm(th);
  iOWeatherData data = Data(weather);

  iOControl control = AppOp.getControl();
  int lastMin = 0;
  int loopCnt = 10;

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "make weather started..." );
  ThreadOp.sleep(1000);
  __doInitialize(weather, True, True );

  data->themetimerrand = rand()%600;

  while( data->run ) {
    long t = ControlOp.getTime(control);
    struct tm* ltm = localtime( &t );
    int hour = ltm->tm_hour;
    int min  = ltm->tm_min;

    if( loopCnt >= 10 ) {
      loopCnt = 0;

      if( lastMin != ltm->tm_min ) {
        TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "weather time is %02d:%02d", hour, min );
        __doDaylight(weather, hour, min, False, False );
        lastMin = min;
      }
    }
    else {
      loopCnt++;
    }
    __checkWeatherThemes(weather, hour, min );
    ThreadOp.sleep(100);
  }
  __doDaylight(weather, 0, 0, True, False );

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "make weather ended..." );

}

static void _halt( iOWeather inst ) {
  iOWeatherData data = Data(inst);
  data->run = False;
  ThreadOp.sleep(120);
}


static void _setWeather( iOWeather inst, const char* id ) {
  iOWeatherData data = Data(inst);
  iOModel model = AppOp.getModel();

  if( id != NULL ) {
    TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "use weather [%s]", id );
    data->props = ModelOp.getWeather(model, id);
  }
  else {
    TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "reset weather" );
    data->props = NULL;
  }
}


static void _setTheme( iOWeather inst, const char* id ) {
  iOWeatherData data = Data(inst);
  if( data->requestedTheme != NULL )
    StrOp.free(data->requestedTheme);
  data->requestedTheme = StrOp.dup(id);
  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "use weather theme [%s]", id );
}


static Boolean _isWeather( iOWeather inst, const char* id ) {
  iOWeatherData data = Data(inst);
  if( data->props == NULL )
    return False;

  TraceOp.trc( name, TRCLEVEL_INFO, __LINE__, 9999, "is weather [%s] ? [%s]", id, wWeather.getid(data->props) );
  return StrOp.equals(id, wWeather.getid(data->props));
}


/**  */
static struct OWeather* _inst( iONode ini ) {
  iOWeather __Weather = allocMem( sizeof( struct OWeather ) );
  iOWeatherData data = allocMem( sizeof( struct OWeatherData ) );
  MemOp.basecpy( __Weather, &WeatherOp, 0, sizeof( struct OWeather ), data );

  /* Initialize data->xxx members... */
  data->props = ini;
  data->run = True;

  data->makeWeather = ThreadOp.inst( "makeWeather", __makeWeather, __Weather );
  ThreadOp.start( data->makeWeather );

  instCnt++;
  return __Weather;
}


/* ----- DO NOT REMOVE OR EDIT THIS INCLUDE LINE! -----*/
#include "rocrail/impl/weather.fm"
/* ----- DO NOT REMOVE OR EDIT THIS INCLUDE LINE! -----*/
