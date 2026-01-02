#pragma once

namespace al {

class ExecutorListBase {
public:
    ExecutorListBase(const char* name);

    virtual ~ExecutorListBase() = default;

    virtual void executeList() const;
    virtual bool isActive() const;

    const char* getName() const { return mName; }

private:
    const char* mName;
};

static_assert(sizeof(ExecutorListBase) == 0x10);

}  // namespace al
