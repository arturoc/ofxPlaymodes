/*
 * ofxYAControlPanel.h
 *
 *  Created on: 21-feb-2009
 *      Author: arturo castro
 */

#ifndef OFXWCONTROLPANEL_H_
#define OFXWCONTROLPANEL_H_

#include "ofxWFrame.h"
class ofxWControlPanel: public ofxWFrame{
public:

	ofxWControlPanel(){
		//addButton("disable all",NULL);
		frameStyle.position=ofPoint(10,10);
		ofxWButton & buttonEnable 	= addButton("enable all",NULL);
		ofxWButton & buttonDisable	= addButton("disable all",NULL);
		ofxWButton & buttonHide 	= addButton("hide",NULL);
		ofAddListener(buttonEnable.boolEvent,this,&ofxWControlPanel::enableAll);
		ofAddListener(buttonDisable.boolEvent,this,&ofxWControlPanel::disableAll);
		ofAddListener(buttonHide.boolEvent,this,&ofxWControlPanel::hide);
		ofAddListener(ofEvents.update,this,&ofxWControlPanel::update);
		setVisible(false);
		disable();
		enabled=false;


	}

	void update(ofEventArgs & args){
	}

	void render(ofxWidgetsStyle & style){
		ofEnableAlphaBlending();
		ofSetColor(10,10,10,80);
		ofRect(0,0,ofGetWidth(),ofGetHeight());
		ofDisableAlphaBlending();
	}

	void registerControl(ofxWidget * control){
		if((dynamic_cast<ofxWControlPanel*>(control))==NULL)
			controls.push_back(control);
	}

	void enableAll(bool & _enable){
		for(unsigned int i=0; i<controls.size(); i++){
			controls[i]->enable();
			controls[i]->setVisible(true);
		}
		disable();
		setVisible(false);
		enable();
		setVisible(true);
	}

	void disableAll(bool & _disable){
		for(unsigned int i=0; i<controls.size(); i++){
			controls[i]->disable();
			controls[i]->setVisible(false);
		}
		enable();
		setVisible(true);
	}
	void hide(bool & enable){
		disable();
		setVisible(false);
	}

	void keyPressed(ofKeyEventArgs & key){
		if(key.key==OF_KEY_MODIFIER)
			modifierActivated=true;
	}

	void keyReleased(ofKeyEventArgs & key){
		if(key.key==OF_KEY_MODIFIER)
			modifierActivated=false;
		else if(key.key=='y'){
			if(enabled){
				setVisible(false);
				disable();
			}else{
				setVisible(true);
				enable();
			}
			enabled=!enabled;
		}
	}

	void disable(){
		ofxWFrame::disable();
		ofAddListener(ofEvents.keyPressed,this,&ofxWControlPanel::keyPressed);
		ofAddListener(ofEvents.keyReleased,this,&ofxWControlPanel::keyReleased);
	}
private:
	vector<ofxWidget*> controls;
	bool modifierActivated;
	bool enabled;

};

//extern ofxYAControlPanel yaControlPanel;
#endif /* OFXYACONTROLPANEL_H_ */
