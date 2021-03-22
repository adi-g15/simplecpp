#ifndef _GRAPHICSIM_INCLUDED
#define _GRAPHICSIM_INCLUDED

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <set>
#include <vector>
#include <common_def.h>
#include <cstring>
#include <cmath>
#include <iostream>
#include <unistd.h>

#include <array2d.h>
#include <sprite.h>
#include <line.h>
#include <point.h>
#include <polygon.h>
#include <keycodes.h>
#include <X.h>
#include <turtle.h>
#include <canvas.h>

#define MouseCode(event) ((event).xbutton.button)
#define KeyCode(event) ((event).xkey.keycode)

#define MouseCode_LEFT Button1
#define MouseCode_RIGHT Button3
#define MouseCode_CENTER Button2
#define MouseCode_SCROLLDN Button4
#define MouseCode_SCROLLUP Button5

#define MousePosX(event) ((event).xbutton.x)
#define MousePosY(event) ((event).xbutton.y)

#define MouseTime(event) ((event).xbutton.time)
#define KeyTime(event) ((event).xkey.time)


#ifndef PI
#define PI 3.14159265358979323846
#endif

#define dotimes(n) for(int _i_=0; _i_<n; _i_++)

#endif
