//These definitions are included only in windows version of code.
#ifndef _BASIC_DEF_INCLUDED_
#define _BASIC_DEF_INCLUDED_

typedef struct{
          short x,y;
      }XPoint;

typedef struct {
	int type;		/* ButtonPress or ButtonRelease */
	int x, y;		/* pointer x, y coordinates in event window */
} XButtonEvent;

typedef struct {
	int type;		/* ButtonPress or ButtonRelease */
	int x, y;		/* pointer x, y coordinates in event window */
} XMotionEvent;

 typedef struct {
	int type;		/* KeyPress or KeyRelease */
	int x, y;		/* pointer x, y coordinates in event window */
}XKeyEvent;

typedef struct _XEvent {
	int type;	/* must not be changed */
	XKeyEvent xkey; //members initialized to zero;
	XButtonEvent xbutton;
	XMotionEvent xmotion;
} XEvent;

#endif
