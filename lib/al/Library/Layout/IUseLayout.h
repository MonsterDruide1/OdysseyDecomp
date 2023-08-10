#pragma once

#include "al/Library/HostIO/IUseName.h"

namespace al {
class LayoutKeeper;

class IUseLayout : virtual public IUseName {
public:
    virtual LayoutKeeper* getLayoutKeeper() const = 0;
};
}
