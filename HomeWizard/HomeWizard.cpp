//===========================================================================================
// HomeWizard.cpp
//
// 433MHz Radio Remote Control Receiver compatible with HomeWizard and EFS
// (Easy Funk System) transmitters and appliances.
//
// Created: 17.05.2017
//
// Copyright (C) 2017 by Guido Hoss
//===========================================================================================
// HomeWizard is free software: you can redistribute it and/or 
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

// To interface with the 433MHz receiver, this module uses "RFControl", which is available at
// https://github.com/pimatic/RFControl
#include <HomeWizard.h>
#include <Arduino.h>
#include <RFControl.h>

// Size of a raw datagram sent by the switch
#define PACKET_LENGTH 132
#define BOUNCE_TIME	300

// Timestamp to keep track of intervals between datagrams
unsigned long HomeWizard::lastReceived;


//-------------------------------------------------------------------------------------------
// startReceiving
//
// Start listening for datagrams on interrupt pin "pin".
//-------------------------------------------------------------------------------------------

void HomeWizard::startReceiving(int pin)
{
	HomeWizard::lastReceived = 0L;
	RFControl::startReceiving(pin);
}


//-------------------------------------------------------------------------------------------
// stopReceiving
//
// Stop listening for datagrams.
//-------------------------------------------------------------------------------------------

void HomeWizard::stopReceiving()
{
	RFControl::stopReceiving();
}


//-------------------------------------------------------------------------------------------
// receiveData
//
// Check if a datagram has been received from the switch, and return the processed data.
//
// data :	Pointer to a datagram structure
//
// Returns TRUE if a datagram was received, and places its data in "data". Otherwise, returns
// FALSE and the contents of "data" are undefined.
//-------------------------------------------------------------------------------------------

bool HomeWizard::receiveData(HW_DATAGRAM *data)
{
  bool res = false;

  // Check if data has been received
  if (RFControl::hasData()) {
    unsigned int *timings;
    unsigned int timings_size;
    unsigned long now;

    // Get the received datagram
    RFControl::getRaw(&timings, &timings_size);

    // Must be PACKET_LENGTH pulses long; else ignore it
    if (timings_size == PACKET_LENGTH)
    {
      unsigned long timing = timings[PACKET_LENGTH - 1];

      // Check for high last byte in footer
      if (timing >= 1024)
      {
        // Ignore the first 2 and the last 2 bytes (header and footer)
        unsigned bitCount = 0;
        res = true;
        data->id = data->unit = data->state = data->all = 0;
        
        for(int i = 2; i < PACKET_LENGTH - 2; i += 4, bitCount ++) {          
          unsigned bitVal;
          
          // Check the 2nd and 4th value for high bits
          if (timings[i + 1] & 0xff00)
          {
             bitVal = 0;
          }
          else if (timings[i + 3] & 0xff00)
          {
             bitVal = 1;
          }
          else
          {
            // Illegal packet; abort decoding
            res = false;
            break;
          }

          // Process bit depending on position
          if (bitCount < 26)
          {
            data->id = (data->id << 1) | bitVal;
          }
          else if (bitCount == 26)
          {
            data->all = bitVal;
          }
          else if (bitCount == 27)
          {
            data->state = bitVal;
          }
          else {
            data->unit = (data->unit << 1) | bitVal;
          }
        }
        RFControl::continueReceiving();
        
        // Ignore the datagram if it was received shortly after the previous one
        // (i.e. implement a debouncing effect)
        now = millis();
        if ((unsigned long) (now - HomeWizard::lastReceived) < BOUNCE_TIME)
        {
        	// Ignore datagram
        	res = false;
        }
 	    HomeWizard::lastReceived = now;
	  }
    }
  }  
  return res;
}
