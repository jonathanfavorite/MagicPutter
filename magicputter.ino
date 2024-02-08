#include "supporting/CommandAction.h"
#include "supporting/CommandAction.cpp"
#include "supporting/Structures.h"
#include "supporting/Structures.cpp"
#include "supporting/GameData.h"
#include "supporting/GameData.cpp"
#include "supporting/Bluetooth.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/********************************/
/****** GAME RELATED MANAGEMENT ******/
/********************************/
GameDataManager gameManager = GameDataManager();
void setupStartingGameData()
{
  gameManager.setHole(1);
  gameManager.setStrokes(0);
}

/********************************/
/****** MISC SETUP ******/
/********************************/

CustomLEDArray ledArr = CustomLEDArray(2);

/********************************/
/****** BUTTONS SETUP ******/
/********************************/

ButtonKeyValueArray buttonPins = ButtonKeyValueArray(6);

unsigned long standardButtonDebounceDuration = 50; // milliseconds
unsigned long powerButtonDebounceDuration = 2000;  // milliseconds

void setupButtons()
{
  buttonPins.add(0, "HOLE_UP", 45, LOW, 0);
  buttonPins.add(1, "HOLE_DOWN", 43, LOW, 0);
  buttonPins.add(2, "STROKE_UP", 41, LOW, 0);
  buttonPins.add(3, "STROKE_DOWN", 39, LOW, 0);
  buttonPins.add(4, "BLUETOOTH", 37, LOW, 0);
  buttonPins.add(5, "POWER", 35, LOW, 0);
  for (int i = 0; i < buttonPins.getCount(); i++)
  {
    Serial.println(buttonPins.pairs[i].key + " Button Started On Pin: " + String(buttonPins.pairs[i].buttonPin));
    buttonPins.pairs[i].lastButtonState = digitalRead(buttonPins.pairs[i].buttonPin);
    pinMode(buttonPins.pairs[i].buttonPin, INPUT_PULLUP);
  }
}

/********************************/
/****** OLED SETUP ******/
/********************************/
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Reset pin (or -1 if sharing Arduino reset pin)
#define TCAADDR 0x70  // Address of the TCA9548A multiplexer

Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void setupScreens()
{
 // Initialize multiplexer and OLED displays
  for (int i = 0; i < 8; i++) {
    tcaselect(i);
    delay(100);
  }

  // Initialize and configure OLED #1
  tcaselect(1); // Select channel 1 for OLED #1
  if (!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Adjust address if needed
    Serial.println(F("SSD1306 allocation failed for display 1"));
    for (;;);
  }

  drawScoresAndStrokes(display1, "HOLE", "1");
  

  // Initialize and configure OLED #2
  tcaselect(2); // Select channel 2 for OLED #2
  if (!display2.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Adjust address if needed
    Serial.println(F("SSD1306 allocation failed for display 2"));
    for (;;);
  }

  drawScoresAndStrokes(display2, "STROKE", "0");
}

int getCenteredCursorX(Adafruit_SSD1306 &disp, int textSize, const String& text) {
  int16_t x1, y1;
  uint16_t w, h;
  disp.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);  // Get the width and height of the string
  return (SCREEN_WIDTH - w) / 2;
}

void drawScoresAndStrokes(Adafruit_SSD1306 &disp, String topText, String bottomText) {
  disp.clearDisplay();
  disp.setTextSize(2);
  disp.setTextColor(SSD1306_WHITE);
  disp.setCursor(getCenteredCursorX(disp, 2, topText), 0); // Center top text
  disp.println(topText);
  disp.setTextSize(5); // Larger text size for the number
  disp.setCursor(getCenteredCursorX(disp, 5, bottomText), 24); // Center bottom text
  disp.println(bottomText);
  disp.display();
}

/********************************/
/****** BLUETOOTH SETUP ******/
/********************************/

// Object initialization
HardwareSerial& blehm10(Serial1);

KeyValueArray BTColors = KeyValueArray(6);
BT_CONNECTION_STATUS BT_STATUS = BT_CONNECTION_STATUS_DISCONNECTED;

const int BT_STATUS_PIN = 33;

void setupBluetoothIndicatorColorsAndPin()
{
  BTColors.setPair(0, "YELLOW_R", 255);
  BTColors.setPair(1, "YELLOW_G", 0);
  BTColors.setPair(2, "YELLOW_B", 0);
  BTColors.setPair(3, "GREEN_R", 0);
  BTColors.setPair(4, "GREEN_G", 255);
  BTColors.setPair(5, "GREEN_B", 0);

  ledArr.add(0, "BT_INDICATOR", 8, 9, 10);

  pinMode(BT_STATUS_PIN, OUTPUT);

  pinMode(ledArr.getValue("BT_INDICATOR").redLEDPin, OUTPUT);
  pinMode(ledArr.getValue("BT_INDICATOR").greenLEDPin, OUTPUT);
  pinMode(ledArr.getValue("BT_INDICATOR").blueLEDPin, OUTPUT);

}

