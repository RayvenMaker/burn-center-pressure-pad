/**
 * Copyright (C) 2019 Jude Brauer. All rights reserved.
 *
 * Compile with: arduino-cli compile --fqbn arduino:avr:<board>
 * Run with: arduino-cli ...
 */

#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "Adafruit_MCP3008.h"

#define MUX_IN A0

Adafruit_MCP3008 adc;

// digital out switch pins: 2-5
byte controlPins[] = {
    B00000000,
    B00100000,
    B00010000,
    B00110000,
    B00001000,
    B00101000,
    B00011000,
    B00111000,
    B00000100,
    B00100100,
    B00010100,
    B00110100,
    B00001100,
    B00101100,
    B00011100,
    B00111100,
};

uint16_t values[16][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

Adafruit_MCP23017 mcp0;
//Adafruit_MCP23017 mcp1;
//Adafruit_MCP23017 mcp2;
//Adafruit_MCP23017 mcp3;
//Adafruit_MCP23017 mcp4;
//Adafruit_MCP23017 mcp5;
//Adafruit_MCP23017 mcp6;
//Adafruit_MCP23017 mcp7;
//
//Adafruit_MCP23017 mcps[8] = {mcp0, mcp1, mcp2, mcp3, mcp4, mcp5, mcp6, mcp7};

void setPin(uint8_t outputPin) {
    PORTD = controlPins[outputPin];
}

void readData() {
    // Cycle through MCP pins
    for (int i = 0; i < 16; i++) {
        mcp0.digitalWrite(i, HIGH);

        // Cycle through MUX pins
        for (int j = 0; j < 16; j++) {
            setPin(i);
            //values[i][j] = analogRead(MUX_IN);
            values[i][j] = adc.readADC(0);
            delayMicroseconds(10);
        }

        mcp0.digitalWrite(i, LOW);
    }
}

void setup() {
    //mcps[0].begin(0x20);
    //mcps[1].begin(0x21);
    //mcps[2].begin(0x22);
    //mcps[3].begin(0x23);
    //mcps[4].begin(0x24);
    //mcps[5].begin(0x25);
    //mcps[6].begin(0x26);
    //mcps[7].begin(0x27);

    Serial.begin(2000000);

    adc.begin();

    DDRD = DDRD | B00111100;

    mcp0.begin();

    for (int i = 0; i < 16; i++) {
        mcp0.pinMode(i, OUTPUT);
    }
}

void loop() {
    Serial.println("----------------------------------------------------------------------------------------------------");
    readData();

    for (int i = 0; i < 16; i++) {
        Serial.print("{");

        for (int j = 0; j < 16; j++) {
            Serial.print(values[i][j], DEC);
            Serial.print(", ");
        }

        Serial.println("}");
    }

    delay(3000);
}
