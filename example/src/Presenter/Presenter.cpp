#include "Presenter.h"
#include "GPF.h"

namespace ofxModule {
    
	Presenter::Presenter(string moduleName):ModuleDrawable("Presenter",moduleName){
		
		// register mouse events for interaction
		ofRegisterMouseEvents(this);


		ofBackground(0);
    }
  
    
    
    //------------------------------------------------------------------
    void Presenter::update() {
		// here could run something ;)
    }

	void Presenter::draw()
	{
		// draw camera image from texture
		if (camImage != nullptr) camImage->draw(0, 0, camImage->getWidth(), camImage->getHeight());

		// draw inverse camera image from pixels
		if (pixelImage != nullptr) {
			int nPix = pixelImage->getWidth()*pixelImage->getHeight()*pixelImage->getNumChannels();
			ofPixels pInverse = *pixelImage;
			auto pix = pInverse.getPixels();
			for (size_t i = 0; i < nPix; i++){
				pix[i] = 255 - pix[i];
			}
			ofPixels p;
			
			ofImage img;
			img.setFromPixels(pInverse);
			img.draw(0, 0, img.getWidth(), img.getHeight());
		}

		ofDrawBitmapStringHighlight("stop capturing by holding mouse button", 50, 50);
	}

	void Presenter::mousePressed(ofMouseEventArgs & mouse)
	{
		// stop recording with sending an idle message to the camera
		ofJson msg;
		msg["value"] = true;
		notifyEvent("setIdle",msg);
	}

	void Presenter::mouseReleased(ofMouseEventArgs & mouse)
	{
		// continue recording with sending an unidle message to the camera
		ofJson msg;
		msg["value"] = false;
		notifyEvent("setIdle",msg);
	}
    
    
    //------------------------------------------------------------------
    void Presenter::proceedModuleEvent(ModuleEvent& e) {
		
		// update the cam image 
		if (e.type == ModuleEvent::MODULE_EVENT_TEXTURE) {
			camImage = e.texture;
		}else if (e.type == ModuleEvent::MODULE_EVENT_PIXELS) {
			pixelImage = e.pixels;
		}
    }
    
}
