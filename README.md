# arduino-scope
simple oscilloscope opengl using arduino as adc input through usb serial

## introduction

Reading about [audio signal acquisition from a headphone jack](http://forum.arduino.cc/index.php?topic=42833.0) I wanted to try to see if suggested experiment works:
- connect headphone through a simple resistors, capacitors [circuit](http://interface.khm.de/index.php/lab/interfaces-advanced/arduino-realtime-audio-processing/) from Android to Arduino
- using [Keuwisoft Dual Channel Function Generator](https://play.google.com/store/apps/details?id=com.keuwl.functiongenerator)
- using Arduino ADC to convert signal to digital, then serial
- using C++ opengl program to display waveform

## keys

- *space* : pause display
- *v/V* : decrease/increase vertical factor
- *h/H* : decrease/increase horizontal factor

## flash arduino

- pre: install [arduino ide](https://www.arduino.cc/en/Main/Software)
- open sketch `arduino-scope/arduino-scope.ino` and flash firmware
  - tools/board ( eg. arduino nano )
  - tools/port
  - sketch upload

## debug and execution

- pre: install [vscode](https://code.visualstudio.com/) and suggested c++ extension asked when open the folder

```
code .
```

- breakpoints works thanks to `gdb` debugger ( `-g` switch to gcc - see Makefile )
- set USB port speed ( eg. `stty -F /dev/ttyUSB1 115200` )
- tune USB port name in code `thReadSerialFn` function ( TODO: args parsing )
- hit F5 to start