void setupBluetoothModule()
{
  //Initialize HM-10 BLE module
  blehm10.begin(9600);
  Serial.println("BLE On....");
}

void toggleBluetoothConnected(bool connected)
{
  if(connected)
  {
    BT_STATUS = BT_CONNECTION_STATUS_CONNECTED;
  }
  else
  {
    BT_STATUS = BT_CONNECTION_STATUS_DISCONNECTED;
  }
}

CustomLED getCorrectBluetoothIndicatorColor(float brightness = 1.0f)
{
  CustomLED led;
  if (BT_STATUS == BT_CONNECTION_STATUS_DISCONNECTED)
  {
    led.redLEDPin = BTColors.getValue("YELLOW_R");
    led.greenLEDPin = BTColors.getValue("YELLOW_G");
    led.blueLEDPin = BTColors.getValue("YELLOW_B");
  }
  else
  {
    led.redLEDPin = BTColors.getValue("GREEN_R");
    led.greenLEDPin = BTColors.getValue("GREEN_G");
    led.blueLEDPin = BTColors.getValue("GREEN_B");
  }

  led.redLEDPin = led.redLEDPin * brightness;
  led.greenLEDPin = led.greenLEDPin * brightness;
  led.blueLEDPin = led.blueLEDPin * brightness;
  return led;
}

/********************************/
/****** SETUP ******/
/********************************/

void setup()
{
  Wire.begin();
  Serial.begin(9600);

  Serial.println();
  while (!Serial); // Wait for serial port to connect
  Serial.println("============== STARTING ON SERIAL 9600 ==================");

  // OLED SETUP
  setupScreens();

  // RESET GAME DATA
  setupStartingGameData();

  // BLUETOOTH MODULE START
  setupBluetoothModule();

  // BLUETOOTH LED INDICATOR SETUP
  setupBluetoothIndicatorColorsAndPin();

  // BUTTON PIN SETUP
  setupButtons();



  Serial.println("SETUP COMPLETED");
}

/********************************/
/****** LOOP ******/
/********************************/

void loop()
{
  int btState = digitalRead(BT_STATUS_PIN);
  if(btState == HIGH)
  {
    Serial.println("CONNECTED!");
    //toggleBluetoothConnected(true);
  }
  else
  {
    //toggleBluetoothConnected(false);
  }

  checkButtonsLoop();
  updateBluetoothIndicatorLoop();
  handleBluetoothSendLoop();
  handleBluetoothResponseLoop();
  updateScreensLoop();
}

/********************************/
/****** OLED METHODS ******/
/********************************/
int c1 = 0;
void updateScreensLoop()
{
  tcaselect(1);
  drawScoresAndStrokes(display1, "HOLE", String(gameManager.getHole()));
  tcaselect(2);
  drawScoresAndStrokes(display2, "STROKES", String(gameManager.getStrokes()));
}


/********************************/
/****** GAME MECHANICS LOGIC ******/
/********************************/

void incrementHole()
{
  gameManager.nextHole();
  sendMessage(CMD_ACTION_HOLE_CHANGED_UP, String(gameManager.getHole()));
}
void decrementHole()
{
  gameManager.previousHole();
  sendMessage(CMD_ACTION_HOLE_CHANGED_DOWN, String(gameManager.getHole()));
}
void incrementStroke()
{
  gameManager.incrementStrokes();
  sendMessage(CMD_ACTION_STROKE_CHANGED_UP, String(gameManager.getStrokes()));
}
void decrementStroke()
{
  gameManager.decrementStrokes();
  sendMessage(CMD_ACTION_STROKE_CHANGED_DOWN, String(gameManager.getStrokes()));
}

/********************************/
/****** BLUETOOTH LED LOGIC ******/
/********************************/

void updateBluetoothIndicatorLoop()
{
  CustomLED led = getCorrectBluetoothIndicatorColor(0.3);
  analogWrite(ledArr.getValue("BT_INDICATOR").redLEDPin, led.redLEDPin);
  analogWrite(ledArr.getValue("BT_INDICATOR").greenLEDPin, led.greenLEDPin);
  analogWrite(ledArr.getValue("BT_INDICATOR").blueLEDPin, led.blueLEDPin);
}

