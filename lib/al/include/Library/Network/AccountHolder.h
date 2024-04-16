#pragma once

#include <nn/account.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class AccountHolder : public HioNode {
public:
    AccountHolder();
    virtual ~AccountHolder();

    bool isValid() const;

private:
    nn::account::UserHandle* mUserHandle = nullptr;
    nn::account::Uid mUid{};
    bool mIsValid = false;
};

static_assert(sizeof(al::AccountHolder) == 0x28);
}  // namespace al
