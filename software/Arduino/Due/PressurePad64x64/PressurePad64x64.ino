/**
 * Copyright (C) 2019 Jude Brauer. All rights reserved.
 *
 * PressurePad64x64.ino
 * This program stores all the data in a char array and prints in directly to serial.
 * This is currently the fastest version of the code, however if you need human-readable data for debugging, use PressurePas64x64Testing instead.
 */

#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "Adafruit_MCP3008.h"

Adafruit_MCP3008 adc;

/** 
 * Analog multiplexer pin switch order
 * 
 * Each row represents the states of the four multiplexer switch pins that correspond to 16 different ADC inputs
 * The order of the rows might need to be adjusted if the data columns are being read in the wrong order
 */
PROGMEM byte controlPins[][4] = {
    {0, 0, 0, 0},
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 1, 0},
    {1, 0, 1, 0},
    {0, 1, 1, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 1},
    {1, 0, 0, 1},
    {0, 1, 0, 1},
    {1, 1, 0, 1},
    {0, 0, 1, 1},
    {1, 0, 1, 1},
    {0, 1, 1, 1},
    {1, 1, 1, 1},
};

/**
 * The values read from the grid
 * The values are stored as characters in the range ' '(32) - '_'(95)
 */
char values[4097];

Adafruit_MCP23017 expander0;
Adafruit_MCP23017 expander1;
Adafruit_MCP23017 expander2;
Adafruit_MCP23017 expander3;

Adafruit_MCP23017 expanders[4] = {expander0, expander1, expander2, expander3};

/**
 * Enable the specified pin on the analog multiplexer
 *
 * Currently, pins 2, 3, 4, and 5 are used. If you need to change these pins, change the pin numbers here and in setup.
 * 
 * If the digital writes are too slow, we might have to use manual port manipulation similar to the pre-Due versions of the code
 */
void setPin(uint8_t pin) {
    digitalWrite(2, controlPins[pin][0]);
    digitalWrite(3, controlPins[pin][1]);
    digitalWrite(4, controlPins[pin][2]);
    digitalWrite(5, controlPins[pin][3]);
}

/** 
 * Read the pressure pad data
 */
void readData() {
    for (int i = 0; i < 64; i++) {
        // Cycle through each pin on the 4 MCP23017 I2C port expanders
        // As i increases this should automatically run through the pins on the 4 MCP23017s in the correct order
        expanders[i / 16].digitalWrite((i+16) % 16, HIGH);

        // Cycle through multiplexer pins
        for (int j = 0; j < 16; j++) {
            // Enable the correct multiplexer pin
            setPin(i);

            // Read the values into the array
            // All of the values will be scaled, converted to chars, and shifted to avoid the non-printing characters
            values[(i*64) + j] = (char) (map(adc.readADC(0), 0, 1023, 0, 63)+32);
            values[(i*64) + j+16] = (char) (map(adc.readADC(1), 0, 1023, 0, 63)+32);
            values[(i*64) + j+32] = (char) (map(adc.readADC(2), 0, 1023, 0, 63)+32);
            values[(i*64) + j+48] = (char) (map(adc.readADC(3), 0, 1023, 0, 63)+32);

            // Delay between each set of 4 analog reads
            delayMicroseconds(10);
        }

        // Set the row to low after we are done reading all the values
        expanders[i / 16].digitalWrite((i+16) % 16, LOW);
    }
}

/**
 * Send the data over serial
 */
void sendFrame() {
    // Since the data is stored as a char array, we can just print it directly
    Serial.println(values);
}

void setup() {
    Serial.begin(115200);

    // Enable the MCP23017s
    expanders[0].begin(0x20);
    expanders[1].begin(0x21);
    expanders[2].begin(0x22);
    expanders[3].begin(0x23);

    // Enable the ADC
    adc.begin(6);

    // Setup the 4 multiplexer switch pins
    for (int i = 2; i <= 5; i++) {
        pinMode(i, OUTPUT);
    }

    // Setup all the pins on the 4 I2C port expanders as outputs
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 16; j++) {
            expanders[i].pinMode(j, OUTPUT);
        }
    }

    // Null-terminate our data string
    values[4096] = '\0';
}

void loop() {
    // Read the data
    readData();

    // Send the data
    sendFrame();

    // Delay between reading each frame
    delay(3000);
}
