# theta-remote-release

_This project is work-in-progress._

A simple remote release device for THETA S.
It takes only 3 secs to establish paring.

[![](http://img.youtube.com/vi/hv6fxFxbwg8/0.jpg)](https://www.youtube.com/watch?v=hv6fxFxbwg8)

## Schematics

WIP

## How to Build

This project is using [PlatformIO](http://http://platformio.org/).

1. Connect the ESP8266 to your PC via serial interface.
2. Lanch the bootloader. (Keep IO0 low, and reaset the ESP8266.)
3. Execute below commands.

```
# Install the platformio if you don't have it.
$ pip install -U pip setuptools     
$ pip install -U platformio

# Clone repository
$ git clone https://github.com/shrhdk/theta-remote-release.git

# Build & Upload
$ cd theta-remote-release
$ platformio run --target upload
```

## License

This project is released under the MIT license.

## Thanks

This project uses the following open source softwares.

- [ArduinoJson](https://github.com/bblanchon/ArduinoJson) Copyright (c) 2014-2015 Benoit BLANCHON
