/*
 * AudioFrame.h
 *
 *  Created on: 27-feb-2009
 *      Author: arturo castro
 */

#pragma once

#include "ofTypes.h"
#include "ofTrueTypeFont.h"

#define OF_BITMAP_CHAR_WIDTH 12
#define OF_BITMAP_CHAR_HEIGHT 12

//--------------------------------------------------
enum ofxWidgetsEvent{
	OFX_W_E_POINTER_OVER,
	OFX_W_E_POINTER_OUT,
	OFX_W_E_POINTER_PRESSED,
	OFX_W_E_POINTER_RELEASED,
	OFX_W_E_POINTER_MOVED,
	OFX_W_E_POINTER_DRAGGED,

	OFX_W_E_KEY_PRESSED,
	OFX_W_E_KEY_RELEASED,

	OFX_W_E_VALUE_CHANGED,
	OFX_W_E_ENABLED,
	OFX_W_E_DISABLED
};


//--------------------------------------------------
static string toString(ofxWidgetsEvent event)
{
	string strState;
    if (event == OFX_W_E_POINTER_OVER)
    	strState = "over";
    else if (event == OFX_W_E_POINTER_OUT)
    	strState = "out";
    else if (event == OFX_W_E_POINTER_PRESSED)
    	strState = "pressed";
    else if (event == OFX_W_E_POINTER_RELEASED)
    	strState = "released";
    else if (event == OFX_W_E_POINTER_MOVED)
    	strState = "moved";
    else if (event == OFX_W_E_POINTER_DRAGGED)
    	strState = "dragged";
    else if (event == OFX_W_E_KEY_PRESSED)
    	strState = "pressed";
    else if (event == OFX_W_E_KEY_RELEASED)
    	strState = "released";
    else if (event == OFX_W_E_VALUE_CHANGED)
    	strState = "changed";
    else if (event == OFX_W_E_ENABLED)
    	strState = "enabled";
    else if (event == OFX_W_E_DISABLED)
    	strState = "disabled";

    return strState;
}


//--------------------------------------------------
enum ofxWidgetsState{
	OFX_WIDGET_PRESSED,		// pressed
	OFX_WIDGET_FOCUSED,		// pointer over
	OFX_WIDGET_UNFOCUSED,	// pointer out
	OFX_WIDGET_OUT			// has been pressed but pointer out
};


//--------------------------------------------------
static string toString(ofxWidgetsState state)
{
	string strTarget;
    if (state == OFX_WIDGET_PRESSED)
        strTarget = "PRESSED";
    else if (state == OFX_WIDGET_FOCUSED)
        strTarget = "FOCUSED";
    else if (state == OFX_WIDGET_UNFOCUSED)
        strTarget = "UNFOCUSED";
    else if (state == OFX_WIDGET_OUT)
        strTarget = "OUT";

    return strTarget;
}


//--------------------------------------------------
typedef struct{
	float width;
	float height;
	ofColor color;
	float lineWidth;
}ofxWidgetsDrawStyle;


//--------------------------------------------------
typedef struct{
	bool ttf;
	ofTrueTypeFont * font;
	ofColor color;
	ofPoint position;
}ofxWidgetsTextStyle;


//--------------------------------------------------
typedef struct ofxWidgetsStyle{
	ofPoint			position;
	ofxWidgetsDrawStyle border;
	ofxWidgetsDrawStyle background;
	ofxWidgetsDrawStyle foreground;
	ofxWidgetsTextStyle text;
	string styleClass;
	string styleState;
}ofxWidgetsStyle;


//--------------------------------------------------
typedef struct ofxWFrameStyle: public ofxWidgetsStyle{

	ofxWFrameStyle(ofxWidgetsStyle style){
		position	= style.position;
		border		= style.border;
		background	= style.background;
		foreground	= style.foreground;
		text		= style.text;
		styleClass	= style.styleClass;
		styleState	= style.styleState;
	}

	ofxWFrameStyle(){}

	float width;
	float height;
	float vSpacing;
	float hSpacing;

}ofxYAFrameStyle;


//--------------------------------------------------
class ofxWidgetEventArgs{

public:
	float			relative_x;
	float			relative_y;

	float			abs_x;
	float			abs_y;
	int				button;
	int				key;

};
