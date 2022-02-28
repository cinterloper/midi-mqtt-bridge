#!/usr/bin/env bash
cd ext/rtmidi && bash build.sh && cd ../../
cd ext/paho.mqtt.c-1.3.9 && bash build.sh && cd ../../

rm -rf ./build
mkdir -p build
cd build && cmake .. && make
