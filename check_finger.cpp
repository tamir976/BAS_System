#include <iostream>
#include <pigpio.h>

#define DETECTION_PIN 0  // GPIO 3 (BCM numbering)

int main() {
    // Initialize the pigpio library
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed." << std::endl;
        return 1;
    }

    // Set the detection pin as an input
    gpioSetMode(DETECTION_PIN, PI_INPUT);
    gpioSetPullUpDown(DETECTION_PIN, PI_PUD_UP);

    while (true) {
        // Read the state of the detection pin
        int fingerprint_detected = gpioRead(DETECTION_PIN);

        if (fingerprint_detected == PI_LOW) {
            std::cout << "Fingerprint detected!" << std::endl;
        } else {
            std::cout << "No fingerprint detected." << std::endl;
        }

        // Sleep for a short period to avoid excessive CPU usage
        time_sleep(0.5);
    }

    // Terminate the pigpio library
    gpioTerminate();

    return 0;
}
