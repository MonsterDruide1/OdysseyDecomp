#pragma once

#include <gfx/seadColor.h>
#include <math/seadBoundBox.h>
#include <math/seadVector.h>

#include "Library/Yaml/ByamlData.h"
#include "Library/Yaml/ByamlIter.h"

namespace sead {
class WriteStream;
}

namespace al {
bool tryGetByamlU8(u8* out, const ByamlIter& iter, const char* key);
bool tryGetByamlU16(u16* out, const ByamlIter& iter, const char* key);
bool tryGetByamlS16(s16* out, const ByamlIter& iter, const char* key);
bool tryGetByamlS32(s32* out, const ByamlIter& iter, const char* key);
bool tryGetByamlU32(u32* out, const ByamlIter& iter, const char* key);
bool tryGetByamlS64(s64* out, const ByamlIter& iter, const char* key);
bool tryGetByamlU64(u64* out, const ByamlIter& iter, const char* key);
bool tryGetByamlF32(f32* out, const ByamlIter& iter, const char* key);
bool tryGetByamlV2f(sead::Vector2f* out, const ByamlIter& iter);
bool tryGetByamlV3f(sead::Vector3f* out, const ByamlIter& iter);
bool tryGetByamlV4f(sead::Vector4f* out, const ByamlIter& iter);
bool tryGetByamlScale(sead::Vector3f* out, const ByamlIter& iter);
bool tryGetByamlV2s32(sead::Vector2i* out, const ByamlIter& iter);
bool tryGetByamlV3s32(sead::Vector3i* out, const ByamlIter& iter);
bool tryGetByamlBox3f(sead::BoundBox3f* out, const ByamlIter& iter);
bool tryGetByamlV3f(sead::Vector3f* out, const ByamlIter& iter, const char* key);
bool tryGetByamlV2f(sead::Vector2f* out, const ByamlIter& iter, const char* key);
bool tryGetByamlV4f(sead::Vector4f* out, const ByamlIter& iter, const char* key);
bool tryGetByamlScale(sead::Vector3f* out, const ByamlIter& iter, const char* key);
bool tryGetByamlV2s32(sead::Vector2i* out, const ByamlIter& iter, const char* key);
bool tryGetByamlV3s32(sead::Vector3i* out, const ByamlIter& iter, const char* key);
bool tryGetByamlBox3f(sead::BoundBox3f* out, const ByamlIter& iter, const char* key);
bool tryGetByamlString(const char** out, const ByamlIter& iter, const char* key);
bool tryGetByamlColor(sead::Color4f* out, const ByamlIter& iter);
bool tryGetByamlColor(sead::Color4f* out, const ByamlIter& iter, const char* key);
bool tryGetByamlBool(bool* out, const ByamlIter& iter, const char* key);
const char* tryGetByamlKeyStringOrNULL(const ByamlIter& iter, const char* key);
s32 tryGetByamlKeyIntOrZero(const ByamlIter& iter, const char* key);
u32 tryGetByamlKeyU32OrZero(const ByamlIter& iter, const char* key);
f32 tryGetByamlKeyFloatOrZero(const ByamlIter& iter, const char* key);
bool tryGetByamlKeyBoolOrFalse(const ByamlIter& iter, const char* key);
bool tryGetByamlIterByKey(ByamlIter* out, const ByamlIter& iter, const char* key);
bool tryGetByamlKeyAndIntByIndex(const char** key, s32* out, const ByamlIter& iter, s32 index);

const char* getByamlKeyString(const ByamlIter& iter, const char* key);
s32 getByamlKeyInt(const ByamlIter& iter, const char* key);
f32 getByamlKeyFloat(const ByamlIter& iter, const char* key);
bool getByamlKeyBool(const ByamlIter& iter, const char* key);
void getByamlIterByKey(ByamlIter* out, const ByamlIter& iter, const char* key);
void getByamlIterByIndex(ByamlIter* out, const ByamlIter& iter, s32 index);

template <ByamlDataType T>
bool isTypeByIndex(const ByamlIter& iter, s32 index) {
    ByamlData data;
    if (iter.getByamlDataByIndex(&data, index))
        return data.getType() == T;
    return false;
}

template <ByamlDataType T>
bool isTypeByKey(const ByamlIter& iter, const char* key) {
    ByamlData data;
    if (iter.getByamlDataByKey(&data, key))
        return data.getType() == T;
    return false;
}

bool isTypeBoolByIndex(const ByamlIter& iter, s32 index);
bool isTypeBoolByKey(const ByamlIter& iter, const char* key);
bool isTypeIntByIndex(const ByamlIter& iter, s32 index);
bool isTypeIntByKey(const ByamlIter& iter, const char* key);
bool isTypeFloatByIndex(const ByamlIter& iter, s32 index);
bool isTypeFloatByKey(const ByamlIter& iter, const char* key);
bool isTypeStringByIndex(const ByamlIter& iter, s32 index);
bool isTypeStringByKey(const ByamlIter& iter, const char* key);
bool isTypeArrayByIndex(const ByamlIter& iter, s32 index);
bool isTypeArrayByKey(const ByamlIter& iter, const char* key);
bool isTypeHashByIndex(const ByamlIter& iter, s32 index);
bool isTypeHashByKey(const ByamlIter& iter, const char* key);

s32 getByamlIterDataNum(const ByamlIter& iter);

struct PrintParams {
    s32 depth;
    u32 offset;
    PrintParams* parent;
};

void printByamlIter(const u8* data);
void printByamlIter(const ByamlIter& iter);
void printByamlIter_(const ByamlIter& iter, PrintParams* param);
}  // namespace al
