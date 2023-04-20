#include "ofxModuleWebcam.h"

namespace ofxModule {
    //------------------------------------------------------------------
	ofxModuleWebcam::ofxModuleWebcam(string moduleName, string settingsPath):ModuleRunnable("Webcam", moduleName,settingsPath) {
		
		width = settings["width"] != nullptr ? settings["width"].get<int>() : 320;
		height = settings["height"] != nullptr ? settings["height"].get<int>()  : 240;
		int framerate = settings["framerate"] != nullptr ? settings["framerate"].get<int>()  : 30;
		int deviceId = settings["deviceId"] != nullptr ? settings["deviceId"].get<int>()  : 0;
		string deviceName = settings["deviceName"] != nullptr ? settings["deviceName"] : "";
		streamingMethod = settings["streamingMethod"] != nullptr && settings["streamingMethod"].get<string>() == "" ? "texture" : settings["streamingMethod"].get<string>();
		isMirrorX = settings["mirrorX"] != nullptr ? settings["mirrorX"].get<bool>()  : false;
		isMirrorY = settings["mirrorY"] != nullptr ? settings["mirrorY"].get<bool>() : false;

		auto li = grabber.listDevices();
		if (deviceName != "") {
			for (auto& device : li) {
				if (device.deviceName == deviceName) deviceId = device.id;
			}
		}

		grabber.setDeviceID(deviceId);
		grabber.setDesiredFrameRate(framerate);
		grabber.setup(width, height);

		if (streamingMethod == "texture" || streamingMethod == "both") {
			texPtr = shared_ptr<ofTexture>(&grabberTexture);
		}  
		if (streamingMethod == "pixels" || streamingMethod == "both") {
			pixPtr = shared_ptr<ofPixels>(&grabberPix);
		}

		if (isMirrorX || isMirrorY) {
			mirrorFbo.allocate(width, height);
		}

		if (settings["rotate"] == 90 || settings["rotate"] == 270) {
			rotateFbo.allocate(height, width);
		}
		
    }
    
    //------------------------------------------------------------------
    void ofxModuleWebcam::update() {
		grabber.update();
		if (grabber.isFrameNew()){

			if (streamingMethod == "texture" || streamingMethod == "both") {
				// update the texture
				grabberTexture = grabber.getTexture();
				if (isMirrorX || isMirrorY) {
					mirrorFbo.begin();
					ofPushMatrix();
					float sx = isMirrorX ? -1 : 1;
					float sy = isMirrorY ? -1 : 1;
					ofTranslate(mirrorFbo.getWidth()* isMirrorX, mirrorFbo.getHeight()* isMirrorY);
					ofPushMatrix();
					ofScale(sx, sy, 1);
					grabberTexture.draw(0, 0);
					ofPopMatrix();
					ofPopMatrix();
					mirrorFbo.end();
					grabberTexture = mirrorFbo.getTexture();
				}

				if (settings["rotate"] == 90 || settings["rotate"] == 270) {
					rotateFbo.begin();
					ofPushMatrix();
					if (settings["rotate"] == 90) {
						ofTranslate(height, 0);
						ofPushMatrix();
						ofRotate(90);
						grabberTexture.draw(0, 0);
						ofPopMatrix();
					} else {
						ofTranslate(0, width);
						ofPushMatrix();
						ofRotate(270);
						grabberTexture.draw(0, 0);
						ofPopMatrix();
					}
					ofPopMatrix();
					rotateFbo.end();
					grabberTexture = rotateFbo.getTexture();
				}

				// send a new camera image
				notifyEvent(texPtr);
			}
			if (streamingMethod == "pixels" || streamingMethod == "both") {
				// update the image
				grabberPix = grabber.getPixels();
				if (isMirrorX || isMirrorY) {
					grabberPix.mirror(isMirrorY, isMirrorX);
				}
				if (settings["rotate"] == 90) {
					grabberPix.rotate90(1);
				} else if (settings["rotate"] == 180) {
					grabberPix.rotate90(2);
				} else if (settings["rotate"] == 270) {
						grabberPix.rotate90(3);
				}

				// send a new camera image
				notifyEvent(pixPtr);
			}
			
		}
    }
    
    //------------------------------------------------------------------
    void ofxModuleWebcam::stopModule() {
        //what happens when the module will be stopped
    }

	// received module events will be processed here
	void ofxModuleWebcam::proceedModuleEvent(ModuleEvent & e)
	{

	}
}
