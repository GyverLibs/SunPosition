This is an automatic translation, may be incorrect in some places. See sources and examples!

# SunPosition
Library for determining the position of the sun by geolocation and time
- Takes latitude, longitude and unix time
- Gives the angle in azimuth, elevation and declination of the sun

### Compatibility
Compatible with all Arduino platforms (using Arduino functions)

## Content
- [Install](#install)
- [Initialization](#init)
- [Usage](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found by the name **SunPosition** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download library](https://github.com/GyverLibs/SunPosition/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features
- Through the IDE library manager: find the library as during installation and click "Update"
- Manually: **delete the folder with the old version**, and then put the new one in its place. "Replacement" cannot be done: sometimes new versions delete files that remain after replacement and can lead to errors!


<a id="init"></a>
## Initialization
```cpp
// initialize without calculation
SunPosition pos;

// specify latitude (degrees), longitude (degrees), unix time (seconds)
// calculation will be performed
SunPosition(latitude, longitude, unix);
```

<a id="usage"></a>
## Usage
```cpp
// perform calculation, specify latitude (degrees), longitude (degrees), unix time (seconds)
void compute(float latitude, float longitude, uint32_t unix);
    
// get the height of the sun above the horizon, degrees
altitude float();

// get the angle of the sun in azimuth (from north clockwise), degrees
float azimuth();

// get the angle of the sun (-23.. 23 during the year), degrees
float declaration();
```

- Latitude and longitude of a place can be taken from any maps (Yandex, Google)
- Unix time can be taken from any real time source (RTC, NTP)

![diagram](/doc/angles.png)
- **Declination angle** - the angle from the direction to the sun at the equinox, varies throughout the year. The equinox angle itself is `90 - latitude`
- **Azimuth** - horizontal direction to the sun, calculated from the direction to the North clockwise
- **Elevation** - vertical direction to the sun from the horizon

<a id="example"></a>
## Example
```cpp
#include <SunPosition.h>

void setup() {
  Serial.begin(9600);
  SunPosition sun(55.75, 37.62, 1658325600);
  //sun.compute(55.75, 37.62, 1658325600);
  Serial.println(sun.altitude());
  Serial.println(sun.azimuth());
  Serial.prcranberry intln(sun.declination());
}

void loop() {
}
```

<a id="versions"></a>
## Versions
- v1.0

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!

When reporting bugs or incorrect work of the library, it is necessary to indicate:
- Library version
- What MK is used
- SDK version (for ESP)
- Arduino IDE Version
- Whether built-in examples work correctly, which use functions and constructs that lead to a bug in your code
- What code was loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimal code