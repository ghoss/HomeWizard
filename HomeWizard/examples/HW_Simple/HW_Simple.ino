//===========================================================================================
// HW_Simple - Simple HomeWizard Receiver Example
//
// Created: 19.05.2017
//
// Copyright (C) 2017 by Guido Hoss
//===========================================================================================
// HW_Switcher is free software: you can redistribute it and/or 
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation, either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
// Git repository home: <https://github.com/ghoss/HomeWizard>
//===========================================================================================

// RF 433MHz library
#include <HomeWizard.h>

// Wireless receiver attached to interrupt pin 0 (D2)
#define IRQ_PIN 0


//-------------------------------------------------------------------------------------------
// setup - This code is run once
//-------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  
  // Start receiving data from switch
  HomeWizard::startReceiving(IRQ_PIN);
}


//-------------------------------------------------------------------------------------------
// loop - Microcontroller execution loop
//-------------------------------------------------------------------------------------------

void loop() {
  HW_DATAGRAM data;

  // Listen to datagrams from specific switch only
  if (HomeWizard::receiveData(&data))
  {
    Serial.print("Switch: 0x"); Serial.println(data.id, HEX);
    Serial.print("Unit: "); Serial.println(data.unit, DEC);
    Serial.print("State: "); Serial.println(data.state ? "ON" : "OFF");
    Serial.print("All? "); Serial.println(data.all ? "Yes" : "No");
  }
}
