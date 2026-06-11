#include "SensorBuilder.h"

ModbusRTUBuilder::ModbusRTUBuilder(Stream &serial) : _base(ModbusBase()), _ss(serial) {}

ModbusRTUBuilder::ModbusRTUBuilder(Stream &serial, uint8_t enPin)
    : _base(ModbusBase()), _ss(serial), _enablePin(enPin) {}

ModbusRTUBuilder::~ModbusRTUBuilder() = default;

uint8_t ModbusRTUBuilder::connect()
{
    if (_enablePin = 255)
    {
        callbackPin() = _enablePin;
        pinMode(_enablePin, OUTPUT);
        digitalWrite(_enablePin, LOW);

        _node.preTransmission(preTransmission);
        _node.postTransmission(postTransmission);
    }

    _node.begin(_base.slaveId, _ss);
    uint8_t result = _node.readHoldingRegisters(0x00, 1);
    if (result == _node.ku8MBSuccess)
    {
        return true;
    }

    // TODO: PARSE ERROR / VERBOSE
    return result;
}

uint16_t ModbusRTUBuilder::read(uint8_t responseIndex)
{
    if (responseIndex >= _base.lengthAddress)
    {
        return 0;
    }

    if (_base.functionCode == 0x03)
    {
        uint8_t result = _node.readHoldingRegisters(_base.address, _base.lengthAddress);
        if (result == _node.ku8MBSuccess)
        {
            return _node.getResponseBuffer(responseIndex);
        }
    }
    else if (_base.functionCode == 0x04)
    {
        uint8_t result = _node.readInputRegisters(_base.address, _base.lengthAddress);
        if (result == _node.ku8MBSuccess)
        {
            return _node.getResponseBuffer(responseIndex);
        }
    }
    return 0;
}

BuilderBaseModbus &ModbusRTUBuilder::setSlaveId(uint8_t slaveId)
{
    _base.slaveId = slaveId;
    return *this;
}
BuilderBaseModbus &ModbusRTUBuilder::setAddress(uint8_t address)
{
    _base.address = address;
    return *this;
}

BuilderBaseModbus &ModbusRTUBuilder::setFunctionCode(uint8_t functionCode)
{
    _base.functionCode = functionCode;
    return *this;
}

BuilderBaseModbus &ModbusRTUBuilder::setLengthAddress(uint8_t lengthAddress)
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

uint8_t MockModbusRTUBuilder::connect()
{
    if (_enablePin == 255)
    {
        Serial.println("Automatic Flow Control, no EN pin");
        return 1;
    }
    callbackPin() = _enablePin;
    Serial.print("EN Pin: ");
    Serial.println(_enablePin);
    return 1;
}

uint16_t MockModbusRTUBuilder::read(uint8_t responseIndex)
{
    if (responseIndex >= _base.lengthAddress)
    {
        Serial.println("Request address outbond");
        return 0;
    }

    if (_base.functionCode == 0x03)
    {
        Serial.print("Doing read holding: ");
        Serial.print("address: ");
        Serial.print(_base.address + responseIndex);
        Serial.println("");
    }
    else if (_base.functionCode == 0x04)
    {
        Serial.print("Doing read input: ");
        Serial.print("address: ");
        Serial.print(_base.address + responseIndex);
        Serial.println("");
    }
    return 0;
}

BuilderBaseModbus &MockModbusRTUBuilder::setSlaveId(uint8_t slaveId)
{
    _base.slaveId = slaveId;
    return *this;
}
BuilderBaseModbus &MockModbusRTUBuilder::setAddress(uint8_t address)
{
    _base.address = address;
    return *this;
}

BuilderBaseModbus &MockModbusRTUBuilder::setFunctionCode(uint8_t functionCode)
{
    _base.functionCode = functionCode;
    return *this;
}

BuilderBaseModbus &MockModbusRTUBuilder::setLengthAddress(uint8_t lengthAddress)
{
    _base.lengthAddress = lengthAddress;
    return *this;
}

ModbusBase MockModbusRTUBuilder::getConfig()
{
    return _base;
}
