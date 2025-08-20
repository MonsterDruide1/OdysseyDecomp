#pragma once

namespace al {
class ScreenPointer;
class ScreenPointTarget;
class SensorMsg;

bool sendMsgScreenPointTarget(const SensorMsg& msg, ScreenPointer* pointer,
                              ScreenPointTarget* target);
}  // namespace al
