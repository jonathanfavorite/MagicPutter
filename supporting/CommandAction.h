#ifndef COMMAND_ACTION_H
#define COMMAND_ACTION_H
#include <ArduinoJson.h>

enum CommandActionID {
  CMD_ACTION_INIT = -1,
  CMD_ACTION_READY = 0,
  CMD_ACTION_FAILURE = 2,
  CMD_ACTION_HOLE_CHANGED_UP = 3,
  CMD_ACTION_HOLE_CHANGED_DOWN = 4,
  CMD_ACTION_STROKE_CHANGED_UP = 5,
  CMD_ACTION_STROKE_CHANGED_DOWN = 6,
  CMD_ACTION_SYNC_FROM_PUTTER = 7,
  CMD_ACTION_SYNC_FROM_PHONE = 8,
  CMD_ACTION_REFRESH_HOLE = 9,
  CMD_ACTION_AT_COMMAND = 10
};

class CommandAction {
public:
  CommandActionID actionID;
  String data;

  CommandAction();
  String toJSONString();
  CommandAction parseActionReceived(String jsonString);
};

#endif // COMMAND_ACTION_H