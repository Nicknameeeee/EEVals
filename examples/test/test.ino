#include <EEPROM.h>
#include <EEVals.h>

void setup() {
  Serial.begin(115200);
  if (!EEPROM.begin(512)) {
    Serial.println("Failed to initialise EEPROM");
    return;
  }
  EEVals::bytePtr myByte = EEVals::bytePtr(0); // <valName>Ptr's first argument is address(uint32_t) in bits, for 512 eeprom bytes you can write 4096 bits, so for Arduino Nano avaliable addresses are 0-4096 
  Serial.print("Old myByte: ");
  Serial.println(myByte.get());
  myByte.set(111);
  Serial.print("New myByte: ");
  Serial.println(myByte.get());

  EEVals::shortPtr myShort = EEVals::shortPtr(100);
  Serial.print("Old myShort: ");
  Serial.println(myShort.get());
  myShort.set(5656);
  Serial.print("New myShort: ");
  Serial.println(myShort.get());

  EEVals::intPtr myInt = EEVals::intPtr(200);
  Serial.print("Old myInt: ");
  Serial.println(myInt.get());
  myInt.set(56645546);
  Serial.print("New myInt: ");
  Serial.println(myInt.get());

  EEVals::longPtr myLong = EEVals::longPtr(300);
  Serial.print("Old myLong: ");
  Serial.println(myLong.get());
  myLong.set(18446744073709551610);
  Serial.print("New myLong: ");
  Serial.println(myLong.get());
  Serial.print("New unsigned myLong: "); // aslo you can read unsigned data
  Serial.println(myLong.getUnsigned());

  EEVals::strPtr myStr = EEVals::strPtr(400);
  /* Some useful stuff about strPtr:
   * strPtr encodes string with 0x0 byte in the end (NUL), so if you want to calculate string size add 8 bits
   * utf8 chars are 1 - 4 bytes (8 - 32 bits!) sized, so it's better to use only ASCII string to save memory
   * this data can be used to calculate string size(for exeample, to saving passwords, which can be dynamic)
   */
  myStr.set("Some text, also you can write utf8 string");
  Serial.print("New myStr: ");
  Serial.println(myStr.get()); // as const char*
  Serial.print("New myStr as String: ");
  Serial.println(myStr.getString()); // as String
  EEPROM.commit();
}

void loop() {
	
}
