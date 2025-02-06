#pragma once

class IJudge;

namespace rs {
void resetJudge(IJudge* judge);
void updateJudge(IJudge* judge);
bool isJudge(const IJudge* judge);
bool updateJudgeAndResult(IJudge* judge);
bool judgeAndResetReturnTrue(IJudge* judge);
}  // namespace rs
