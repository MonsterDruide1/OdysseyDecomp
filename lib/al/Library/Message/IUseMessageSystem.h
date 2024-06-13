#pragma once

namespace al {
class MessageSystem;

class IUseMessageSystem {
public:
    virtual const MessageSystem* getMessageSystem() const;
};
}  // namespace al
