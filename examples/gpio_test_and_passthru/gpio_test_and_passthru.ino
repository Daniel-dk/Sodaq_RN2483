/*
* Copyright (c) 2015 SODAQ. All rights reserved.
*
* This file is part of Sodaq_RN2483.
*
* Sodaq_RN2483 is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation, either version 3 of
* the License, or(at your option) any later version.
*
* Sodaq_RN2483 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with Sodaq_RN2483.  If not, see
* <http://www.gnu.org/licenses/>.
*/

#include <Sodaq_RN2483.h>

// MBili
//#define debugSerial Serial
// Autonomo
#define debugSerial SerialUSB
#define loraSerial Serial // RN2483 on "Serial" uart 

uint8_t devEUI[8];

// USE YOUR OWN KEYS!
const uint8_t appKey[16] =
{
	0x70, 0xB3, 0xD5, 0x76,
	0xAE, 0x00, 0x00, 0x1F,
	0x70, 0xB3, 0xD5, 0x76,
	0xAE, 0x00, 0x00, 0x1F,
};

// USE YOUR OWN KEYS!
const uint8_t appEUI[8] =
{
	0x70, 0xB3, 0xD5, 0x76,
	0xAE, 0x00, 0x00, 0x1F,
};

uint16_t adcIn, vddIn;
void setup()
{
	debugSerial.begin(57600);
	loraSerial.begin(LoRaBee.getDefaultBaudRate());
	// we have to init the RN**** device with APB or OTA to set up the serial port 
	// OTAA join and uses the hweui pre programmed into the RN module
	if (LoRaBee.initOTA(loraSerial, appEUI, appKey, false)) {
		debugSerial.println("Connection to the network was successful.");
	}
	else {
		debugSerial.println("Connection to the network failed!");
	}

	delay(1000);
	debugSerial.println("pin setup ");
	debugSerial.println(LoRaBee.pinMode(0, analog));
	debugSerial.println(LoRaBee.pinMode(13, digitalIn));
	debugSerial.println(LoRaBee.pinMode(1, digitalOut));
	debugSerial.println(LoRaBee.pinMode(3, digitalOut));
	debugSerial.println(LoRaBee.pinMode(5, digitalOut));

	debugSerial.println("digital: ");
	debugSerial.println(LoRaBee.digitalWrite(1, HIGH));
	delay(200);
	debugSerial.println(LoRaBee.digitalWrite(1, LOW));
	delay(200);
	debugSerial.println(LoRaBee.digitalWrite(3, HIGH));
	delay(200);
	debugSerial.println(LoRaBee.digitalWrite(3, LOW));
	delay(200);
	debugSerial.println(LoRaBee.digitalWrite(5, HIGH));
	delay(200);
	debugSerial.println(LoRaBee.digitalWrite(5, LOW));
	delay(200);
	debugSerial.print("analog: ");
	adcIn = LoRaBee.analogRead(0);
	vddIn = LoRaBee.readVdd();
	debugSerial.println(adcIn);
	debugSerial.print("vdd: ");
	debugSerial.println(vddIn);
	double adcVoltage = (vddIn / 1023.0)*adcIn;

	debugSerial.print("adcVoltage (V)= : ");
	debugSerial.println(adcVoltage/1000);

 debugSerial.print("battVoltage (V)= : ");
debugSerial.println(readBackupBatt());
}

void loop()
{
	while (debugSerial.available()) {
		loraSerial.write((char)debugSerial.read());
	}

	while (loraSerial.available()) {
		debugSerial.write((char)loraSerial.read());
	}
}

/*!
* \brief reads backup battery voltage
*
* \retval float battery voltage in V
*/
float readBackupBatt() {
  uint16_t adcIn, vddIn, adcVoltage;
  adcIn = LoRaBee.analogRead(0);
  vddIn = LoRaBee.readVdd();
  adcVoltage = (vddIn / 1023.0)*adcIn;
 float outputValue = adcIn * (1420.0f / 510.0f * (vddIn/1000.0f) / 1024.0f);
 return outputValue;
}
