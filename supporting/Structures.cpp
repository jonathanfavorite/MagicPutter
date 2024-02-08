#include "Structures.h"


/********************************/
/****** KeyValueArray ******/
/********************************/


KeyValueArray::KeyValueArray(int size) {
  this->size = size;
  pairs = new KeyValuePair[size];
}

void KeyValueArray::setPair(int index, String key, int value) {
  if (index >= 0 && index < size) {
    pairs[index].key = key;
    pairs[index].value = value;
  }
}

int KeyValueArray::getValue(String key) {
  for (int i = 0; i < size; i++) {
    if (pairs[i].key == key) {
      return pairs[i].value;
    }
  }
  return -1;
}
int KeyValueArray::getCount() {
  return size;
}

void KeyValueArray::printAll() {
  for (int i = 0; i < size; i++) {
    Serial.println(pairs[i].key + ": " + pairs[i].value);
  }
}



/********************************/
/****** ButtonKeyValueArray ******/
/********************************/
ButtonKeyValueArray::ButtonKeyValueArray(int size) {
  this->size = size;
  pairs = new ButtonKeyValuePair[size];
}

void ButtonKeyValueArray::add(int index, String key, int buttonPin, byte lastButtonState, unsigned long lastTimeButtonChanged) {
  if (index >= 0 && index < size) {
    pairs[index].key = key;
    pairs[index].buttonPin = buttonPin;
    pairs[index].lastButtonState = lastButtonState;
    pairs[index].lastTimeButtonChanged = lastTimeButtonChanged;
  }
}

ButtonKeyValuePair ButtonKeyValueArray::getValue(String key) {
  for (int i = 0; i < size; i++) {
    if (pairs[i].key == key) {
      return pairs[i];
    }
  }
  return ButtonKeyValuePair();
}
int ButtonKeyValueArray::getCount() {
  return size;
}

void ButtonKeyValueArray::printAll() {
  for (int i = 0; i < size; i++) {
    Serial.println(pairs[i].key + ": " + pairs[i].buttonPin);
  }
}




/********************************/
/****** CustomLED and CustomLED Array ******/
/********************************/
CustomLEDArray::CustomLEDArray(int size) {
  this->size = size;
  items = new CustomLED[size];
}

void CustomLEDArray::add(int index, String key, int redLEDPin, int greenLEDPin, int blueLEDPin, bool enabled = false) {
  if (index >= 0 && index < size) {
    items[index].key = key;
    items[index].greenLEDPin = greenLEDPin;
    items[index].redLEDPin = redLEDPin;
    items[index].blueLEDPin = blueLEDPin;
  }
}

CustomLED CustomLEDArray::getValue(String key) {
  for (int i = 0; i < size; i++) {
    if (items[i].key == key) {
      return items[i];
    }
  }
  return CustomLED();
}
int CustomLEDArray::getCount() {
  return size;
}

void CustomLEDArray::printAll() {
  for (int i = 0; i < size; i++) {
    Serial.println(items[i].key + "| R: " + items[i].redLEDPin + " G: " + items[i].greenLEDPin + " B: " + items[i].blueLEDPin);
  }
}

/*
struct CustomLED {
  String Key;
  int greenLEDPin;
  int redLEDPin;
  int blueLEDPin;
  bool enabled;
};

class CustomLEDArray {
public:
  CustomLEDArray(int size);
  void add(int index, String key, int greenLEDPin, int redLEDPin, int blueLEDPin, enabled = false);
  CustomLED getValue(String key);
  void printAll();
  int getCount();
  CustomLED* items;

private:
  int size;
};
*/

