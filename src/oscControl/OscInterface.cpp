/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "OscInterface.h"

OscInterface::OscInterface(vector<VideoHeader*> videoHeaders, vector<VideoRenderer*> videoRenderers, VideoBuffer * videoBuffer){
    receiver.setup(OSC_INTERFACE_PORT);
    this->videoHeaders=videoHeaders;
    this->videoRenderers=videoRenderers;
    this->videoBuffer=videoBuffer;
    ofAddListener(receiver.newOscMessage,this,&OscInterface::newOscMessage);
}

void OscInterface::mapMessage(string property,int *value){
    integers.insert(make_pair(property,value));
}
void OscInterface::mapMessage(string property,float *value){
    floats.insert(make_pair(property,value));
}

void OscInterface::mapMessage(string property,pmIntDelegate* setter){
    intSetters.insert(make_pair(property,setter));
}
void OscInterface::mapMessage(string property,pmFloatDelegate* setter){
    floatSetters.insert(make_pair(property,setter));
}

void OscInterface::newOscMessage(ofxOscMessage & message){
	ofLog(OF_LOG_NOTICE,"new osc in");
    if(message.getAddress().length()>4){
        string property = message.getAddress();
        string command  = property.substr(1,3);
        property = property.substr(4,property.size()-4);
        if(command!="set" && command!="get"){
            vector<string> commands;
            commands = ofSplitString(message.getAddress(),"/");
            if(commands[0]=="l88pH"){
                int header=atoi(commands[1].c_str());
                if(commands[2]=="pos"){
                    float pct = message.getArgAsFloat(0);
                    videoHeaders[header]->setPct(pct);
                }else if(commands[2]=="speed"){
                    videoHeaders[header]->speed=message.getArgAsFloat(0);
                }else if(commands[2]=="gate"){
                    videoRenderers[header]->setDraw((bool)message.getArgAsFloat(0));
                }
            }else if(commands[0]=="l88p"){
                if(commands[1]=="freeze"){
                    if(message.getArgAsFloat(0)==0)
                        videoBuffer->resume();
                    else
                        videoBuffer->stop();
                }
            }



        }else
        if(message.getArgType(0)==OFXOSC_TYPE_INT32){
            if(integers.find(property)!=integers.end()){
                int * var=integers[property];
                if(command=="set"){
                    *var=message.getArgAsInt32(0);
                }
                if(command=="get")
                    cout << property << ": " << *var << "\n";
            }else if(intSetters.find(property)!=intSetters.end()){
                pmIntDelegate * delegate=intSetters[property];
                if(command=="set"){
                    int value=message.getArgAsInt32(0);
                    delegate->notify(this,value);
                }
                /*if(command=="get")
                    cout << property << ": " << *var << "\n";*/
            }
                //TODO: send message with variable value*/

            //cout << command << " " << property << ": " << *var <<"\n";
        }else if(message.getArgType(0)==OFXOSC_TYPE_FLOAT){
            if(floats.find(property)!=floats.end()){
                float * var=floats[property];
                if(command=="set"){
                    *var=message.getArgAsFloat(0);
                }
                if(command=="get")
                    cout << property << ": " << message.getArgAsFloat(0) << "\n";
                    //TODO: send message with variable value

                cout << command << " " << property << ": " << message.getArgAsFloat(0) <<"\n";
            }else if(floatSetters.find(property)!=floatSetters.end()){
                pmFloatDelegate * delegate=floatSetters[property];
                if(command=="set"){
                    float value=message.getArgAsFloat(0);
                    delegate->notify(this,value);
                }
            }
        }
    }
}
