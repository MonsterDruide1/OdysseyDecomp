#pragma once

namespace al {
class SensorMsg;
}

namespace rs {
typedef bool (*break_judge_function_t)(const al::SensorMsg*);

break_judge_function_t getBreakJudgeFunction(const char* name);
bool isMsgBreakByNormalAttack(const al::SensorMsg* sensorMsg);
bool isMsgBreakCage(const al::SensorMsg* sensorMsg);
bool isMsgBreakByExplosion(const al::SensorMsg* sensorMsg);
bool isMsgBreakBySword(const al::SensorMsg* sensorMsg);
bool isMsgStatueDrop(const al::SensorMsg* sensorMsg);
}  // namespace rs
