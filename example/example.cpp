#include <Arduino.h>
#include <SensorBuilder.h>
#include <SoftwareSerial.h>

#define EN_PIN 3

EspSoftwareSerial::UART myPort;
MockModbusRTUBuilder modbusRTU(myPort);

void setup()
{
    Serial.begin(115200);
    myPort.begin(9600, SWSERIAL_8N1, 16, 17);

    modbusRTU.setSlaveId(0x01).setFunctionCode(0x03).setAddress(0x02).setLengthAddress(2);
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
    ReadResult temperatureResult = modbusRTU.read(0);
    if (temperatureResult.isOk())
    {
        float temperature = modbusRTU.read(0).value;
    }
    else
    {
        String error = modbusRTU.read(0).errorMessage();
        Serial.println(error);
    }

    ReadResult humidityResult = modbusRTU.read(1);
    if (humidityResult.isOk())
    {
        float humidity = modbusRTU.read(1).value;
    }
    else
    {
        String error = modbusRTU.read(1).errorMessage();
        Serial.println(error);
    }

    delay(2000);
}