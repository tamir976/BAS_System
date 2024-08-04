#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdexcept>
#include <cstring>

int setupSerialPort(const char *device, int baud) {
    int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        throw std::runtime_error("Failed to open the serial port");
    }

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, baud);
    cfsetospeed(&options, baud);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    tcsetattr(fd, TCSANOW, &options);

    return fd;
}

void sendCommand(int fd, const uint8_t *command, size_t length, uint8_t *response, size_t responseLength) {
    write(fd, command, length);
    usleep(100000); // Increased wait time
    read(fd, response, responseLength);
}

uint16_t calculateChecksum(const uint8_t *packet, size_t length) {
    uint16_t sum = 0;
    for (size_t i = 0; i < length; i++) {
        sum += packet[i];
    }
    return sum;
}

void enrollFingerprint(int fd) {
    uint8_t response[12];

    uint8_t readImageCommand[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x01, 0x00, 0x05};
    uint8_t convertImageCommand[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x02, 0x01, 0x00, 0x08};

    std::cout << "Waiting for finger..." << std::endl;

    // Read the first image
    while (true) {
        sendCommand(fd, readImageCommand, sizeof(readImageCommand), response, sizeof(response));
        std::cout << "Read Image Response Code: " << std::hex << static_cast<int>(response[9]) << std::endl;
        if (response[9] == 0x00) {
            break; // Exit loop if the image was successfully captured
        }
        usleep(500000); // Wait 500 ms before trying again
    }

    sendCommand(fd, convertImageCommand, sizeof(convertImageCommand), response, sizeof(response));
    std::cout << "Convert Image Response Code: " << std::hex << static_cast<int>(response[9]) << std::endl;
    if (response[9] != 0x00) {
        std::cerr << "Error: Could not convert image. Error code: " << std::hex << static_cast<int>(response[9]) << std::endl;
        return;
    }

    std::cout << "Remove finger..." << std::endl;
    sleep(2);

    std::cout << "Waiting for the same finger again..." << std::endl;

    while (true) {
        sendCommand(fd, readImageCommand, sizeof(readImageCommand), response, sizeof(response));
        std::cout << "Read Image Response Code (Second Time): " << std::hex << static_cast<int>(response[9]) << std::endl;
        if (response[9] == 0x00) {
            break; // Exit loop if the image was successfully captured
        }
        usleep(500000); // Wait 500 ms before trying again
    }

    convertImageCommand[10] = 0x02;
    convertImageCommand[12] = calculateChecksum(convertImageCommand + 6, 6);
    sendCommand(fd, convertImageCommand, sizeof(convertImageCommand), response, sizeof(response));
    std::cout << "Convert Image Response Code (Second Time): " << std::hex << static_cast<int>(response[9]) << std::endl;
    if (response[9] != 0x00) {
        std::cerr << "Error: Could not convert image. Error code: " << std::hex << static_cast<int>(response[9]) << std::endl;
        return;
    }

    uint8_t createTemplateCommand[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x05, 0x00, 0x09};
    sendCommand(fd, createTemplateCommand, sizeof(createTemplateCommand), response, sizeof(response));
    std::cout << "Create Template Response Code: " << std::hex << static_cast<int>(response[9]) << std::endl;
    if (response[9] != 0x00) {
        std::cerr << "Error: Could not create template. Error code: " << std::hex << static_cast<int>(response[9]) << std::endl;
        return;
    }

    uint8_t storeTemplateCommand[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x06, 0x06, 0x01, 0x00, 0x00, 0x00, 0x0E};
    sendCommand(fd, storeTemplateCommand, sizeof(storeTemplateCommand), response, sizeof(response));
    std::cout << "Store Template Response Code: " << std::hex << static_cast<int>(response[9]) << std::endl;
    if (response[9] == 0x00) {
        std::cout << "Fingerprint enrolled successfully!" << std::endl;
    } else {
        std::cerr << "Error: Could not store template. Error code: " << std::hex << static_cast<int>(response[9]) << std::endl;
    }
}

int main() {
    try {
        int fd = setupSerialPort("/dev/ttyAMA0", B57600);
        enrollFingerprint(fd);
        close(fd);
    } catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }

    return 0;
}
