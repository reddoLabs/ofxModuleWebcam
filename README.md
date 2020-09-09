# ofxModuleCanon

Module for [ofxModule](https://github.com/reddoLabs/ofxModule) that allows easy control over Webcams.

Compatible with openFrameworks 0.11.0+

Generate all project files using the openFrameworks Project Generator.

## Required Addons

* [ofxModule](https://github.com/reddoLabs/ofxModule)

## Getting started

Have a look at the sample project to get started directly and have a small explanation here.

## Settings

The module can be configured in the settings.json.

```json
{
    "Webcam": {
        "width": 640,
        "height": 480,
        "streamingMethod":"pixels"
    }
}
```

| property      | type          | description  |
| ------------- |---------------| -----|
| mirrorX       | boolean       | mirrors Image on x-axis |
| mirrorY       | boolean       | mirrors Image on y-axis |
| rotate        | int           | rotate the image either 0, 90, 180 or 270|
| streamingMethod | string      | transmit type of images to other modules either "texture","pixels" or "both"|
| framerate      | int           | desired camera framerate |
| deviceId       | int           | the device id (number of detection)|
| deviceName     | string        | the device name |


## Communication

### Control the module

Activate/ deactivate streaming:

```cpp
ofJson j;
j["value"] = false; // activate the module, for deactivate use true
notifyEvent("setIdle", j);
```

### Get messages from the module

Depending of the selected streaming method you receive the cam image as `ofTexture` or `ofPixels`. A photo arrives at the address `photo`, other messages represent the current live view.

```cpp
void Presenter::proceedModuleEvent(ModuleEvent& e) {
		
		// update the cam image 
		if (e.type == ModuleEvent::MODULE_EVENT_TEXTURE) {
		  camTexture = e.texture;
		}else if (e.type == ModuleEvent::MODULE_EVENT_PIXELS) {
			camPic.setFromPixels(*e.pixels);
		}
    }
```