/*
 * ofxYAfps.h
 *
 *  Created on: 21-feb-2009
 *      Author: arturo castro
 */

#pragma once

#include "ofxWidget.h"

class ofxWidgetFps: public ofxWidget{
	virtual void render(ofxWidgetsStyle & style){
		ofPushMatrix();
		//ofTranslate(style.position.x,style.position.y);
		ofDrawBitmapString(ofToString(ofGetFrameRate()),style.position.x,style.position.y);
		ofPopMatrix();
	}
	virtual ofRectangle getControlArea(ofxWidgetsStyle & style){
		ofRectangle area;
		area.x=0;
		area.y=0;
		area.width=100;
		area.height=10;
		return area;
	}
	int getValueI(){
		return ofGetFrameRate();
	}

	float getValueF(){
		return ofGetFrameRate();
	}

	bool getValueB(){
		return ofGetFrameRate();
	}
};

