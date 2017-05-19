//===========================================================================================
// HomeWizard.h
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

#ifndef Arduino_HomeWizard_h
#define Arduino_HomeWizard_h

//
// RF Datagram structure
//
typedef struct {
  unsigned long id;
  bool all;
  bool state;
  unsigned int unit;
} HW_DATAGRAM;


//
// Class definition
//
class HomeWizard
{
  public:
 	static void startReceiving(int pin);
	static void stopReceiving();
    static bool receiveData(HW_DATAGRAM *data);
  private:
    static unsigned long lastReceived;
};
#endif
