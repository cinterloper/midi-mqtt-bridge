cmake_minimum_required(VERSION 3.16)
project(midi_mqtt)
#option(BUILD_SUBDIR1 "Enable building subdir1" ON)
#option(BUILD_SUBDIR2 "Enable building subdir2" ON)
#set(PAHO_MQTT_C_INCLUDE_DIRS ext/paho.mqtt.c-1.3.9/)
#set(PAHO_MQTT_C_LIBRARIES ext/paho.mqtt.c-1.3.9/)

#if(BUILD_SUBDIR1)
#    add_subdirectory(ext/paho.mqtt.c-1.3.9)
#endif()
#if(BUILD_SUBDIR2)
#    add_subdirectory(ext/paho.mqtt.cpp-1.2.0)
#endif()
set(CMAKE_CXX_STANDARD 20)
include_directories(ext/rtmidi/)
include_directories(ext/paho.mqtt.c-1.3.9/src/)
include_directories(ext/argparse-2.2/include)

link_directories(ext/rtmidi/build/)
link_directories(ext/paho.mqtt.c-1.3.9/build/src)
add_executable(midi_probe probe.cpp probe.h)
add_executable(midi_mqtt main.cpp paho_mqtt.hpp)

target_link_libraries(midi_probe librtmidi.a)
target_link_libraries(midi_mqtt librtmidi.a)
target_link_libraries(midi_mqtt libpaho-mqtt3a.a)
target_link_libraries(midi_mqtt libpaho-mqtt3c.a)

find_package(ALSA REQUIRED)
if (ALSA_FOUND)
    include_directories(${ALSA_INCLUDE_DIRS})
    target_link_libraries(midi_mqtt ${ALSA_LIBRARIES})
    target_link_libraries(midi_probe ${ALSA_LIBRARIES})

endif (ALSA_FOUND)
target_link_libraries(midi_mqtt pthread)
target_link_libraries(midi_probe pthread)


