# HomeWizard

## Description
433MHz Arduino Receiver Code for HomeWizard / Easy Funk System Home Control Transmitters.

This library allows you to monitor home control appliances which are compatible with the KaKu/CoCo/EFS 433MHz protocol.

## Prequisites

* Arduino
* A cheap chinese RF 433MHz receiver (search Google/Amazon/eBay for "chinese 433mhz receiver arduino")
* Installation of the [RFControl](https://github.com/pimatic/RFControl) library

## Hardware

Connect the VCC and GND pins of the RF receiver to your power supply, and either one of the DATA pins (which one, doesn't matter) to pin D2 of your Arduino.

## Usage

    #include <HomeWizard.h>
    #define IRQ_PIN 0

    void setup() {
      Serial.begin(9600);
      HomeWizard::startReceiving(IRQ_PIN);
    }

    void loop() {
      HW_DATAGRAM data;

      if (HomeWizard::receiveData(&data))
      {
        Serial.print("Switch: 0x"); Serial.println(data.id, HEX);
        Serial.print("Unit: "); Serial.println(data.unit, DEC);
        Serial.print("State: "); Serial.println(data.state ? "ON" : "OFF");
        Serial.print("All? "); Serial.println(data.all ? "Yes" : "No");
      }
    }
