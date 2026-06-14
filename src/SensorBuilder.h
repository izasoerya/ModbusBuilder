#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <ModbusMaster.h>

class ModbusBase
{
public:
    uint8_t slaveId;
    uint8_t address;
    uint8_t functionCode;
    uint8_t lengthAddress;

    String toString()
    {
        char buffer[37];
        snprintf(buffer, sizeof(buffer),
                 "id:%" PRIu8 ", fc:%" PRIu8 ", add:%" PRIu8 ", len:%" PRIu8 "",
                 slaveId, functionCode, address, lengthAddress);
        return String(buffer);
    }
};

struct ReadResult
{
    uint16_t value;
    uint8_t error;
 
    bool isOk() const { return error == ModbusMaster::ku8MBSuccess; }

        const char *errorMessage() const
    {
        switch (error)
        {
        case ModbusMaster::ku8MBSuccess:
            return "Success";
        case ModbusMaster::ku8MBIllegalFunction:
            return "Illegal Function";
        case ModbusMaster::ku8MBIllegalDataAddress:
            return "Illegal Data Address";
        case ModbusMaster::ku8MBIllegalDataValue:
            return "Illegal Data Value";
        case ModbusMaster::ku8MBSlaveDeviceFailure:
            return "Slave Device Failure";
        case ModbusMaster::ku8MBInvalidSlaveID:
            return "Invalid Slave ID";
        case ModbusMaster::ku8MBInvalidFunction:
            return "Invalid Function";
        case ModbusMaster::ku8MBResponseTimedOut:
            return "Response Timed Out";
        case ModbusMaster::ku8MBInvalidCRC:
            return "Invalid CRC";
        default:
            return "Unknown Error";
        }
    }

        static ReadResult ok(uint16_t val)
    {
        return {val, ModbusMaster::ku8MBSuccess};
    }
 
    static ReadResult fail(uint8_t err)
    {
        return {0, err};
    }
};

class BuilderBaseModbus
{
public:
    virtual ~BuilderBaseModbus() = default;
    virtual BuilderBaseModbus &setSlaveId(uint8_t slaveId) = 0;
    virtual BuilderBaseModbus &setAddress(uint8_t address) = 0;
    virtual BuilderBaseModbus &setFunctionCode(uint8_t functionCode) = 0;
    virtual BuilderBaseModbus &setLengthAddress(uint8_t lengthAddress) = 0;

    virtual uint8_t connect() = 0;
    virtual ReadResult read(uint8_t index) = 0;

    virtual ModbusBase getConfig() = 0;
};

class ModbusRTUBuilder : public BuilderBaseModbus
{
private:
    ModbusBase _base;

    ModbusMaster _node;
    Stream &_ss;
    uint8_t _enablePin = 255;

    static void preTransmission()
    {
        digitalWrite(callbackPin(), HIGH);
    }

    static void postTransmission()
    {
        digitalWrite(callbackPin(), LOW);
    }

    static uint8_t &callbackPin()
    {
        static uint8_t pin = 255;
        return pin;
    }

public:
    ModbusRTUBuilder(Stream &serial);
    ModbusRTUBuilder(Stream &serial, uint8_t enPin);
    ~ModbusRTUBuilder();

    BuilderBaseModbus &setSlaveId(uint8_t slaveId) override;
    BuilderBaseModbus &setAddress(uint8_t address) override;
    BuilderBaseModbus &setFunctionCode(uint8_t functionCode) override;
    BuilderBaseModbus &setLengthAddress(uint8_t lengthAddress) override;

    uint8_t connect() override;
    ReadResult read(uint8_t index) override;

    ModbusBase getConfig() override;
};

class MockModbusRTUBuilder : public BuilderBaseModbus
{
private:
    ModbusBase _base;

    ModbusMaster _node;
    Stream &_ss;
    uint8_t _enablePin = 255;

    static uint8_t &callbackPin()
    {
        static uint8_t pin = 255;
        return pin;
    }

public:
    MockModbusRTUBuilder(Stream &serial);
    MockModbusRTUBuilder(Stream &serial, uint8_t enPin);
    ~MockModbusRTUBuilder();

    BuilderBaseModbus &setSlaveId(uint8_t slaveId) override;
    BuilderBaseModbus &setAddress(uint8_t address) override;
    BuilderBaseModbus &setFunctionCode(uint8_t functionCode) override;
    BuilderBaseModbus &setLengthAddress(uint8_t lengthAddress) override;

    uint8_t connect() override;
    ReadResult read(uint8_t index) override;

    ModbusBase getConfig() override;
};

#endif