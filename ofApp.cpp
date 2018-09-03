#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	this->number_of_targets = 21;
	for (int i = 0; i < this->number_of_targets; i++) {

		this->targets.push_back(glm::vec4(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()), 0.0, 0.0));
		glm::vec4  color = glm::vec4(1.0, 1.0, 1.0, 0.0);
		int color_type = i % 3;
		switch (color_type) {
		case 0:
			color = glm::vec4(1.0, 0.0, 0.0, 0.0);
			break;
		case 1:
			color = glm::vec4(0.0, 1.0, 0.0, 0.0);
			break;
		case 2:
			color = glm::vec4(0.0, 0.0, 1.0, 0.0);
			break;
		}
		this->colors.push_back(color);
	}

	this->src.allocate(ofGetWidth(), ofGetHeight());
	this->dst.allocate(ofGetWidth(), ofGetHeight());

	this->shader.load("shader/shader.vert", "shader/shader.frag");
}
//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(this->number_of_targets);

	glm::vec4 rgb_point;
	for (int i = 0; i < this->number_of_targets; i++) {
		
		if (i % 3 == 0) {

			rgb_point = glm::vec4(ofNoise(ofRandom(this->number_of_targets), ofGetFrameNum() * 0.005) * ofGetWidth(), ofNoise(ofRandom(this->number_of_targets), ofGetFrameNum() * 0.005) * ofGetHeight(), 0.0, 0.0);
		}

		this->targets[i] = rgb_point + glm::vec4(ofMap(ofNoise(ofRandom(this->number_of_targets), ofGetFrameNum() * 0.01), 0, 1, -30, 30), ofMap(ofNoise(ofRandom(this->number_of_targets), ofGetFrameNum() * 0.01), 0, 1, -30, 30), 0.0, 0.0);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->dst.begin();
	ofClear(0);

	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform4fv("targets", &this->targets[0].x, this->number_of_targets);
	this->shader.setUniform4fv("colors", &this->colors[0].x, this->number_of_targets);
	this->shader.setUniformTexture("tex0", this->src.getTexture(), 1);

	this->src.draw(0, 0);
	this->shader.end();

	this->dst.end();

	ofSetColor(255);
	this->dst.draw(0, 0);

	std::swap(this->src, this->dst);
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}