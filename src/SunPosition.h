/*
    Библиотека для определения положения солнца по геолокации и времени
    Документация:
    GitHub: https://github.com/GyverLibs/SunPosition
    Возможности:
    - Берёт широту, долготу и unix время
    - Выдаёт угол по азимуту, угол возвышения и склонения солнца
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License
    
    v1.1 - полностью переписаны уравнения, добавлен вывод времени и азимута заката/рассвета и другие полезные углы (см. доку)
*/

/*
    Источники:
    Astronomical Almanac p C2
    https://www.astrogreg.com/snippets/equationoftime-simple.html
    https://www.itacanet.org/the-sun-as-a-source-of-energy/part-3-calculating-solar-angles/
    https://webcms3.cse.unsw.edu.au/ENGG1811/16s1/forums/514357
    https://gml.noaa.gov/grad/solcalc/calcdetails.html
    нужен double! http://users.electromagnetic.net/bu/astro/sunrise-set.php

    для проверки https://www.suncalc.org/
    http://www.geoastro.de/astro/suncalc/index.htm
*/

#ifndef _SunPosition_h
#define _SunPosition_h
#include <Arduino.h>

struct SunPosition {
    // просто создать
    SunPosition() {}

    // указать широту (градусы), долготу (градусы), unix время (секунды), часовой пояс в часах или минутах. Будет выполнен расчёт
    SunPosition(float lat, float lon, uint32_t unix, int16_t gmt = 0) {
        compute(lat, lon, unix, gmt);
    }

    // выполнить расчёт, указать широту (градусы), долготу (градусы), unix время (секунды), часовой пояс в часах или минутах
    void compute(float lat, float lon, uint32_t unix, int16_t gmt = 0) {
        lat = radians(lat);
        float hours = (unix % 86400ul) / 86400.0;       // decimal hours
#ifndef SP_PRECISE
        int day = fmod(unix / 86400.0, 365.25);
        float t = radians(279.342 + 0.985647 * day);    // temp
        float eqTime = (-104.9 * sin(t) + 596.2 * sin(2 * t) + 4.3 * sin(3 * t) - 12.7 * sin(4 * t) - 429.3 * cos(t) - 2.0 * cos(2 * t) + 19.3 * cos(3 * t)) / 60.0;  // time equation
        t = (day + hours / 24 - 1.5) * TWO_PI / 365;    // temp
        decl = 0.006918 - 0.4 * cos(t) + 0.070257 * sin(t) - 0.006758 * cos(t * 2) + 0.000907 * sin(t * 2) - 0.002697 * cos(t * 3) + 0.00148 * sin(t * 3);  // declination
#else
        float JulCent = (unix / 86400.0 - 10957.5) / 36525.0;                                                   // Julian Century
        float GeomMeanLong = radians(fmod(280.46646 + JulCent * (36000.76983 + JulCent * 0.0003032), 360));     // Geom Mean Long Sun
        float GeomMeanAnom = radians(357.52911 + JulCent * (36000 - 0.0001537 * JulCent));                      // Geom Mean Anom Sun
        float EccEart = 0.016708634 - JulCent * (0.000042037 + 0.0000001267 * JulCent);                         // Eccent Earth Orbit
        float SunEqCtr = radians(sin(GeomMeanAnom) * (1.914602 - JulCent * (0.004817 + 0.000014 * JulCent)) + sin(2 * GeomMeanAnom) * (0.019993 - 0.000101 * JulCent) + sin(3 * GeomMeanAnom) * 0.000289); // Sun Eq of Ctr
        float SunApp = GeomMeanLong + SunEqCtr - 0.0001 - 0.00008 * sin(radians(125.04 - 1934.136 * JulCent));  // Sun App Long
        float MeanOblq = 23 + (26 + ((21.448 - JulCent * (46.815 + JulCent * (0.00059 - JulCent * 0.001813)))) / 60.0) / 60.0;  // Mean Obliq Ecliptic
        float ObliqCor = radians(MeanOblq + 0.00256 * cos(radians(125.04 - 1934.136 * JulCent)));               // Obliq Corr
        decl = asin(sin(ObliqCor) * sin(SunApp));    // Sun Declin
        float y = tan(ObliqCor / 2) * tan(ObliqCor / 2);
        float eqTime = 4 * degrees(y * sin(2 * GeomMeanLong) - 2 * EccEart * sin(GeomMeanAnom) + 4 * EccEart * y * sin(GeomMeanAnom) * cos(2 * GeomMeanLong) - 0.5 * y * y * sin(4 * GeomMeanLong) - 1.25 * EccEart * EccEart * sin(2 * GeomMeanAnom));   // Eq of Time (minutes)
#endif
        ha = degrees(acos(-0.01454 / (cos(lat) * cos(decl)) - tan(lat) * tan(decl)));       // HA sunrise
        if (abs(gmt) <= 12) gmt *= 60;                                                      // gmt -> minutes
        noonT = 720 - 4 * lon - eqTime + gmt;                                               // Solar Noon (min) + gmt
        float hrAngl = fmod(hours * 1440 + eqTime + 4 * lon, 1440) / 4;                     // True Solar Time (min)
        hrAngl = hrAngl + (hrAngl < 0 ? 180 : -180);                                        // Hour Angle
        zen = acos(sin(lat) * sin(decl) + cos(lat) * cos(decl) * cos(radians(hrAngl)));     // Zenith
        azm = degrees(acos(((sin(lat) * cos(zen)) - sin(decl)) / (cos(lat) * sin(zen))));   // Azimuth
        decl = degrees(decl);
        alt = 90 - degrees(zen);
        azm = (hrAngl > 0) ? (azm + 180) : (540 - azm);
        azm = fmod(azm, 360);
        zen = 90 - degrees(lat) + decl;
        angle = round(ha);  // 8 * ha * 360 / 60 / 24 / 2
    }

