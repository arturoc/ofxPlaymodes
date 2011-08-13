/*
 * ofxYAFrame.h
 *
 *  Created on: 20-feb-2009
 *      Author: arturo castro
 */

#pragma once

#include "ofxWSlider.h"
#include "ofxWButton.h"
#include "ofxWToggle.h"
#include "ofxWFps.h"
#include "ofxWStyleLoader.h"
#include "ofxXmlSettings.h"

class ofxWFrame: public ofxWidget{
public:

	ofxWFrame(){
		setStyle("default");
		title = "frame";
	}

	void addSaveButton(const string & filename, const string & xml_root){
		ofxWButton * button = new ofxWButton;
		button->init("save to xml",NULL,style);

		button->setPosition(getNextPosition());

		controls.push_back(button);

		this->filename = filename;
		this->xml_root = xml_root;

		ofAddListener(button->intEvent,this,&ofxWFrame::onSave);
	}

	void addLoadButton(const string & filename, const string & xml_root){
		ofxWButton * button = new ofxWButton;
		button->init("load from xml",NULL,style);

		button->setPosition(getNextPosition());

		controls.push_back(button);

		this->filename = filename;
		this->xml_root = xml_root;

		ofAddListener(button->intEvent,this,&ofxWFrame::onLoad);
	}

	void onSave(int & pressed){
		if(pressed){
			saveTo(filename,xml_root);
		}
	}

	void onLoad(int & pressed){
		if(pressed){
			loadFrom(filename,xml_root);
		}
	}

	void saveTo(const string & filename, const string & xml_root){
		ofxXmlSettings xml;
		xml.addTag(xml_root);
		xml.pushTag(xml_root);
		map<string,ofxWidget*>::iterator it;
		for(it=controlsIndex.begin(); it!=controlsIndex.end();it++){
			it->second->saveTo(xml,it->first);
		}
		xml.popTag();
		xml.saveFile(filename);
	}

	void loadFrom(const string & filename, const string & xml_root){
		ofxXmlSettings xml;
		xml.loadFile(filename);
		xml.pushTag(xml_root);
		map<string,ofxWidget*>::iterator it;
		for(it=controlsIndex.begin(); it!=controlsIndex.end();it++){
			it->second->loadFrom(xml,it->first);
		}
		xml.popTag();
	}

	int getValueI(string controlName, int defaultValue=0){
		if(controlsIndex.find(controlName)!=controlsIndex.end()){
			ofxWidget * control = controlsIndex[controlName];
			return control->getValueI();
		}else{
			return defaultValue;
		}
	}

	float getValueF(string controlName, float defaultValue=0){
		if(controlsIndex.find(controlName)!=controlsIndex.end()){
			ofxWidget * control = controlsIndex[controlName];
			return control->getValueF();
		}else{
			return defaultValue;
		}
	}

