#include "Library/Math/MatrixPtrHolder.h"

#include "Library/Base/StringUtil.h"

namespace al {

MtxPtrHolder::MtxPtrHolder() = default;

void MtxPtrHolder::init(s32 size) {
    mSize = size;
    mNames = new const char*[size];
    mMtxArray = new const sead::Matrix34f*[size];

    for (s32 i = 0; i < mSize; i++)
        setMtxPtrAndName(i, nullptr, nullptr);
}

void MtxPtrHolder::setMtxPtrAndName(s32 index, const char* name, const sead::Matrix34f* mtx) {
    // NOTE: No bounds checking. Arbitrary write to memory!
    mNames[index] = name;
    mMtxArray[index] = mtx;
}

void MtxPtrHolder::setMtxPtr(const char* name, const sead::Matrix34f* mtx) {
    // BUG: findIndex returns -1 when the name isn't found. This can write out of bounds.
    mMtxArray[findIndex(name)] = mtx;
}

s32 MtxPtrHolder::findIndex(const char* name) const {
    return tryFindIndex(name);
}

const sead::Matrix34f* MtxPtrHolder::findMtxPtr(const char* name) const {
    // BUG: findIndex returns -1 when the name isn't found. This can return an invalid ptr.
    return mMtxArray[findIndex(name)];
}

const sead::Matrix34f* MtxPtrHolder::tryFindMtxPtr(const char* name) const {
    s32 index = tryFindIndex(name);

    if (index < 0)
        return nullptr;

    return mMtxArray[index];
}

s32 MtxPtrHolder::tryFindIndex(const char* name) const {
    for (s32 i = 0; i < mSize; i++)
        if (isEqualString(mNames[i], name))
            return i;
    return -1;
}

}  // namespace al
