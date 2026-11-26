#include "SensorBuilder.h"

ModbusRTUBuilder::ModbusRTUBuilder(Stream &serial) : _base(ModbusBase()), _ss(serial) {}

ModbusRTUBuilder::ModbusRTUBuilder(Stream &serial, uint8_t enPin)
    : _base(ModbusBase()), _ss(serial), _enablePin(enPin) {}

ModbusRTUBuilder::~ModbusRTUBuilder() = default;

void ModbusRTUBuilder::connect()
{
    if (_enablePin != 255)
    {
        callbackPin() = _enablePin;
        pinMode(_enablePin, OUTPUT);
        digitalWrite(_enablePin, LOW);

        _node.preTransmission(preTransmission);
        _node.postTransmission(postTransmission);
    }
    _node.begin(_base.slaveId, _ss);
}

ReadResult ModbusRTUBuilder::read(uint16_t index, uint8_t retries = 3)
{
    if (index >= _base.lengthAddress)
        return ReadResult::fail(ModbusMaster::ku8MBIllegalDataAddress);

    if (_base.functionCode == 0x03)
    {
        uint8_t result = 0xE2;
        for (int i = 0; i < retries; i++)
        {
            result = _node.readHoldingRegisters(_base.address, _base.lengthAddress);
            if (result == _node.ku8MBSuccess)
                return ReadResult::ok(_node.getResponseBuffer(index));
            delay(50);
        }
        return ReadResult::fail(result);
    }
    else if (_base.functionCode == 0x04)
    {
        uint8_t result = 226U;
        for (int i = 0; i < retries; i++)
        {
            result = _node.readInputRegisters(_base.address, _base.lengthAddress);
            if (result == _node.ku8MBSuccess)
                return ReadResult::ok(_node.getResponseBuffer(index));
        }
        return ReadResult::fail(result);
    }
    return ReadResult::fail(ModbusMaster::ku8MBInvalidFunction);
}

BuilderBaseModbus &ModbusRTUBuilder::setSlaveId(uint8_t slaveId)
{
    _base.slaveId = slaveId;
    return *this;
}
BuilderBaseModbus &ModbusRTUBuilder::setAddress(uint16_t address)
{
    _base.address = address;
    return *this;
}

BuilderBaseModbus &ModbusRTUBuilder::setFunctionCode(uint8_t functionCode)
{
    _base.functionCode = functionCode;
    return *this;
}

BuilderBaseModbus &ModbusRTUBuilder::setLengthAddress(uint16_t lengthAddress)
{
    _base.lengthAddress = lengthAddress;
    return *this;
}

ModbusBase ModbusRTUBuilder::getConfig()
{
    return _base;
}

MockModbusRTUBuilder::MockModbusRTUBuilder(Stream &serial) : _base(ModbusBase()), _ss(serial) {}

MockModbusRTUBuilder::MockModbusRTUBuilder(Stream &serial, uint8_t enPin)
    : _base(ModbusBase()), _ss(serial), _enablePin(enPin) {}

MockModbusRTUBuilder::~MockModbusRTUBuilder() = default;

void MockModbusRTUBuilder::connect()
{
    if (_enablePin == 255)
        Serial.println("Automatic Flow Control, no EN pin");
    callbackPin() = _enablePin;
    Serial.printf("EN Pin is on: %d\n", _enablePin);
}

ReadResult MockModbusRTUBuilder::read(uint16_t index, uint8_t retries = 3)
{
    if (index >= _base.lengthAddress)
    {
        Serial.println("Request address outbond");
        return ReadResult::fail(ModbusMaster::ku8MBIllegalDataAddress);
    }

    if (_base.functionCode == 0x03)
    {
        Serial.print("Doing read holding: ");
        Serial.print("address: ");
        Serial.print(_base.address + index);
        Serial.println("");
    }
    else if (_base.functionCode == 0x04)
    {
        Serial.print("Doing read input: ");
        Serial.print("address: ");
        Serial.print(_base.address + index);
        Serial.println("");
    }
    return ReadResult::fail(ModbusMaster::ku8MBInvalidFunction);
}

BuilderBaseModbus &MockModbusRTUBuilder::setSlaveId(uint8_t slaveId)
{
    _base.slaveId = slaveId;
    return *this;
}
BuilderBaseModbus &MockModbusRTUBuilder::setAddress(uint16_t address)
{
    _base.address = address;
    return *this;
}

BuilderBaseModbus &MockModbusRTUBuilder::setFunctionCode(uint8_t functionCode)
{
    _base.functionCode = functionCode;
    return *this;
}

BuilderBaseModbus &MockModbusRTUBuilder::setLengthAddress(uint16_t lengthAddress)
{
    _base.lengthAddress = lengthAddress;
    return *this;
}

ModbusBase MockModbusRTUBuilder::getConfig()
{
    return _base;
}
