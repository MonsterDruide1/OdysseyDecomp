#pragma once

#include <basis/seadTypes.h>

// NOTE: no bounds check done for any operations
template <typename T, s32 Size>
class FixedHeapArray {
public:
    void alloc() { mPtr = new T[Size]; }

    s32 size() const { return Size; }

    T& operator[](s32 index) { return mPtr[index]; }

    const T& operator[](s32 index) const { return mPtr[index]; }

    T* begin() const { return mPtr; }

    T* end() const { return mPtr + Size; }

private:
    T* mPtr = nullptr;
};
