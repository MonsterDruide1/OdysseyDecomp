#pragma once

namespace al {
class ExecuteDirector;

class ModelDrawBufferUpdater {
public:
    ModelDrawBufferUpdater(const ExecuteDirector*);
    ~ModelDrawBufferUpdater();

    // incomplete
private:
    void* filler[3];
};
static_assert(sizeof(ModelDrawBufferUpdater) == 0x18);

}  // namespace al
