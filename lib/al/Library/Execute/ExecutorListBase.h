#pragma once

namespace al {

class ExecutorListBase {
public:
    ExecutorListBase(const char* name);

    virtual ~ExecutorListBase();
    virtual void executeList() const;
    virtual bool isActive() const;

private:
    const char* mName;
};

static_assert(sizeof(ExecutorListBase) == 0x10);

}  // namespace al
