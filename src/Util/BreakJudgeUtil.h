#pragma once

namespace al {
class SensorMsg;
class HitSensor;
}  // namespace al

namespace rs {
typedef bool (*break_judge_function_t)(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);

break_judge_function_t getBreakJudgeFunction(const char* name);
}  // namespace rs