/********************************/
/****** BLUETOOTH RESPONSE HANDLING ******/
/********************************/

void handleBluetoothSendLoop()
{
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    Serial.print("SENDING COMMAND: " + input + " | RESPONSE: ");
    blehm10.print(input); // Send command to HM-11
  }
}

const char startMarker = '<';
const char endMarker = '>';


void handleBluetoothResponseLoop() {
    if (blehm10.available()) {
        String blehm10Str = blehm10.readStringUntil('\n');
        blehm10Str.trim(); // Trim it up

          Serial.print("from HM11: ");
        Serial.print(blehm10Str);
        Serial.println();

        // Check and split concatenated messages
        int startIndex = blehm10Str.indexOf('<');
        int endIndex = blehm10Str.lastIndexOf('>');

        if (startIndex != -1 && endIndex != -1 && endIndex > startIndex) {
            // Extract and process the AT/command status part
            String statusPart = blehm10Str.substring(0, startIndex);
            processATCommandOrStatus(statusPart);

            // Extract and process the command part
            String commandPart = blehm10Str.substring(startIndex, endIndex + 1);
            if (commandPart.startsWith("<") && commandPart.endsWith(">")) {
                commandPart = commandPart.substring(1, commandPart.length() - 1); // Remove markers
                processAppMessage(commandPart);
            }
        } else {
            // No concatenation, process normally
            if (blehm10Str.startsWith("OK+") || blehm10Str.startsWith("AT+")) {
                processATCommandOrStatus(blehm10Str);
            } else if (blehm10Str.startsWith("<") && blehm10Str.endsWith(">")) {
                blehm10Str = blehm10Str.substring(1, blehm10Str.length() - 1); // Remove markers
                processAppMessage(blehm10Str);
            } else {
                Serial.println("Unrecognized message format: " + blehm10Str);
            }
        }
    }
}

void processATCommandOrStatus(const String &atCommandResponse) {
    // Your existing code to handle AT command responses
    if (atCommandResponse.startsWith("OK+CONN")) {
        Serial.println("------> NEW BLUETOOTH CONNECTION");
        toggleBluetoothConnected(true);
    }
    else if(atCommandResponse.startsWith("OK+LOST"))
      {
        Serial.println("------> LOST BLUETOOTH CONNECTION");
        toggleBluetoothConnected(false);
      }
      else if(atCommandResponse.startsWith("OK+DISC"))
      { 
        Serial.println("------> BLUETOOTH DISCONNECTION");
        toggleBluetoothConnected(false);
      }
      else 
      {
        blehm10.println(atCommandResponse);
      }
}


void processAppMessage(String &blehm10Str)
{
       blehm10Str.trim(); // trim it up

      
          CommandAction action = parseActionReceived(blehm10Str);
          if(action.actionID == CMD_ACTION_FAILURE) {
              Serial.println("ERROR: " + blehm10Str);
          } else {
              handleIncomingAction(action);
          }

     
        Serial.println(blehm10Str);
        Serial.println();
}

void handleIncomingAction(CommandAction &action)
{
  if(action.actionID == CMD_ACTION_READY)
  {

  }
  else if(action.actionID == CMD_ACTION_SYNC_FROM_PHONE)
  {
    parseActionReceivedSyncFromPhone(action.data);
  }
  else if(action.actionID == CMD_ACTION_HOLE_CHANGED_UP)
  {
    incrementHole();
    Serial.println("Hole Changed (+): " + action.data);
  }
  else if(action.actionID == CMD_ACTION_HOLE_CHANGED_DOWN)
  {
    decrementHole();
    Serial.println("Hole Changed (-): " + action.data);
  }
  else if(action.actionID == CMD_ACTION_STROKE_CHANGED_UP)
  {
    incrementStroke();
    Serial.println("Stroke Changed (+): " + action.data);
  }
  else if(action.actionID == CMD_ACTION_STROKE_CHANGED_DOWN)
  {
    decrementStroke();
    Serial.println("Stroke Changed (-): " + action.data);
  }
}

void parseActionReceivedSyncFromPhone(String &data) {
    // Check if the data is not empty and contains a comma
    if (data.length() > 0 && data.indexOf(',') != -1) {
        // Find the position of the comma
        int commaIndex = data.indexOf(',');

        // Ensure the comma is not at the start or end of the string
        if (commaIndex > 0 && commaIndex < data.length() - 1) {
            // Extract the substrings
            String firstNumberString = data.substring(0, commaIndex);
            String secondNumberString = data.substring(commaIndex + 1);

            // Convert the substrings to integers
            int newHole = firstNumberString.toInt();
            int newStroke = secondNumberString.toInt();

            gameManager.setHole(newHole);
            gameManager.setStrokes(newStroke);

            Serial.println("Synced the Hole/Stroke from Phone");

        } else {
            Serial.println("parseActionReceivedSyncFromPhone -> Invalid format: Comma at the start or end.");
        }
    } else {
        Serial.println("parseActionReceivedSyncFromPhone -> Invalid data: Empty or no comma.");
    }
}

