/*
 * ofxYAButton.h
 *
 *  Created on: 19-feb-2009
 *      Author: arturo castro
 */
#pragma once

#include "ofxWTypes.h"
#include "ofxWidget.h"
#include "ofxWStyleLoader.h"
#include "ofMain.h"


class ofxWSlider: public ofxWidget{
public:


	float * ftargetValue;
	int   * itargetValue;

	float min;
	float max;

	ofxWSlider(){
		value 			= 0;
		ftargetValue 	= 0;
		itargetValue 	= 0;
		title			= "";
	}

	void init(const string & _title, float * value, float _min, float _max, const string & style){
		itargetValue	= 0;
		ftargetValue	= value;
		title			= _title;
		min				= _min;
		max				= _max;
		setStyles("slider",style);
	}

	void init(const string & _title, int * value, int _min, int _max, const string & style){
		ftargetValue	= 0;
		itargetValue	= value;
		title			= _title;
		min				= _min;
		max				= _max;
		setStyles("slider",style);

	}
	void init(const string & _title, float _value, float _min, float _max, const string & style){
		itargetValue	= 0;
		ftargetValue	= 0;
		title			= _title;
		min				= _min;
		max				= _max;
		value			= ofMap(_value,min,max,0,1);
		setStyles("slider",style);
	}

	void init(const string & _title, int _value, int _min, int _max, const string & style){
		ftargetValue	= 0;
		itargetValue	= 0;
		title			= _title;
		min				= _min;
		max				= _max;
		value			= ofMap(_value,min,max,0,1);
		setStyles("slider",style);

	}
	int getValueI(){
		return ofMap(value,0,1,min,max);
	}

	float getValueF(){
		return ofMap(value,0,1,min,max);
	}

	bool getValueB(){
		return value;
	}

	ofEvent<int> 	intEvent;
	ofEvent<float>  floatEvent;

protected:

	void update(){
		if(ftargetValue)
			value = ofClamp(ofMap(*ftargetValue,min,max,0,1),0,1);
		if(itargetValue)
			value = ofClamp(ofMap(*itargetValue,min,max,0,1),0,1);
	}

	void setValue(float _value){
		value = ofClamp(_value,0,1);

		float floatValue = ofMap(value,0,1,min,max);
		int intValue 	 = floatValue;

		ofNotifyEvent(intEvent,intValue);
		ofNotifyEvent(floatEvent,floatValue);

		if(ftargetValue)
			*ftargetValue = floatValue;

		if(itargetValue)
			*itargetValue = intValue;
	}

	void render(ofxWidgetsStyle & style){
		ofPushMatrix();
		ofTranslate(style.position.x,style.position.y);

		//background
		ofFill();
		ofSetColor(style.background.color.r,style.background.color.g,style.background.color.b);
		float backgroundX= (style.border.width-style.background.width)/2;
		float backgroundY= (style.border.height-style.background.height)/2;
		ofRect(backgroundX,backgroundY,style.background.width,style.background.height);

		//foreground
		ofSetColor(style.foreground.color.r,style.foreground.color.g,style.foreground.color.b);
		float foregroundX= (style.background.width-style.foreground.width)/2;
		float foregroundY= (style.background.height-style.foreground.height)/2;
		ofRect(foregroundX,foregroundY,style.foreground.width*value,style.foreground.height);

		// border
		ofNoFill();
		ofSetColor(style.border.color.r,style.border.color.g,style.border.color.b);
		ofSetLineWidth(style.border.lineWidth);
		ofRect(0,0,style.border.width,style.border.height);

		char msg[1024];
		if(itargetValue)
			sprintf(msg,"%i",*itargetValue);
		else if(ftargetValue)
			sprintf(msg,"%.2f",*ftargetValue);
		else
			sprintf(msg,"%.2f",ofMap(value,0,1,min,max));
		ofSetColor(style.text.color.r,style.text.color.g,style.text.color.b);
		if(style.text.ttf){
			style.text.font->drawString(title +" "+ msg,style.text.position.x,style.text.position.y);
		}else{
			ofDrawBitmapString(title +" "+ msg,style.text.position.x,style.text.position.y);
		}

		ofPopMatrix();
	}

	ofRectangle getControlArea(ofxWidgetsStyle & style){
		ofRectangle area;
		area.x=style.position.x;
		area.y=style.position.y;
		area.width=style.foreground.width;
		area.height=style.foreground.height;
		return area;
	}


	// UNFOCUSED -> over -> FOCUSED -> pressed -> PRESSED -> out -> OUT -> released -> UNFOCUSED
	// PRESSED -> released -> FOCUSSED
	// FOCUSED -> out ->  UNFOCUSED
	virtual ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){
		switch(event){

		case OFX_W_E_POINTER_OVER:
			if(currentState==OFX_WIDGET_PRESSED){
				setValue(args.relative_x);
			}
			if(currentState==OFX_WIDGET_UNFOCUSED){
				return OFX_WIDGET_FOCUSED;
			}
			if(currentState==OFX_WIDGET_OUT){
				return OFX_WIDGET_PRESSED;
			}
			break;

		case OFX_W_E_POINTER_OUT:
			if(currentState==OFX_WIDGET_OUT){
				setValue(args.relative_x);
			}
			if(currentState==OFX_WIDGET_FOCUSED){
				return OFX_WIDGET_UNFOCUSED;
			}
			if(currentState==OFX_WIDGET_PRESSED){
				return OFX_WIDGET_OUT;
			}
			break;

		case OFX_W_E_POINTER_PRESSED:
			if(currentState==OFX_WIDGET_FOCUSED){
				setValue(args.relative_x);
				return OFX_WIDGET_PRESSED;
			}
			break;
		case OFX_W_E_POINTER_RELEASED:
			if(currentState==OFX_WIDGET_PRESSED){
				return OFX_WIDGET_FOCUSED;
			}
			if(currentState==OFX_WIDGET_OUT){
				return OFX_WIDGET_UNFOCUSED;
			}
			break;
		case OFX_W_E_ENABLED:
		case OFX_W_E_DISABLED:
		case OFX_W_E_VALUE_CHANGED:
		case OFX_W_E_POINTER_MOVED:
		case OFX_W_E_POINTER_DRAGGED:
		case OFX_W_E_KEY_PRESSED:
		case OFX_W_E_KEY_RELEASED:
		break;
		}

		return currentState;
	}

	void saveTo(ofxXmlSettings & xml, const string & tag){
		if(ftargetValue) xml.addValue(tag,*ftargetValue);
		else if(itargetValue) xml.addValue(tag, *itargetValue);
		else xml.addValue(tag,ofMap(value,0,1,min,max));
	}

	void loadFrom(ofxXmlSettings & xml, const string & tag){
		value = ofMap(xml.getValue(tag,min),min,max,0,1);
		if(ftargetValue) *ftargetValue = xml.getValue(tag,*ftargetValue);
		else if(itargetValue) *itargetValue = xml.getValue(tag,*itargetValue);
	}

private:

	float  value;
};

