# midi-mqtt-bridge
Forward midi over mqtt
```
$ ./build.sh 
....
Scanning dependencies of target midi_mqtt
[ 25%] Building CXX object CMakeFiles/midi_mqtt.dir/main.cpp.o
[ 50%] Linking CXX executable midi_mqtt
[ 50%] Built target midi_mqtt
Scanning dependencies of target midi_probe
[ 75%] Building CXX object CMakeFiles/midi_probe.dir/probe.cpp.o
[100%] Linking CXX executable midi_probe
[100%] Built target midi_probe
```
```
$ build/midi_mqtt --help
Usage: midi_mqtt [options] 

Optional arguments:
-h --help       shows help message and exits
-v --version    prints version information and exits
--list_devices  [default: false]
--dump_midi     [default: false]
--mqtt_server   [default: ""]
--midi_device   [default: 1]
```
```
$ build/midi_mqtt --list_devices

There are 3 MIDI input sources available.
  Input Port #1: Midi Through:Midi Through Port-0 14:0
  Input Port #2: OhmRGB:OhmRGB Controls 24:0
  Input Port #3: OhmRGB:OhmRGB  24:1

There are 3 MIDI output ports available.
  Output Port #1: Midi Through:Midi Through Port-0 14:0
  Output Port #2: OhmRGB:OhmRGB Controls 24:0
  Output Port #3: OhmRGB:OhmRGB  24:1
```
```
$ build/midi_mqtt --dump_midi
Byte 0 = 144, Byte 1 = 7, Byte 2 = 64, stamp = 0
Byte 0 = 144, Byte 1 = 7, Byte 2 = 0, stamp = 0.106994
...
```
```
$ build/midi_mqtt --mqtt_server 'tcp://localhost:1833'
Byte 0 = 144, Byte 1 = 7, Byte 2 = 64, stamp = 0
Byte 0 = 144, Byte 1 = 7, Byte 2 = 0, stamp = 0.0803047
Byte 0 = 144, Byte 1 = 7, Byte 2 = 64, stamp = 0.107032
Byte 0 = 144, Byte 1 = 7, Byte 2 = 0, stamp = 0.0936893
```
