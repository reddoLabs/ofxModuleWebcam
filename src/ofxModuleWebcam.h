#ifndef _ofxModuleWebcam
#define _ofxModuleWebcam

#include "ofMain.h"
#include "ModuleRunnable.h"


namespace ofxModule {

	/// \brief captures images from webcam
	class ofxModuleWebcam : public ModuleRunnable {

	public:

		ofxModuleWebcam(string moduleName = "Webcam", string settingsPath = "settings.json");
		void update();
		void stopModule();

	protected:
		void proceedModuleEvent(ModuleEvent& e);

	private:

		string streamingMethod;

		int width;
		int height;
		int framerate;
		int deviceId;
		string deviceName;
		bool isMirrorX;
		bool isMirrorY;

		ofVideoGrabber grabber;
		ofFbo mirrorFbo;
		ofFbo rotateFbo;

		ofTexture grabberTexture;
		shared_ptr<ofTexture> texPtr;

		ofPixels grabberPix;
		shared_ptr<ofPixels> pixPtr;
	};
}
#endif
