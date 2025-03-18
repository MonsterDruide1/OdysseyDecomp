#include "Library/Network/AccountHolder.h"

#include <vapours/results/results_common.hpp>

namespace al {
AccountHolder::AccountHolder() {
    nn::account::Initialize();
    mUserHandle = new nn::account::UserHandle;

    if (nn::account::OpenPreselectedUser(mUserHandle).IsSuccess() &&
        nn::account::GetUserId(&mUid, *mUserHandle).IsSuccess())
        mIsValid = true;
}

AccountHolder::~AccountHolder() {
    nn::account::CloseUser(*mUserHandle);

    delete mUserHandle;
}

bool AccountHolder::isValid() const {
    return mIsValid;
}
}  // namespace al
