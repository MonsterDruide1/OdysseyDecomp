#pragma once

#include <nn/account.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class AccountHolder: public HioNode {
public:
    AccountHolder();
    ~AccountHolder();

    bool isValid() const;	
private:
    UserHandle *mUserHandle;
    Uid mUid;
    bool mIsValid;
};

static_assert(sizeof(AccountHolder) == 0x28);
}  // namespace al
