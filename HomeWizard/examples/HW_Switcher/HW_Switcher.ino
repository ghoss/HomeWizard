//===========================================================================================
// HW_Switcher - Bathroom Light & Fan Control Example
//
// Created: 14.05.2017
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
#include <elapsedMillis.h>

// Wireless receiver attached to interrupt pin 0 (D2)
#define IRQ_PIN 0

// Switch ID to monitor
#define SWITCH_ID 0x5f53ff

// Number of switch units to monitor
#define NUM_SWITCHES 2

// Individual switch unit IDs
#define UNIT_LIGHT  14
#define UNIT_FAN    13

// Output pin associated with each switch unit
int outputPin[NUM_SWITCHES] = {4, 5};

// Timeout values in milliseconds for each switch unit
unsigned long timeout[NUM_SWITCHES] = {10 * 1000L, 10 * 1000L};

// Time each unit was switch on so far
elapsedMillis timeOn[NUM_SWITCHES];

// Table containing previous states for all switches
bool prevState[NUM_SWITCHES] = {false, false}; 


//-------------------------------------------------------------------------------------------
// setup - This code is run once
//-------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(9600);

  // Initialize output pins
  for (int i = 0; i < NUM_SWITCHES; i ++)
  {
    int pin = outputPin[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  
  // Start receiving data from switches
  HomeWizard::startReceiving(IRQ_PIN);
}


//-------------------------------------------------------------------------------------------
// loop - Microcontroller execution loop
//-------------------------------------------------------------------------------------------

void loop() {
  HW_DATAGRAM data;

  // Listen to datagrams from specific switch only
  if (HomeWizard::receiveData(&data) && (data.id == SWITCH_ID))
  {
    int unitIdx = (data.unit == UNIT_LIGHT) ? 0 : ((data.unit == UNIT_FAN) ? 1 : -1);

    // Ignore invalid switch units
    if (unitIdx != -1)
    {
      // Reset running time to zero if switched on, or to timeout value if switched off
      timeOn[unitIdx] = data.state ? 0 : timeout[unitIdx];
      
      if (data.state && (! prevState[unitIdx]))
      {
        // Switch unit on
        prevState[unitIdx] = true;
        digitalWrite(outputPin[unitIdx], HIGH);
      }
    }
  }

  // Check timeouts for all units
  for (int i = 0; i < NUM_SWITCHES; i ++)
  {
    if (prevState[i] && (timeOn[i] >= timeout[i]))
    {
      // Switch off unit if it was turned on and the timeout elapsed
      prevState[i] = false;
      digitalWrite(outputPin[i], LOW); 
    }
  }
}
