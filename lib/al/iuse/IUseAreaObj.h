#pragma once

namespace al {
class AreaObjDirector;

class IUseAreaObj {
public:
    virtual AreaObjDirector* getAreaObjDirector() const = 0;
};
}  // namespace al
