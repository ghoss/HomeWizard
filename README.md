# HomeWizard

## Description
433MHz Arduino Receiver Code for HomeWizard / Easy Funk System Home Control Transmitters

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
