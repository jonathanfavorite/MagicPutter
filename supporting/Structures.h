#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <Arduino.h>

struct KeyValuePair {
  String key;
  int value;
};

class KeyValueArray {
public:
  KeyValueArray(int size);
  void setPair(int index, String key, int value);
  int getValue(String key);
  void printAll();
  int getCount();

private:
  KeyValuePair* pairs;
  int size;
};


struct ButtonKeyValuePair {
    String key;
    int buttonPin;
    byte lastButtonState;
    unsigned long lastTimeButtonChanged;
};

class ButtonKeyValueArray {
public:
  ButtonKeyValueArray(int size);
  void add(int index, String key, int buttonPin, byte lastButtonState, unsigned long lastTimeButtonChanged);
  ButtonKeyValuePair getValue(String key);
  void printAll();
  int getCount();
  ButtonKeyValuePair* pairs;
  
private:
  int size;
};


/*********************************/
/* CUSTOM LED AND ARRAY
/**********************************/

struct CustomLED {
  String key;
  int greenLEDPin;
  int redLEDPin;
  int blueLEDPin;
  bool enabled;
};

class CustomLEDArray {
public:
  CustomLEDArray(int size);
  void add(int index, String key, int redLEDPin, int greenLEDPin, int blueLEDPin, bool enabled);
  CustomLED getValue(String key);
  void printAll();
  int getCount();
  CustomLED* items;

private:
  int size;
};

#endif // STRUCTURES_H