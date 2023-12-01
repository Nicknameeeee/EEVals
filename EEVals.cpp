#include "EEVals.h"

#include <EEPROM.h>

#if defined(__AVR__)
#define EEVALS_USE_EEPROM_UPDATE
#endif

// Implementation of eepromBase class methods
byte eepromBase::read(uint32_t addr) {
    // Read a byte from EEPROM at the specified address
    return EEPROM.read(addr);
}

void eepromBase::write(uint32_t addr, byte data) {
#ifdef EEVALS_USE_EEPROM_UPDATE
    // If defined, use EEPROM.update() for optimized writing
    EEPROM.update(addr, data);
#else
    // Use EEPROM.write() for standard writing
    EEPROM.write(addr, data);
#endif
}

// Implementation of eepromHelper class methods
void eepromHelper::writeBit(uint32_t addr, boolean bit) {
    int byteIndex = addr / 8;
    int bitIndex = addr % 8;
    uint8_t prevData = eepromBase::read(byteIndex);
    // Set or clear the specific bit
    if (bit) {
        eepromBase::write(byteIndex, prevData | (1 << bitIndex));
    } else {
        eepromBase::write(byteIndex, prevData & ~(1 << bitIndex));
    }
}

boolean eepromHelper::readBit(uint64_t addr) {
    int byteIndex = addr / 8;
    int bitIndex = addr % 8;
    // Read the specific bit
    return (eepromBase::read(byteIndex) & (1 << bitIndex)) != 0;
}

void eepromHelper::writeInt(uint32_t addr, uint64_t value, int count) {
    // Write an integer value bit by bit
    for (int i = 0; i < count; i++) {
        writeBit(addr + i, (value & ((uint64_t)1 << i)) != 0);
    }
}

uint64_t eepromHelper::readInt(uint32_t addr, int count) {
    uint64_t result = 0;
    // Read an integer value bit by bit
    for (int i = 0; i < count; i++) {
        if (readBit(addr + i)) {
            result |= ((uint64_t)1 << i);
        }
    }
    return result;
}

void EEVals::basePtr::set(uint32_t val) {
}

int8_t EEVals::bytePtr::get() {
    return eepromHelper::readInt(address, 8);
}

uint8_t EEVals::bytePtr::getUnsigned() {
    return eepromHelper::readInt(address, 8);
}

void EEVals::bytePtr::set(int8_t val) {
    eepromHelper::writeInt(address, val, 8);
    basePtr::set(val);
}

int16_t EEVals::shortPtr::get() {
    return eepromHelper::readInt(address, 16);
}

uint16_t EEVals::shortPtr::getUnsigned() {
    return eepromHelper::readInt(address, 16);
}

void EEVals::shortPtr::set(int16_t val) {
    eepromHelper::writeInt(address, val, 16);
    basePtr::set(val);
}

int32_t EEVals::intPtr::get() {
    return eepromHelper::readInt(address, 32);
}

uint32_t EEVals::intPtr::getUnsigned() {
    return eepromHelper::readInt(address, 32);
}

void EEVals::intPtr::set(int32_t val) {
    eepromHelper::writeInt(address, val, 32);
    basePtr::set(val);
}

int64_t EEVals::longPtr::get() {
    return eepromHelper::readInt(address, 64);
}

uint64_t EEVals::longPtr::getUnsigned() {
    return eepromHelper::readInt(address, 64);
}

void EEVals::longPtr::set(int64_t val) {
    eepromHelper::writeInt(address, val, 64);
    basePtr::set(val);
}

int64_t EEVals::customSizeValuePtr::get() {
    return eepromHelper::readInt(address, valueSize);
}

uint64_t EEVals::customSizeValuePtr::getUnsigned() {
    return eepromHelper::readInt(address, valueSize);
}

void EEVals::customSizeValuePtr::set(int64_t val) {
    eepromHelper::writeInt(address, val, valueSize);
    basePtr::set(val);
}

bool EEVals::boolPtr::get() {
    return eepromHelper::readInt(address, 1) == 1;
}

void EEVals::boolPtr::set(bool val) {
    eepromHelper::writeInt(address, val ? 1 : 0, 1);
    basePtr::set(val ? 1 : 0);
}

const char *EEVals::strPtr::get() {
    uint32_t laddr = address;
    uint16_t size = 0;
    // Count characters until null terminator or size limit
    while (size <= 65534) {
        char nc = eepromHelper::readInt(laddr, 8);  // Current char
        laddr += 8;                                 // Move to the next byte
        if (nc == '\0')                             // char is NUL, string end
        {
            break;
        }
        size++;
    }
    char *str = new char[size + 1];
    if (str == nullptr) {
        // TODO: Handle memory allocation failure
    }
    uint32_t raddr = address;
    for (int i = 0; i < size; i++) {
        str[i] = eepromHelper::readInt(raddr, 8);  // Get & write byte
        raddr += 8;                                // Move to the next byte
    }
    str[size] = '\0';
    return str;
}
void EEVals::strPtr::set(const char *str) {
    int strLen = strlen(str) + 1;  // Include null terminator
    uint32_t laddr = address;
    for (int i = 0; i < strLen; i++) {
        eepromHelper::writeInt(laddr, str[i], 8);  // Write character
        laddr += 8;                                // Move to the next byte
    }
}

String EEVals::strPtr::getString() {
    return String(this->get());
}