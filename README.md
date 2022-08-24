[![arduino-library-badge](https://www.ardu-badge.com/badge/SunPosition.svg?)](https://www.ardu-badge.com/SunPosition)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/SunPosition?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# SunPosition
Библиотека для определения положения солнца по геолокации и времени
- Берёт широту, долготу и unix время
- Выдаёт угол по азимуту, угол возвышения и склонения солнца

> Для определения времени заказа/рассвета используйте библиотеку [TimeLord](https://github.com/probonopd/TimeLord)

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **SunPosition** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/SunPosition/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!


<a id="init"></a>
## Инициализация
```cpp
// инициализировать без расчёта
SunPosition pos;

// указать широту (градусы), долготу (градусы), unix время (секунды), будет выполнен расчёт
SunPosition(latitude, longitude, unix);
SunPosition(latitude, longitude, unix, gmt);    // + часовой пояс в часах или минутах
```

<a id="usage"></a>
## Использование
```cpp
// выполнить расчёт, указать широту (градусы), долготу (градусы), unix время (секунды)
void compute(float latitude, float longitude, uint32_t unix);

// выполнить расчёт, указать широту (градусы), долготу (градусы), unix время (секунды), часовой пояс в часах или минутах
void compute(float latitude, float longitude, uint32_t unix, int16_t gmt);

uint16_t sunrise();     // время рассвета, в минутах от начала дня по локальному времени
uint16_t noon();        // полдень, в минутах от начала дня по локальному времени
uint16_t sunset();      // время заката, в минутах от начала дня по локальному времени
uint16_t daylight();    // длительность светового дня, в минутах
float altitude();       // угол возвышения
float declination();    // угол склонения
float zenith();         // угол зенита в полдень
float azimuth();        // азимут солнца, от севера по часовой стрелке
uint16_t azimuthMin();  // азимут рассвета
uint16_t azimuthMax();  // азимут заката
uint16_t angle180();    // азимут, масштабированный в диапазон 0-180 градусов для поворота солнечной панели (летом 0..180, зимой 50.. 130)
```

- Широту и долготу места можно взять из любых карт (Яндекс, Google)
- Unix время можно взять из любого источника реального времени (RTC, NTP), а также конвертировать из даты и времени при помощи библиотеки [UnixTime](https://github.com/GyverLibs/UnixTime)

![diagram](/doc/angles.png)  
- **Угол склонения** - угол от направления на солнце в точке равноденствия, меняется в течение года. Сам угол равноденствия равен `90 - широта`
- **Азимут** - направление на солнце по горизонтали, считается от направления на Север по часовой стрелке
- **Возвышение** - направление на солнце по вертикали от горизонта

<a id="example"></a>
## Пример
```cpp
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
```

<a id="versions"></a>
## Версии
- v1.0
- v1.1 - полностью переписаны уравнения, добавлен вывод времени и азимута заката/рассвета и другие полезные углы (см. доку)

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код
