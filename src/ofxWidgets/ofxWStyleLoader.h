/*
 * ofxStyleLoader.h
 *
 *  Created on: 23-feb-2009
 *      Author: arturo castro
 */

#pragma once

#include "ofxXmlSettings.h"

class ofxWStyleLoader{
public:

	static ofxWStyleLoader & getLoader(){
		static ofxWStyleLoader instance;
		return instance;
	}

	ofxWidgetsStyle getStyle(string controlType, string state, string style="default"){
		ofxWidgetsStyle controlStyle;
		controlStyle.background.color.r=xmlStyles.getValue(style+":"+controlType+":"+state+":background:color:r",255);
		controlStyle.background.color.g=xmlStyles.getValue(style+":"+controlType+":"+state+":background:color:g",255);
		controlStyle.background.color.b=xmlStyles.getValue(style+":"+controlType+":"+state+":background:color:b",255);
		controlStyle.background.width=xmlStyles.getValue(style+":"+controlType+":"+state+":background:width",200);
		controlStyle.background.height=xmlStyles.getValue(style+":"+controlType+":"+state+":background:height",20);
		controlStyle.background.lineWidth=xmlStyles.getValue(style+":"+controlType+":"+state+":background:lineWidth",0);


		controlStyle.foreground.color.r=xmlStyles.getValue(style+":"+controlType+":"+state+":foreground:color:r",255);
		controlStyle.foreground.color.g=xmlStyles.getValue(style+":"+controlType+":"+state+":foreground:color:g",255);
		controlStyle.foreground.color.b=xmlStyles.getValue(style+":"+controlType+":"+state+":foreground:color:b",255);
		controlStyle.foreground.width=xmlStyles.getValue(style+":"+controlType+":"+state+":foreground:width",198);
		controlStyle.foreground.height=xmlStyles.getValue(style+":"+controlType+":"+state+":foreground:height",18);
		controlStyle.foreground.lineWidth=xmlStyles.getValue(style+":"+controlType+":"+state+":foreground:lineWidth",0);

		controlStyle.border.color.r=xmlStyles.getValue(style+":"+controlType+":"+state+":border:color:r",0);
		controlStyle.border.color.g=xmlStyles.getValue(style+":"+controlType+":"+state+":border:color:g",0);
		controlStyle.border.color.b=xmlStyles.getValue(style+":"+controlType+":"+state+":border:color:b",0);
		controlStyle.border.width=xmlStyles.getValue(style+":"+controlType+":"+state+":border:width",200);
		controlStyle.border.height=xmlStyles.getValue(style+":"+controlType+":"+state+":border:height",20);
		controlStyle.border.lineWidth=xmlStyles.getValue(style+":"+controlType+":"+state+":border:lineWidth",2);

		controlStyle.position.x=xmlStyles.getValue(style+":"+controlType+":"+state+":position:x",30);
		controlStyle.position.y=xmlStyles.getValue(style+":"+controlType+":"+state+":position:y",30);

		controlStyle.text.ttf=true;//xmlStyles.getValue(style+":"+controlType+":"+state+":text:ttf",true);
		if(controlStyle.text.ttf){
			controlStyle.text.font = new ofTrueTypeFont;
			controlStyle.text.font->loadFont(xmlStyles.getValue(style+":"+controlType+":"+state+":text:font","verdana.ttf"),8);
		}
		controlStyle.text.color.r=xmlStyles.getValue(style+":"+controlType+":"+state+":text:color:r",0);
		controlStyle.text.color.g=xmlStyles.getValue(style+":"+controlType+":"+state+":text:color:g",0);
		controlStyle.text.color.b=xmlStyles.getValue(style+":"+controlType+":"+state+":text:color:b",0);
		controlStyle.text.position.x=xmlStyles.getValue(style+":"+controlType+":"+state+":text:position:x",24);
		controlStyle.text.position.y=xmlStyles.getValue(style+":"+controlType+":"+state+":text:position:y",14);

		return controlStyle;
	}

	ofxWFrameStyle getFrameStyle(string state, string style="default"){
		ofxWFrameStyle frameStyle = (ofxWFrameStyle)getStyle("frame",state,style);
		frameStyle.width		= xmlStyles.getValue(style+":frame:"+state+":width",200);
		frameStyle.height		= xmlStyles.getValue(style+":frame:"+state+":height",200);
		frameStyle.hSpacing		= xmlStyles.getValue(style+":frame:"+state+":hSpacing",20);
		frameStyle.vSpacing		= xmlStyles.getValue(style+":frame:"+state+":vSpacing",10);
		return frameStyle;
	}

private:
	ofxXmlSettings xmlStyles;

	ofxWStyleLoader(){
		xmlStyles.loadFile("default.xml");
		xmlStyles.setVerbose(true);
	}

};
