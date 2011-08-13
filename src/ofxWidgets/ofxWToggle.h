/*
 * ofxYAGToggle.h
 *
 *  Created on: 20-feb-2009
 *      Author: arturo castro
 */

#ifndef OFXYAGTOGGLE_H_
#define OFXYAGTOGGLE_H_

#include "ofxWButton.h"

enum ofxWToggleState{
	OFX_YATOGGLE_ON,
	OFX_YATOGGLE_OFF
};
class ofxWToggle: public ofxWButton{
public:
	ofxWToggle(){

	}

	// UNFOCUSED -> over -> FOCUSED -> pressed -> PRESSED -> released -> FOCUSED
	// FOCUSED -> out -> UNFOCUSED
	virtual ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){
		switch(event){

		case OFX_W_E_POINTER_OVER:
			if(currentState==OFX_WIDGET_UNFOCUSED){
				return OFX_WIDGET_FOCUSED;
			}
			break;

		case OFX_W_E_POINTER_OUT:
			if(currentState==OFX_WIDGET_FOCUSED){
				return OFX_WIDGET_UNFOCUSED;
			}
			break;

		case OFX_W_E_POINTER_PRESSED:
			if(currentState==OFX_WIDGET_FOCUSED){
				on();
				return OFX_WIDGET_PRESSED;
			}
			if(currentState==OFX_WIDGET_PRESSED){
				off();
				return OFX_WIDGET_FOCUSED;
			}
			break;

		case OFX_W_E_VALUE_CHANGED:
			if(currentState==OFX_WIDGET_PRESSED){
				if(mouseIn())
					return OFX_WIDGET_FOCUSED;
				else
					return OFX_WIDGET_UNFOCUSED;
			}else{
				return OFX_WIDGET_PRESSED;
			}
			break;

		case OFX_W_E_POINTER_RELEASED:
		case OFX_W_E_POINTER_MOVED:
		case OFX_W_E_POINTER_DRAGGED:
		case OFX_W_E_KEY_PRESSED:
		case OFX_W_E_KEY_RELEASED:
		case OFX_W_E_DISABLED:
		case OFX_W_E_ENABLED:
		break;
		}

		return currentState;
	}


};
#endif /* OFXYAGTOGGLE_H_ */
