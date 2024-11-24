#pragma once

namespace al {
class SensorMsg;
}

namespace rs {
typedef bool (*break_judge_function_t)(const al::SensorMsg*);

break_judge_function_t getBreakJudgeFunction(const char* name);
}  // namespace rs