    // время рассвета, в минутах от начала дня по локальному времени
    uint16_t sunrise() {
        return noonT - ha * 4;
    }
    
    // полдень, в минутах от начала дня по локальному времени
    uint16_t noon() {
        return noonT;
    }
    
    // время заката, в минутах от начала дня по локальному времени
    uint16_t sunset() {
        return noonT + ha * 4;
    }
    
    // длительность светового дня, в минутах
    uint16_t daylight() {
        return ha * 8;
    }
    
    // угол возвышения
    float altitude() {
        return alt;
    }
    
    // угол склонения
    float declination() {
        return decl;
    }
    
    // угол зенита
    float zenith() {
        return zen;
    }

    // азимут солнца, от севера по часовой стрелке
    float azimuth() {
        return azm;
    }
    
    // азимут рассвета
    uint16_t azimuthMin() {
        return (180 - round(ha));
    }
    
    // азимут заката
    uint16_t azimuthMax() {
        return (180 + round(ha));
    }
    
    // азимут, масштабированный в диапазон 0-180 градусов для поворота солнечной панели (летом 0..180, зимой 50.. 130)
    uint16_t angle180() {
        uint16_t a = round(ha);
        return map(azm, 180 - a, 180 + a, 90 - min(a, 90), 90 + min(a, 90));
    }

    float alt = 0, azm = 0, decl = 0, zen = 0, ha = 0;
    uint16_t noonT = 0;
};

/*
    // без оптимизации
    float JulDay = unixSecs / 86400.0 + 2440587.5;  // Julian day
    float JulCent = (JulDay - 2451545.0) / 36525.0; // Julian Century
    float GeomMeanLong = fmod(280.46646 + JulCent * (36000.76983 + JulCent * 0.0003032), 360); // Geom Mean Long Sun
    float GeomMeanAnom = 357.52911 + JulCent * (35999.05029 - 0.0001537 * JulCent); // Geom Mean Anom Sun
    float EccEart = 0.016708634 - JulCent * (0.000042037 + 0.0000001267 * JulCent); // Eccent Earth Orbit
    float SunEqCtr = sin(radians(GeomMeanAnom)) * (1.914602 - JulCent * (0.004817 + 0.000014 * JulCent)); // Sun Eq of Ctr
    SunEqCtr += sin(2 * radians(GeomMeanAnom)) * (0.019993 - 0.000101 * JulCent);
    SunEqCtr += sin(3 * radians(GeomMeanAnom)) * 0.000289;
    float SunTrLong = GeomMeanLong + SunEqCtr;  // Sun True Long
    float SunTrAnom = GeomMeanAnom + SunEqCtr;  // Sun True Anom
    float SunRad = (1.000001018 * (1.0 - EccEart * EccEart)) / (1 + EccEart * cos(radians(SunTrAnom))); // Sun Rad Vector (AUs)
    float SunApp = SunTrLong - 0.00569 - 0.00478 * sin(radians(125.04 - 1934.136 * JulCent)); // Sun App Long
    float MeanOblq = 23 + (26 + ((21.448 - JulCent * (46.815 + JulCent * (0.00059 - JulCent * 0.001813)))) / 60.0) / 60.0;  // Mean Obliq Ecliptic
    float ObliqCor = MeanOblq + 0.00256 * cos(radians(125.04 - 1934.136 * JulCent));  // Obliq Corr
    float SunDecl = degrees(asin(sin(radians(ObliqCor)) * sin(radians(SunApp)))); // Sun Declin
    float y = tan(radians(ObliqCor / 2)) * tan(radians(ObliqCor / 2));
    float EqTime = 4 * degrees(y * sin(2 * radians(GeomMeanLong)) - 2 * EccEart * sin(radians(GeomMeanAnom)) + 4 * EccEart * y * sin(radians(GeomMeanAnom)) * cos(2 * radians(GeomMeanLong)) - 0.5 * y * y * sin(4 * radians(GeomMeanLong)) - 1.25 * EccEart * EccEart * sin(2 * radians(GeomMeanAnom)));   // Eq of Time (minutes)
    float HA = degrees(acos(cos(radians(90.833)) / (cos(radians(lat)) * cos(radians(SunDecl))) - tan(radians(lat)) * tan(radians(SunDecl))));  // HA Sunrise
    int GMT = 0;
    float SunN = 720 - 4 * lon - EqTime + GMT * 60; // Solar Noon (LST) + gmt
    float SunR = SunN - HA * 4;  // Sunrise Time (LST) + gmt
    float SunS = SunN + HA * 4;  // Sunset Time (LST) + gmt
    float SunL = 8 * HA;  // Sunlight Duration (minutes) minutes
    float hours = (unixSecs % 86400) / 86400.0;
    float TrueSol = fmod(hours * 1440 + EqTime + 4 * lon, 1440);  // True Solar Time (min)
    float HourAngle = (TrueSol / 4 < 0) ? TrueSol / 4 + 180 : TrueSol / 4 - 180;  // Hour Angle
    float Zenith = degrees(acos(sin(radians(lat)) * sin(radians(SunDecl)) + cos(radians(lat)) * cos(radians(SunDecl)) * cos(radians(HourAngle))));
    float Elev = 90 - Zenith;
    float Azim = degrees(acos(((sin(radians(lat)) * cos(radians(Zenith))) - sin(radians(SunDecl))) / (cos(radians(lat)) * sin(radians(Zenith)))));
    if (HourAngle > 0) Azim = Azim + 180;
    else Azim = 540 - Azim;
    Azim = fmod(Azim, 360);
*/
#endif
