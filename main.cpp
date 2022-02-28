#include <iostream>
#include <cstdlib>
#include <signal.h>
#include "RtMidi.h"
#include "paho_mqtt.hpp"
#include "probe.h"
#include <argparse/argparse.hpp>


// Platform-dependent   sleep routines.
#if defined(WIN32)
#include <windows.h>
#define SLEEP( milliseconds ) Sleep( (DWORD) milliseconds )
#else // Unix variants

#include <unistd.h>

#define SLEEP(milliseconds) usleep( (unsigned long) (milliseconds * 1000.0) )
#endif

bool halt;

static void finish(int /*ignore*/) { halt = true; }

void handle_midi_data(RtMidiIn *midiin, bool forward) {
    // check input queue.
    double stamp;
    std::vector<unsigned char> message;
    int nBytes, i;
    stamp = midiin->getMessage(&message);
    nBytes = message.size();
    std::vector<int> numeric_message;
    for (i = 0; i < nBytes; i++) {
        std::cout << "Byte " << i << " = " << (int) message[i] << ", ";
        numeric_message.push_back((int) message[i]);
    }
    std::stringstream result;
    std::copy(numeric_message.begin(), numeric_message.end(), std::ostream_iterator<int>(result, " "));

    if (nBytes > 0) {
        std::cout << "stamp = " << stamp << std::endl;
        if (forward) { phao_publish("midi/test", result.str()); }
    }

}

void midi_loop(bool connect, int device, const std::string &server) {
    RtMidiIn *midiin = 0;


    // RtMidiIn constructor
    try {
        midiin = new RtMidiIn();
        if (connect) {
            int con_res = phao_connect(server);
            if (con_res != 0) {
                exit(con_res);
            }
        }
    }
    catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }


    // Check available ports vs. specified.
    unsigned int port = device;
    unsigned int nPorts = midiin->getPortCount();


    try {
        midiin->openPort(port);
    }
    catch (RtMidiError &error) {
        error.printMessage();
        goto cleanup;
    }

    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes(false, false, false);

    // Install an interrupt handler function.
    halt = false;
    (void) signal(SIGINT, finish);
    while (!halt) {
        handle_midi_data(midiin, connect);
        SLEEP(2);
    }


    // Clean up
    cleanup:
    delete midiin;
}

int main(int argc, char *argv[]) {
    argparse::ArgumentParser program("midi_mqtt");

    program.add_argument("--list_devices")
            .default_value(false)
            .implicit_value(true);

    program.add_argument("--dump_midi")
            .default_value(false)
            .implicit_value(true);

    program.add_argument("--mqtt_server")
            .default_value(std::string(""));

    program.add_argument("--midi_device")
            .default_value(1);
    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    auto list_devices = program.get<bool>("--list_devices");
    if (list_devices) {
        probe_devices();
        return 0;
    }

    auto midi_device = program.get<int>("--midi_device");
    auto dump_midi = program.get<bool>("--dump_midi");
    if (dump_midi) {
        midi_loop(false, midi_device, "");
        return 0;
    }

    auto mqtt_server = program.get<std::string>("--mqtt_server");
    if (!mqtt_server.empty()) {
        midi_loop(true, midi_device, mqtt_server);
    }

    return 0;
}
