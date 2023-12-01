#pragma once
#include <Arduino.h>

class eepromBase  // You can change this part of code to work eith custom eeprom libraries
{
   public:
    static byte read(uint32_t addr);
    static void write(uint32_t addr, byte data);
};
class eepromHelper {
   public:
    static void writeBit(uint32_t addr, bool bit);
    static bool readBit(uint64_t addr);
    static void writeInt(uint32_t addr, uint64_t value, int count);
    static uint64_t readInt(uint32_t addr, int count);
};
namespace EEVals {
    class basePtr {
       public:
        basePtr(uint16_t addr) : address(addr) {}
        // virtual uint32_t get() = 0;
        virtual void set(uint32_t val);

       protected:
        uint16_t address;
    };

    class bytePtr : public basePtr {
       public:
        bytePtr(uint32_t addr) : basePtr(addr) {}
        int8_t get();
        uint8_t getUnsigned();
        void set(int8_t val);
    };

    class shortPtr : public basePtr {
       public:
        shortPtr(uint32_t addr) : basePtr(addr) {}
        int16_t get();
        uint16_t getUnsigned();
        void set(int16_t val);
    };

    class intPtr : public basePtr {
       public:
        intPtr(uint32_t addr) : basePtr(addr) {}
        int32_t get();
        uint32_t getUnsigned();
        void set(int32_t val);
    };

    class longPtr : public basePtr {
       public:
        longPtr(uint32_t addr) : basePtr(addr) {}
        int64_t get();
        uint64_t getUnsigned();
        void set(int64_t val);
    };

    class customSizeValuePtr : public basePtr {
       public:
        customSizeValuePtr(uint32_t addr, uint32_t size) : basePtr(addr), valueSize(size) {}
        int64_t get();
        uint64_t getUnsigned();
        void set(int64_t val);

       private:
        uint8_t valueSize;
    };

    class boolPtr : public basePtr {
       public:
        boolPtr(uint32_t addr) : basePtr(addr) {}
        bool get();
        void set(bool val);
    };

    class strPtr : public basePtr {
       public:
        strPtr(uint32_t addr)
            : basePtr(addr) {}
        const char *get();
        void set(const char *val);
        String getString();
    };
}  // namespace EEVals