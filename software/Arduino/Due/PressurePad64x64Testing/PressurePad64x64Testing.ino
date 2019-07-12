/**
 * Copyright (C) 2019 Jude Brauer. All rights reserved.
 *
 * PressurePad64x64Testing.ino
 * This program will print the data in a human readable format. Use this program for debugging the pressure pad.
 * PressurePas64x64 is faster than this program, however the data is not output in a human readable format
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
 * The values are stored as 8 bit integers in the range 0-63
 */
uint8_t values[64][64];

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
void setPin(uint8_t outputPin) {
    digitalWrite(2, controlPins[outputPin][0]);
    digitalWrite(3, controlPins[outputPin][1]);
    digitalWrite(4, controlPins[outputPin][2]);
    digitalWrite(5, controlPins[outputPin][3]);
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
            // All of the values will be scaled to be in the range 0-63
            values[i][j] = map(adc.readADC(0), 0, 1023, 0, 63);
            values[i][j+16] = map(adc.readADC(1), 0, 1023, 0, 63);
            values[i][j+32] = map(adc.readADC(2), 0, 1023, 0, 63);
            values[i][j+48] = map(adc.readADC(3), 0, 1023, 0, 63);

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
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            Serial.print((char) (values[i][j] + 32));
        }
    }
    Serial.println();
}

/**
 * Send the data over serial in a human readable format
 */
void sendFrameHumanReadable() {
    for (int i = 0; i < 64; i++) {
        Serial.print("{");

        for (int j = 0; j < 64; j++) {
            Serial.print(values[i][j]);
            Serial.print(", ");
        }

        Serial.println("}");
    }
}

void setup() {
    Serial.begin(115200);

    // Enable the MCP23017s
    expanders[0].begin(0x20);
    expanders[1].begin(0x21);
    expanders[2].begin(0x22);
    expanders[3].begin(0x23);

    // Enable the ADC
    adc.begin();

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
}

void loop() {
    Serial.println("----------------------------------------------------------------------------------------------------");

    // Read the data
    readData();

    // Send the data
    //sendFrame();

    // If you the data to be printed in a human readable format, uncomment this line and comment the previous one out
    sendFrameHumanReadable();

    // Delay between reading each frame
    delay(3000);
}
