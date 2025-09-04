#pragma once

#include <basis/seadTypes.h>

namespace sead {
class Thread;
}

namespace al {
class ExecuteDirector;

class ModelDrawBufferUpdater {
public:
    ModelDrawBufferUpdater(const ExecuteDirector*);
    void executeUpdateAsync(sead::Thread*, s32);
    ~ModelDrawBufferUpdater();
    void executeUpdate();
    void tryUpdateAsync();
    void waitAsync() const;

private:
    void* filler[3];
};

static_assert(sizeof(ModelDrawBufferUpdater) == 0x18);

}  // namespace al
