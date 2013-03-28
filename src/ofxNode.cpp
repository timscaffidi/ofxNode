//
//  ofxNode.cpp
//  ofxNode
//
//  Created by Timothy Scaffidi on 3/24/13.
//
//

#include "ofxNode.h"

ofxNode::ofxNode() :
ofNode(),
content(NULL),
drawOffset(0,0,0),
color(255,255,255,255),
visible(true) {
    
}

ofxNode::~ofxNode() {
    delete content;
    ofNode::~ofNode();
}

void ofxNode::setContent(ofBaseDraws * content, bool centerContent) {
    this->content = content;
    if(centerContent && content != NULL) {
        drawOffset.x = content->getWidth()*-0.5;
        drawOffset.y = content->getHeight()*-0.5;
    }
}

void ofxNode::customDraw() {
    if(content != NULL) {
        ofPushStyle();
        ofSetColor(color);
        content->draw(drawOffset);
        ofPopStyle();
    }
}


ofxNodeObject::ofxNodeObject() {
    
}

ofxNodeObject::~ofxNodeObject() {
    for (int i=0; i<components.size(); i++) {
        delete components[i];
    }
}

void ofxNodeObject::draw() {
    for (int i=0; i<components.size(); i++) {
        if(components[i]->getVisibility()){
            components[i]->draw();
        }
    }
}

void ofxNodeObject::loadXML(string filename) {
    ofxXmlSettings xml(filename);
    
    loadNode(xml, NULL);
}

void ofxNodeObject::addComponent(ofxNode * component){
    components.push_back(component);
}

void ofxNodeObject::loadNode(ofxXmlSettings &xml, ofxNode * parent){
    int numNodes = xml.getNumTags("node");
    
    
    ofLogVerbose() << "ofxNodeObject::loadNode() numNodes = " << numNodes;
    
    ofxNode * curNode;
    for(int i=0;i<numNodes;i++){
        if(xml.pushTag("node", i)){
            ofVec3f pos, scale(1.0);
            ofVec4f orientation;
            
            if(xml.pushTag("position")){
                pos.x = xml.getValue("x", 0.0);
                pos.y = xml.getValue("y", 0.0);
                pos.z = xml.getValue("z", 0.0);
                xml.popTag();
            }
            
            if(xml.pushTag("orientation")){
                orientation.x = xml.getValue("x", 0.0);
                orientation.y = xml.getValue("y", 0.0);
                orientation.z = xml.getValue("z", 0.0);
                orientation.w = xml.getValue("w", 0.0);
                xml.popTag();
            }
            
            if(xml.pushTag("scale")){
                scale.x = xml.getValue("x", 1.0);
                scale.y = xml.getValue("y", 1.0);
                scale.z = xml.getValue("z", 1.0);
                xml.popTag();
            }
            
            curNode = new ofxNode();
            curNode->setParent(*parent);
            curNode->setPosition(pos);
            curNode->setOrientation(ofQuaternion(orientation));
            curNode->setScale(scale);
            
            if(xml.pushTag("content")){
                curNode->setContent(loadContent(xml));
                xml.popTag();
            }
            
            addComponent(curNode);
            
            if(xml.pushTag("children")){
                loadNode(xml, curNode);
                xml.popTag();
            }
            
            xml.popTag(); //node
        }
    }
}

ofBaseDraws * ofxNodeObject::loadContent(ofxXmlSettings &xml) {
    ofBaseDraws * content = NULL;
    
    string type = xml.getValue("type", "");
    ofLogVerbose() << "ofxNodeObject::loadContent() type = " << type;

    if(type == "texture") {
        ofTexture * tex = new ofTexture();
        string path = xml.getValue("path", "");
        ofLogVerbose() << "ofxNodeObject::loadContent() path = " << path;
        ofLoadImage(*tex, path);
        content = tex;
    }
    
    return content;
}