	bool getValueB(string controlName, bool defaultValue=0){
		if(controlsIndex.find(controlName)!=controlsIndex.end()){
			ofxWidget * control = controlsIndex[controlName];
			return control->getValueB();
		}else{
			return defaultValue;
		}
	}
	void setStyle(const string & _style){
		setDisabledStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
		setEnabledStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
		setFocusedStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
		setPressedStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
		setOutStyle(ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style));
		frameStyle = ofxWStyleLoader::getLoader().getFrameStyle("enabled",_style);
		style=_style;
	}

	virtual void enable(){
		for(unsigned int i=0; i< controls.size(); i++){
			controls[i]->enable();
		}
		ofxWidget::enable();
	}

	virtual void disable(){
		for(unsigned int i=0; i< controls.size(); i++){
			controls[i]->disable();
		}
		ofxWidget::disable();
	}


	virtual void setVisible(bool visible){
		ofxWidget::setVisible(visible);
		for(unsigned int i=0; i< controls.size(); i++){
			controls[i]->setVisible(visible);
		}
	}

	virtual void render(ofxWidgetsStyle & style){
		ofPushMatrix();

		ofTranslate(frameStyle.position.x,frameStyle.position.y);
		ofSetRectMode(OF_RECTMODE_CORNER);

		//background
		/*ofFill();
		ofEnableAlphaBlending();
		ofSetColor(frameStyle.background.color.r,frameStyle.background.color.g,frameStyle.background.color.b, frameStyle.background.color.a);
		float backgroundX= (frameStyle.border.width-40);
		float backgroundY= (frameStyle.border.height-40);
		ofRect(backgroundX,backgroundY,frameStyle.background.width,frameStyle.background.height);
		ofDisableAlphaBlending();*/

		// border
		ofNoFill();
		ofSetColor(frameStyle.border.color.r,frameStyle.border.color.g,frameStyle.border.color.b);
		ofSetLineWidth(frameStyle.border.lineWidth);
		ofRect(0,20,frameStyle.border.width,frameStyle.border.height);

		// menu
		ofRect(0,0,frameStyle.border.width,20);
		//ofDrawBitmapString(toString(state),10,3);

		ofPopMatrix();

		static ofEventArgs voidArgs;
		//if(!isVisible()){
			for(unsigned i=0; i<controls.size(); i++){
				controls[i]->draw(voidArgs);
			}
		//}
	}
	ofRectangle getControlArea(ofxWidgetsStyle & style){
		ofRectangle area(frameStyle.position.x,frameStyle.position.y,frameStyle.width,20);
		return area;
	}

	ofxWidgetsState manageEvent(ofxWidgetsEvent event, ofxWidgetEventArgs & args, ofxWidgetsState currentState){
		static ofPoint prevMousePos;
		ofxWidgetsState nextState = currentState;;

		switch(currentState){
		case OFX_WIDGET_UNFOCUSED:
			if(event == OFX_W_E_POINTER_OVER)
				nextState = OFX_WIDGET_FOCUSED;
		break;

		case OFX_WIDGET_FOCUSED:
			if(event == OFX_W_E_POINTER_PRESSED){
				prevMousePos.x = args.abs_x;
				prevMousePos.y = args.abs_y;
				nextState = OFX_WIDGET_PRESSED;
			}
			if(event == OFX_W_E_POINTER_OUT){
				nextState = OFX_WIDGET_UNFOCUSED;
			}
		break;

		case OFX_WIDGET_OUT:
			if(event == OFX_W_E_POINTER_OVER){
				nextState = OFX_WIDGET_PRESSED;
			}
		case OFX_WIDGET_PRESSED:
			if(event == OFX_W_E_POINTER_OVER || event == OFX_W_E_POINTER_OUT){
				ofPoint posIncrement = ofPoint(args.abs_x,args.abs_y) - prevMousePos;
				frameStyle.position += posIncrement;

				for(unsigned i=0; i<controls.size(); i++){
					controls[i]->setPosition(controls[i]->getControlPosition() + posIncrement);
				}

				prevMousePos.x = args.abs_x;
				prevMousePos.y = args.abs_y;
			}
			if(event == OFX_W_E_POINTER_OUT){
				nextState = OFX_WIDGET_OUT;
			}
			if(event == OFX_W_E_POINTER_RELEASED){
				nextState = OFX_WIDGET_FOCUSED;
			}
		break;

		default:
			nextState = currentState;
		}

		return nextState;
	}

	void update(){
		float borderHeight = 0;
		float maxWidth	 = 0;
		float maxX		 = 0;
		for(unsigned i=0; i<controls.size(); i++){
			float controlX = controls[i]->getControlArea().x;
			float totalWidth = controlX * 2 + controls[i]->getControlArea().width;

			if( controlX > maxX ) maxX = controlX;
			if( totalWidth > maxWidth ) maxWidth = totalWidth;

			borderHeight += frameStyle.vSpacing + controls[i]->getControlArea().height;
		}
		if(controls.size()>0){
			borderHeight += controls[0]->getControlArea().y * 2 + 20;
			frameStyle.border.width = maxWidth;
			frameStyle.border.height = borderHeight;
			frameStyle.width = maxWidth;
			frameStyle.height = borderHeight;
		}

	}

	ofxWSlider & addSlider(const string & title, int * value, int min, int max, string controlName="", const string & _style=""){
		ofxWSlider * slider = new ofxWSlider;
		slider->init(title,value,min,max,_style==""?style:_style);

		slider->setPosition(getNextPosition());

		controls.push_back(slider);

		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=slider;
		return *slider;
	}

	ofxWSlider & addSlider(const string & title, float * value, float min, float max, string controlName="", const string & _style=""){
		ofxWSlider * slider = new ofxWSlider;
		slider->init(title,value,min,max,_style==""?style:_style);

		slider->setPosition(getNextPosition());

		controls.push_back(slider);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=slider;
		return *slider;
	}

	ofxWSlider & addSlider(const string & title, int value, int min, int max, string controlName="", const string & _style=""){
		ofxWSlider * slider = new ofxWSlider;
		slider->init(title,value,min,max,_style==""?style:_style);

		slider->setPosition(getNextPosition());

		controls.push_back(slider);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=slider;
		return *slider;
	}

	ofxWSlider & addSlider(const string & title, float value, float min, float max, string controlName="", const string & _style=""){
		ofxWSlider * slider = new ofxWSlider;
		slider->init(title,value,min,max,_style==""?style:_style);

		slider->setPosition(getNextPosition());

		controls.push_back(slider);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=slider;
		return *slider;
	}

	ofxWButton & addButton(const string & title, int * value, string controlName="", string _style=""){
		ofxWButton * button = new ofxWButton;
		button->init(title,value,_style==""?style:_style);

		button->setPosition(getNextPosition());

		controls.push_back(button);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=button;
		return *button;
	}

	ofxWToggle & addToggle(const string & title, int * value, string controlName="", string _style=""){
		ofxWToggle * toggle = new ofxWToggle;
		toggle->init(title,value,_style==""?style:_style);

		toggle->setPosition(getNextPosition());

		controls.push_back(toggle);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=toggle;
		return *toggle;
	}

	ofxWidgetFps & addFps(string controlName=""){
		ofxWidgetFps * fps = new ofxWidgetFps;

		/*fps->setEnabledStyle(buttonStyle);
		fps->setDisabledStyle(buttonStyle);
		fps->setFocusedStyle(buttonFocusedStyle);
		fps->setPressedStyle(buttonPressedStyle);
		fps->setOutStyle(buttonPressedStyle);*/


		fps->setPosition(getNextPosition());

		controls.push_back(fps);
		if(controlName=="") controlName = "widget" + ofToString((int)controls.size());

		controlsIndex[controlName]=fps;
		return *fps;
	}

	ofPoint getNextPosition(){
		float totalHeight = 20;
		float totalWidth = 0;
		float frameWidth = frameStyle.width!=-1?frameStyle.width:ofGetScreenWidth();
		float frameHeight = frameStyle.height!=-1?frameStyle.height:ofGetScreenHeight();
		float maxControlWidth = 0;
		for(unsigned int i = 0; i<controls.size(); i++){
			float controlWidth=controls[i]->getControlSize().x;
			float controlHeight=controls[i]->getControlSize().y;
			totalHeight += controlHeight;
			totalHeight += frameStyle.vSpacing;
			if(controlWidth>maxControlWidth)
				maxControlWidth=controlWidth;
			if(totalHeight>frameHeight){
				totalHeight=0;
				totalWidth+=maxControlWidth+frameStyle.hSpacing;
				maxControlWidth=0;
			}
		}
		return ofPoint(frameStyle.position.x+totalWidth,frameStyle.position.y+totalHeight);
	}
	int getValueI(){
		return 0;
	}

	float getValueF(){
		return 0;
	}

	bool getValueB(){
		return 0;
	}
protected:
	vector<ofxWidget*> controls;
	map<string, ofxWidget*> controlsIndex;
	string style;
	ofxWFrameStyle		frameStyle;

	string				filename;
	string				xml_root;
};


