#include "CommandAction.h"

CommandAction::CommandAction() : actionID(CMD_ACTION_INIT), data("initializing") {}

String CommandAction::toJSONString() {
  StaticJsonDocument<1024> doc;
  doc["actionID"] = static_cast<int>(actionID);  // Cast enum to int
  doc["data"] = data;
  String output;
  serializeJson(doc, output);
  return output;
}

CommandAction parseActionReceived(String jsonString) {
    DynamicJsonDocument doc(1024); // Adjust size as needed
    DeserializationError error = deserializeJson(doc, jsonString);

    CommandAction cmd;

    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        //Serial.println(error.c_str());
        cmd.actionID = CMD_ACTION_FAILURE; // Indicate an error with a special actionID
        return cmd;
    }

    if (!doc.containsKey("A") || !doc.containsKey("D")) {
        Serial.println(F("JSON does not contain expected keys"));
        cmd.actionID = CMD_ACTION_FAILURE; // Indicate an error with a special actionID
        return cmd;
    }

    cmd.actionID = doc["A"]; // Assuming actionID is an int
    cmd.data = doc["D"].as<String>(); // Assuming data is a string

    return cmd;
}