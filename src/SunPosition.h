/*
    Библиотека для определения положения солнца по геолокации и времени
    Документация:
    GitHub: https://github.com/GyverLibs/SunPosition
    Возможности:
    - Берёт широту, долготу и unix время
    - Выдаёт угол по азимуту, угол возвышения и склонения солнца
    
    Основано на https://forum.arduino.cc/t/sun-position-calculator-im-looking-for-good-code-to-calculate-values/244220
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License
*/

#ifndef _SunPosition_h
#define _SunPosition_h
#include <Arduino.h>

struct SunPosition {
    // просто создать
    SunPosition() {}
    
    // указать широту (градусы), долготу (градусы), unix время (секунды), будет выполнен расчёт
    SunPosition(float latitude, float longitude, uint32_t unix) {
        compute(latitude, longitude, unix);
    }
    
    // выполнить расчёт, указать широту (градусы), долготу (градусы), unix время (секунды)
    void compute(float latitude, float longitude, uint32_t unix) {
        float elpsJulian = (unix / 86400.0 + 2440587.5) - 2451545.0;
        float Omega = 2.1429 - 0.0010394594 * elpsJulian;
        float MeanAnomaly = 0.0172019699 * elpsJulian + 6.2400600;
        float EcliptLong = 4.8950630 + 0.017202791698 * elpsJulian + 0.03341607 * sin(MeanAnomaly);
        EcliptLong += 0.00034894 * sin(2 * MeanAnomaly) - 0.0000203 * sin(Omega) - 0.0001134;
        float EcliptObliq = 0.4090928 - 6.2140e-9 * elpsJulian + 0.0000396 * cos(Omega);
        float RightAscension = atan2(cos(EcliptObliq) * sin(EcliptLong), cos(EcliptLong));
        if (RightAscension < 0.0) RightAscension += TWO_PI;
        float Declin = asin(sin(EcliptObliq) * sin(EcliptLong));
        float decHours = (unix % 86400) / 3600.0;
        float GreenvT = 6.6974243242 + 0.0657098283 * elpsJulian + decHours;
        float HourAngle = (GreenvT * 15 + longitude) * DEG_TO_RAD - RightAscension;
        latitude *= DEG_TO_RAD;
        float Zenith = acos(cos(latitude) * cos(HourAngle) * cos(Declin) + sin(Declin) * sin(latitude));
        float Azimuth = atan2(-sin(HourAngle), tan(Declin) * cos(latitude) - sin(latitude) * cos(HourAngle));
        if (Azimuth < 0.0) Azimuth = Azimuth + TWO_PI;
        float Parallax = (6371.01 / 149597890) * sin(Zenith);  // earth rad / astro unit, km
        azm = Azimuth / DEG_TO_RAD;
        alt = 90.0 - (Zenith + Parallax) / DEG_TO_RAD;
        decl = Declin / DEG_TO_RAD;
    }

    // получить высоту солнца над горизонтом, градусы
    float altitude() {
        return alt;
    }
    
    // получить угол солнца по азимуту (от направления на север по часовой стрелке), градусы
    float azimuth() {
        return azm;
    }
    
    // получить угол склонения солнца (-23.. 23 в течение года), градусы
    float declination() {
        return decl;
    }

    float alt = 0;
    float azm = 0;
    float decl = 0;
};
#endif
