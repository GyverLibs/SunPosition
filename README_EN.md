This is an automatic translation, may be incorrect in some places. See sources and examples!

# Sunposition
Library for determining the position of the Sun by geolocation and time
- takes breadth, longitude and unix time
- gives an angle by azimuth, the angle of elevation and declination of the sun

## compatibility
Compatible with all arduino platforms (used arduino functions)

## Content
- [installation] (# Install)
- [initialization] (#init)
- [use] (#usage)
- [Example] (# Example)
- [versions] (#varsions)
- [bugs and feedback] (#fedback)

<a id="install"> </a>
## Installation
- The library can be found by the name ** suposition ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download the library] (https://github.com/gyverlibs/sunposition/archive/refs/heads/main.zip) .Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!


<a id="init"> </a>
## initialization
`` `CPP
// initialize without calculation
SunPounion POS;

// indicate latitude (degrees), longitude (degrees), unix time (seconds), a calculation will be performed
SunPounion (Latitude, Longitude, Unix);
SunPounion (Latitude, Longitude, Unix, GMT);// + hourly belt in hours or minutes
`` `

<a id="usage"> </a>
## Usage
`` `CPP
// perform the calculation, indicate the latitude (degrees), longitude (degrees), unix time (seconds)
Void Compute (Float Latitude, Float Longitude, Uint32_T Unix);

// perform the calculation, specify the latitude (degrees), longitude (degrees), unix time (seconds), an hourly belt in hours or minutes
Void Compute (Float Latitude, Float Longitude, Uint32_T Unix, Int16_T GMT);

int sunrise ();// Dawn time, minutes from the beginning of the day in local time
int noon ();// Half, in minutes from the beginning of the day in local time
int sunset ();// Sunset time, minutes from the beginning of the day in local time
int daylight ();// Duration of daylight hours, in minutes
Float altitude ();// Avoor angle
Float Decline ();// angle of declension
Float Zenith ();// Zenit angle at noon
Float Azimuth ();// Azimuth of the Sun, from the north clockwise
int Azimuthmin ();// Azimuth of Dawn
int Azimuthmax ();// Azimuth of sunset
int angle180 ();// azimuth, scaled in the range 0-180 degrees DCranberries of turning the solar panel (in summer 0..180, in winter 50 .. 130)
int angle90 ();// Azimuth, scaled in the range -90..90 degrees for turning the solar panel (in summer -90..90, in winter -20 .. 20)
`` `

- The latitude and longitude can be taken from any cards (Yandex, Google)
- Unix time can be taken from any real time source (RTC, NTP), and also converted from the date and time using the library [unixtime] (https://github.com/gyverlibs/unixtime)

! [Diagram] (/doc/angles.png)
- ** The angle of declension ** - the angle from the direction to the sun at the point of the equinox, changes during the year.The angle of equinox itself is `90 - LATE`
- ** azimuth ** - a direction in the sun is horizontally, it is considered from the direction to the north clockwise
- ** elevation ** - a direction in the sun vertically from the horizon

<a id="EXAMPLE"> </a>
## Example
`` `CPP
#include <Sunposition.h>

VOID setup () {
  Serial.Begin (9600);
  SunPounion Sun (55.75, 37.62, 1658325600);
  //sun.computE(55.75, 37.62, 1658325600);
  Serial.println (sun.altite ());
  Serial.println (sun.azimuth ());
  Serial.println (sun.Decline ());
}

VOID loop () {
}
`` `

<a id="versions"> </a>
## versions
- V1.0
- V1.1 - the equations are completely rewritten, the output of the time and the azimuth of sunset/dawn and other useful angles (see Doku) have been added

<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!

When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code