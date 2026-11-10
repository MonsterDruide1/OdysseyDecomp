#pragma once

// Interface for judging whether an item exists. The method name is not
// verified against the original binary (no symbol for it exists); only the
// vtable slot (0), signature (bool, no arguments, const) and the
// "JudgeItemExistance" query that calls it are known.
class IItemExistanceJudge {
public:
    virtual bool isExist() const = 0;
};
