#include <SunPosition.h>

void setup() {
  Serial.begin(9600);
  SunPosition sun(55.75, 37.62, 1658325600);
  //sun.compute(55.75, 37.62, 1658325600);
  Serial.println(sun.altitude());
  Serial.println(sun.azimuth());
  Serial.println(sun.declination());
}

void loop() {
}