#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

//
//  ofxNode.h
//  ofxNode
//
//  Created by Timothy Scaffidi on 3/24/13.
//
//

class ofxNode : public ofNode {
public:
    ofxNode();
    ~ofxNode();
    void customDraw();
    void setContent(ofBaseDraws * content, bool centerContent = true);
    ofBaseDraws * getContent() { return content; }
    void setDrawOffset(ofPoint drawOffset) { this->drawOffset = drawOffset; }
    
    void setVisibility(bool v) { visible = v; }
    void setColor(ofColor c) { color = c; }
    
    bool getVisibility() { return visible; }
    ofColor getColor() { return color; }
    
private:
    ofBaseDraws* content;
    ofPoint drawOffset;
    ofColor color;
    bool visible;
};

class ofxNodeObject {
public:
    ofxNodeObject();
    ~ofxNodeObject();
    void loadXML(string filename);
    void addComponent(ofxNode * component);
    vector<ofxNode *> getComponents() { return components; }
    void draw();
    void draw(float x, float y) { ofPushMatrix(); ofTranslate(x, y); draw(); ofPopMatrix(); }
    void draw(ofPoint p) { draw(p.x,p.y); }
private:
    vector<ofxNode *> components;
    void loadNode(ofxXmlSettings &xml, ofxNode * parent);
    ofBaseDraws * loadContent(ofxXmlSettings &xml);
};