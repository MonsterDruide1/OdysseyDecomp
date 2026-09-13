#pragma once

#include <nn/account.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class AccountHolder : public HioNode {
public:
    AccountHolder();
    virtual ~AccountHolder();

    bool isValid() const;

    nn::account::UserHandle* getUserHandle() const { return mUserHandle; }

private:
    nn::account::UserHandle* mUserHandle = nullptr;
    nn::account::Uid mUid{};
    bool mIsValid = false;
};

static_assert(sizeof(AccountHolder) == 0x28);
}  // namespace al