void sendMessage(CommandActionID actionID, const String &message) {
    const int chunkSize = 30;

    CommandAction cmd = CommandAction();
    cmd.actionID = actionID;
    cmd.data = message;

    String json = cmd.toJSONString();

    // int messageLength = json.length();

    // for (int i = 0; i < json.length(); i += chunkSize) {
    //     String chunk = message.substring(i, min(i + chunkSize, messageLength));
    //     blehm10.println(chunk);
    //     delay(50);
    // }

    blehm10.println(json);
    delay(50);
    blehm10.println("@@@");
}


/********************************/
/****** CHECK BUTTONS STATE ******/
/********************************/

void checkButtonsLoop()
{
  for (int i = 0; i < buttonPins.getCount(); i++)
  {
    checkButton(buttonPins.pairs[i]);
  }
}

void checkButton(ButtonKeyValuePair &button)
{
  // getting the ms difference between NOW and when the last time the button was pressed. if it's
  // greater than 50ms, we allow it
  if (millis() - button.lastTimeButtonChanged > standardButtonDebounceDuration)
  {
    byte buttonState = digitalRead(button.buttonPin);
    if (buttonState != button.lastButtonState)
    {
      button.lastTimeButtonChanged = millis();
      button.lastButtonState = buttonState;

      if (button.key == "HOLE_UP")
      {
        handleHoleUpButton(button, buttonState);
      }
      else if (button.key == "HOLE_DOWN")
      {
        handleHoleDownButton(button, buttonState);
      }
      else if (button.key == "STROKE_UP")
      {
        handleStrokeUpButton(button, buttonState);
      }
      else if (button.key == "STROKE_DOWN")
      {
        handleStrokeDownButton(button, buttonState);
      }
      else if (button.key == "BLUETOOTH")
      {
        handleBluetoothButton(button, buttonState);
      }
      else if (button.key == "POWER")
      {
        handlePowerButton(button, buttonState);
      }
    }
  }
}

void handleHoleUpButton(ButtonKeyValuePair &button, byte &buttonState)
{
  if (buttonState == HIGH)
  {
    // TODO: INCREMENT HOLE, SEND COMMAND TO PUTTER
    //toggleBluetoothConnected(true);
    incrementHole();
    Serial.println("Incrementing the hole");
  }
  else
  {
    // BUTTON RELEASED
    // Serial.println("Released");
  }
}
void handleHoleDownButton(ButtonKeyValuePair &button, byte &buttonState)
{
  if (buttonState == HIGH)
  {
    // TODO: DECREMENT HOLE, SEND COMMAND TO PUTTER
    decrementHole();
    Serial.println("Decremening the hole");
  }
  else
  {
    // BUTTON RELEASED
  }
}
void handleStrokeUpButton(ButtonKeyValuePair &button, byte &buttonState)
{
  if (buttonState == HIGH)
  {
    // TODO: INCREMENT STROKE, SEND COMMAND TO PUTTER
    incrementStroke();
    Serial.println("Incrementing Stroke");
  }
  else
  {
    // BUTTON RELEASED
    // Serial.println("Pressed");
  }
}
void handleStrokeDownButton(ButtonKeyValuePair &button, byte &buttonState)
{
  if (buttonState == HIGH)
  {
    // TODO: DECREMENT STROKE, SEND COMMAND TO PUTTER
    decrementStroke();
    Serial.println("Decrementing Stroke");
  }
  else
  {
    // BUTTON RELEASED
    // Serial.println("Released");
  }
}
void handleBluetoothButton(ButtonKeyValuePair &button, byte &buttonState)
{
  if (buttonState == HIGH)
  {
    // TODO: HANDLE POWERING DOWN
    Serial.println("Bluetooth Button Down");
  }
  else
  {
    // BUTTON RELEASED
    // Serial.println("Released");
  }
}
void handlePowerButton(ButtonKeyValuePair &button, byte &buttonState)
{
  if (buttonState == HIGH)
  {
    // TODO: HANDLE POWERING DOWN
    Serial.println("Power Button Down");
  }
  else
  {
    // BUTTON RELEASED
    // Serial.println("Released");
  }
}