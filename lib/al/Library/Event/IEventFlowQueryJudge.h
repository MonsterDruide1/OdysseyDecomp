#pragma once

namespace al {
class IEventFlowQueryJudge {
public:
    virtual const char* judgeQuery(const char* queryName) const = 0;
};
}  // namespace al
