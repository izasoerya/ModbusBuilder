#include <Arduino.h>
#include <SensorBuilder.h>

#define EN_PIN 3

ModbusRTUBuilder modbusRTU(Serial2, EN_PIN);

void setup()
{
    Serial.begin(115200);

    modbusRTU.setSlaveId(0x01).setFunctionCode(0x03).setAddress(0x02).setLengthAddress(2);
    modbusRTU.connect();

    Serial.println(modbusRTU.getConfig().toString());
}

void loop()
{

    float temperature = modbusRTU.read(0); // Read first register
    float humidity = modbusRTU.read(1);    // Read second register

    delay(2000);
}