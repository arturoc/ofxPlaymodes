/*
 * ofxYAButton.h
 *
 *  Created on: 19-feb-2009
 *      Author: arturo castro
 */
#pragma once

#include "ofxWTypes.h"
#include "ofxWidget.h"
#include "ofMain.h"
#include "ofxWStyleLoader.h"


class ofxWButton: public ofxWidget{
public:

	int *	targetValue;
	int		value;

	int getValueI(){
		return value;
	}

	float getValueF(){
		return value;
	}

	bool getValueB(){
		return value;
	}

	void init(const string & _title, int * _value, string style="default"){
		targetValue = _value;
		if(_value)
			value	= *_value;
		else
			value	= 0;
		title 		= _title;

		setStyles("button",style);

	}

	ofEvent<int>  intEvent;
	ofEvent<bool> boolEvent;

protected:
	void on(){
		if(targetValue)
			*targetValue=1;
		value = 1;
		bool boolValue = true;
		ofNotifyEvent(intEvent , value);
		ofNotifyEvent(boolEvent, boolValue);
	}
	void off(){
		if(targetValue)
			*targetValue=0;
		value = 0;
		bool boolValue = false;
		ofNotifyEvent(intEvent , value);
		ofNotifyEvent(boolEvent, boolValue);
	}
	void update(){
		if(targetValue && value!=*targetValue){
			value = *targetValue;
			ofxWidgetEventArgs args;
			ofxWidget::newEvent(OFX_W_E_VALUE_CHANGED,args);
		}
	}
	void render(ofxWidgetsStyle & style){
		ofPushMatrix();
		ofTranslate(style.position.x,style.position.y);
		ofSetRectMode(OF_RECTMODE_CORNER);

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
		ofRect(foregroundX,foregroundY,style.foreground.width,style.foreground.height);

		// border
		ofNoFill();
		ofSetColor(style.border.color.r,style.border.color.g,style.border.color.b);
		ofSetLineWidth(style.border.lineWidth);
		ofRect(0,0,style.border.width,style.border.height);

		ofSetColor(style.text.color.r,style.text.color.g,style.text.color.b);
		if(style.text.ttf){
			style.text.font->drawString(title,style.text.position.x,style.text.position.y);
		}else{
			ofDrawBitmapString(title,style.text.position.x,style.text.position.y);
		}

		ofPopMatrix();
	}

	ofRectangle getControlArea(ofxWidgetsStyle & style){
		ofRectangle area;
		area.x=style.position.x;
		area.y=style.position.y;
		area.width=style.border.width;// + OF_BITMAP_CHAR_WIDTH * title.size();
		area.height=MAX(style.border.height, OF_BITMAP_CHAR_HEIGHT);
		return area;
	}

	virtual ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){

		switch(event){

		case OFX_W_E_POINTER_OVER:
			if(currentState==OFX_WIDGET_UNFOCUSED){
				return OFX_WIDGET_FOCUSED;
			}
			if(currentState==OFX_WIDGET_OUT){
				return OFX_WIDGET_PRESSED;
			}
			break;

		case OFX_W_E_POINTER_OUT:
			if(currentState==OFX_WIDGET_FOCUSED){
				return OFX_WIDGET_UNFOCUSED;
			}
			if(currentState==OFX_WIDGET_PRESSED){
				return OFX_WIDGET_OUT;
			}
			break;

		case OFX_W_E_POINTER_PRESSED:
			if(currentState==OFX_WIDGET_FOCUSED){
				on();
				return OFX_WIDGET_PRESSED;
			}
			break;

		case OFX_W_E_POINTER_RELEASED:
			if(currentState==OFX_WIDGET_PRESSED){
				off();
				return OFX_WIDGET_FOCUSED;
			}
			if(currentState==OFX_WIDGET_OUT){
				off();
				return OFX_WIDGET_UNFOCUSED;
			}
			break;

		case OFX_W_E_VALUE_CHANGED:
			if(currentState==OFX_WIDGET_PRESSED){
				return OFX_WIDGET_FOCUSED;
			}else{
				return OFX_WIDGET_PRESSED;
			}
			break;
		case OFX_W_E_DISABLED:
			if(currentState==OFX_WIDGET_PRESSED){
				return OFX_WIDGET_UNFOCUSED;
			}
			break;
		case OFX_W_E_ENABLED:
			if(currentState==OFX_WIDGET_PRESSED){
				if(mouseIn())
					return OFX_WIDGET_FOCUSED;
				else
					return OFX_WIDGET_UNFOCUSED;
			}
			break;
		case OFX_W_E_POINTER_MOVED:
		case OFX_W_E_POINTER_DRAGGED:
		case OFX_W_E_KEY_PRESSED:
		case OFX_W_E_KEY_RELEASED:
		break;
		}

		return currentState;
	}

	void saveTo(ofxXmlSettings & xml, const string & tag){
		xml.addValue(tag,value);
	}

	void loadFrom(ofxXmlSettings & xml, const string & tag){
		value = xml.getValue(tag,0);
	}
};

