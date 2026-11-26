#include <Arduino.h>
#include <SensorBuilder.h>
#include <SoftwareSerial.h>

#define EN_PIN 3

EspSoftwareSerial::UART myPort;
ModbusRTUBuilder modbusRTU(myPort);

void setup()
{
    Serial.begin(115200);
    myPort.begin(9600, SWSERIAL_8N1, 16, 17);

    modbusRTU.setSlaveId(0x01)
        .setFunctionCode(0x03)
        .setAddress(0x00)
        .setLengthAddress(2);
    modbusRTU.connect();

    Serial.println(modbusRTU.getConfig().toString());
}

void loop()
{
    /**
     * @brief Recommended way to read modbus device
     * Store the object result in a variable, then check if its not an error
     *
     * This way we can be sure that the data is not an error code
     */
    ReadResult resTemp = modbusRTU.read(0);
    if (resTemp.isOk())
    {
        float temperature = resTemp.value;
        Serial.printf("Temperature is: %d\n", temperature);
    }
    else
    {
        Serial.printf("Err Code: %d | %s\n",
                      resTemp.error, resTemp.errorMessage());
    }

    ReadResult resHum = modbusRTU.read(1);
    if (resHum.isOk())
    {
        float humidity = modbusRTU.read(1).value;
        Serial.printf("Humidity is: %d\n", humidity);
    }
    else
    {
        Serial.printf("Err Code: %d | %s\n",
                      resHum.error, resHum.errorMessage());
    }

    delay(2000);
}