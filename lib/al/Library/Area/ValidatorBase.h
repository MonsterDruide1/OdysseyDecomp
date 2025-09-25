#pragma once

namespace al {
class ValidatorBase {
public:
    virtual bool validate() const = 0;
    virtual ValidatorBase* fnc_8() const = 0;
};
}  // namespace al
