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
bool tryGetByamlU8(u8* pOut, const ByamlIter& rIter, const char* pKey);
bool tryGetByamlU16(u16* pOut, const ByamlIter& rIter, const char* pKey);
bool tryGetByamlS16(s16* pOut, const ByamlIter& rIter, const char* pKey);
bool tryGetByamlS32(s32* pOut, const ByamlIter& rIter, const char* pKey);
bool tryGetByamlU32(u32* pOut, const ByamlIter& rIter, const char* pKey);
bool tryGetByamlS64(s64* pOut, const ByamlIter& rIter, const char* pKey);
bool tryGetByamlU64(u64* pOut, const ByamlIter& rIter, const char* pKey);
bool tryGetByamlF32(f32* pOut, const ByamlIter& rIter, const char* pKey);
bool tryGetByamlV2f(sead::Vector2f* pOut, const ByamlIter& rIter);
bool tryGetByamlV3f(sead::Vector3f* pOut, const ByamlIter& rIter);
bool tryGetByamlV4f(sead::Vector4f* pOut, const ByamlIter& rIter);
bool tryGetByamlScale(sead::Vector3f* pOut, const ByamlIter& rIter);
bool tryGetByamlV2s32(sead::Vector2i* pOut, const ByamlIter& rIter);
bool tryGetByamlV3s32(sead::Vector3i* pOut, const ByamlIter& rIter);
bool tryGetByamlBox3f(sead::BoundBox3f* pOut, const ByamlIter& rIter);
bool tryGetByamlV3f(sead::Vector3f* pOut, const ByamlIter& sIter, const char* key);
bool tryGetByamlV2f(sead::Vector2f* pOut, const ByamlIter& sIter, const char* key);
bool tryGetByamlV4f(sead::Vector4f* pOut, const ByamlIter& sIter, const char* key);
bool tryGetByamlScale(sead::Vector3f* pOut, const ByamlIter& sIter, const char* key);
bool tryGetByamlV2s32(sead::Vector2i* pOut, const ByamlIter& sIter, const char* key);
bool tryGetByamlV3s32(sead::Vector3i* pOut, const ByamlIter& sIter, const char* key);
bool tryGetByamlBox3f(sead::BoundBox3f* pOut, const ByamlIter& sIter, const char* key);
bool tryGetByamlString(const char** pOut, const ByamlIter& rIter, const char* key);
bool tryGetByamlColor(sead::Color4f* pOut, const ByamlIter& rIter);
bool tryGetByamlColor(sead::Color4f* pOut, const ByamlIter& sIter, const char* key);
bool tryGetByamlBool(bool* pOut, const ByamlIter& rIter, const char* key);
const char* tryGetByamlKeyStringOrNULL(const ByamlIter& rIter, const char* key);
s32 tryGetByamlKeyIntOrZero(const ByamlIter& rIter, const char* key);
u32 tryGetByamlKeyU32OrZero(const ByamlIter& rIter, const char* key);
f32 tryGetByamlKeyFloatOrZero(const ByamlIter& rIter, const char* key);
bool tryGetByamlKeyBoolOrFalse(const ByamlIter& rIter, const char* key);
bool tryGetByamlIterByKey(ByamlIter* pOut, const ByamlIter& rIter, const char* key);
bool tryGetByamlKeyAndIntByIndex(const char** key, s32* pOut, const ByamlIter& rIter, s32 index);

const char* getByamlKeyString(const ByamlIter& rIter, const char* key);
s32 getByamlKeyInt(const ByamlIter& rIter, const char* key);
f32 getByamlKeyFloat(const ByamlIter& rIter, const char* key);
bool getByamlKeyBool(const ByamlIter& rIter, const char* key);
void getByamlIterByKey(ByamlIter* pOut, const ByamlIter& rIter, const char* pKey);
void getByamlIterByIndex(ByamlIter* pOut, const ByamlIter& rIter, s32 index);

template <ByamlDataType T>
bool isTypeByIndex(const ByamlIter& rIter, s32 index) {
    ByamlData data;
    if (rIter.getByamlDataByIndex(&data, index))
        return data.getType() == T;
    return false;
}

template <ByamlDataType T>
bool isTypeByKey(const ByamlIter& rIter, const char* key) {
    ByamlData data;
    if (rIter.getByamlDataByKey(&data, key))
        return data.getType() == T;
    return false;
}

bool isTypeBoolByIndex(const ByamlIter& rIter, s32 index);
bool isTypeBoolByKey(const ByamlIter& rIter, const char* pKey);
bool isTypeIntByIndex(const ByamlIter& rIter, s32 index);
bool isTypeIntByKey(const ByamlIter& rIter, const char* pKey);
bool isTypeFloatByIndex(const ByamlIter& rIter, s32 index);
bool isTypeFloatByKey(const ByamlIter& rIter, const char* pKey);
bool isTypeStringByIndex(const ByamlIter& rIter, s32 index);
bool isTypeStringByKey(const ByamlIter& rIter, const char* pKey);
bool isTypeArrayByIndex(const ByamlIter& rIter, s32 index);
bool isTypeArrayByKey(const ByamlIter& rIter, const char* pKey);
bool isTypeHashByIndex(const ByamlIter& rIter, s32 index);
bool isTypeHashByKey(const ByamlIter& rIter, const char* pKey);

s32 getByamlIterDataNum(const ByamlIter& rIter);

struct PrintParams {
    s32 depth;
    u32 offset;
    PrintParams* parent;
};

void printByamlIter(const u8* data);
void printByamlIter(const ByamlIter& iter);
void printByamlIter_(const ByamlIter& iter, PrintParams* param);
}  // namespace al
