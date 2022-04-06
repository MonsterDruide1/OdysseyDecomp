#pragma once

#include <gfx/seadColor.h>
#include <math/seadBoundBox.h>
#include <math/seadVector.h>
#include "al/byaml/ByamlData.h"
#include "al/byaml/ByamlIter.h"

namespace sead {
class WriteStream;
}

namespace al {
bool tryGetByamlU8(u8*, const ByamlIter&, const char*);
bool tryGetByamlU16(u16*, const ByamlIter&, const char*);
bool tryGetByamlS16(s16*, const ByamlIter&, const char*);
bool tryGetByamlS32(s32*, const ByamlIter&, const char*);
bool tryGetByamlU32(u32*, const ByamlIter&, const char*);
bool tryGetByamlS64(s64*, const ByamlIter&, const char*);
bool tryGetByamlU64(u64*, const ByamlIter&, const char*);
bool tryGetByamlF32(float*, const ByamlIter&, const char*);
bool tryGetByamlV2f(sead::Vector2f*, const ByamlIter&);
bool tryGetByamlV3f(sead::Vector3f*, const ByamlIter&);
bool tryGetByamlV4f(sead::Vector4f*, const ByamlIter&);
bool tryGetByamlScale(sead::Vector3f*, const ByamlIter&);
bool tryGetByamlV2s32(sead::Vector2i*, const ByamlIter&);
bool tryGetByamlV3s32(sead::Vector3i*, const ByamlIter&);
bool tryGetByamlBox3f(sead::BoundBox3f*, const ByamlIter&);
bool tryGetByamlV3f(sead::Vector3f*, const ByamlIter&, const char*);
bool tryGetByamlV2f(sead::Vector2f*, const ByamlIter&, const char*);
bool tryGetByamlV4f(sead::Vector4f*, const ByamlIter&, const char*);
bool tryGetByamlScale(sead::Vector3f*, const ByamlIter&, const char*);
bool tryGetByamlV2s32(sead::Vector2i*, const ByamlIter&, const char*);
bool tryGetByamlV3s32(sead::Vector3i*, const ByamlIter&, const char*);
bool tryGetByamlBox3f(sead::BoundBox3f*, const ByamlIter&, const char*);
bool tryGetByamlString(const char**, const ByamlIter&, const char*);
bool tryGetByamlColor(sead::Color4f*, const ByamlIter&);
bool tryGetByamlColor(sead::Color4f*, const ByamlIter&, const char*);
bool tryGetByamlBool(bool*, const ByamlIter&, const char*);
const char* tryGetByamlKeyStringOrNULL(const ByamlIter&, const char*);
s32 tryGetByamlKeyIntOrZero(const ByamlIter&, const char*);
u32 tryGetByamlKeyU32OrZero(const ByamlIter&, const char*);
float tryGetByamlKeyFloatOrZero(const ByamlIter&, const char*);
bool tryGetByamlKeyBoolOrFalse(const ByamlIter&, const char*);
bool tryGetByamlIterByKey(ByamlIter*, const ByamlIter&, const char*);
bool tryGetByamlKeyAndIntByIndex(const char**, int*, const ByamlIter&, int);

const char* getByamlKeyString(const ByamlIter&, const char*);
s32 getByamlKeyInt(const ByamlIter&, const char*);
float getByamlKeyFloat(const ByamlIter&, const char*);
bool getByamlKeyBool(const ByamlIter&, const char*);
void getByamlIterByKey(ByamlIter*, const ByamlIter&, const char*);
void getByamlIterByIndex(ByamlIter*, const ByamlIter&, int);

template <ByamlDataType T>
bool isTypeByIndex(const ByamlIter& rIter, int index) {
    ByamlData data;
    if (rIter.getByamlDataByIndex(&data, index)) {
        return data.getType() == T;
    }
    return false;
}
template <ByamlDataType T>
bool isTypeByKey(const ByamlIter& rIter, const char* key) {
    ByamlData data;
    if (rIter.getByamlDataByKey(&data, key)) {
        return data.getType() == T;
    }
    return false;
}
bool isTypeBoolByIndex(const ByamlIter&, int);
bool isTypeBoolByKey(const ByamlIter&, const char*);
bool isTypeIntByIndex(const ByamlIter&, int);
bool isTypeIntByKey(const ByamlIter&, const char*);
bool isTypeFloatByIndex(const ByamlIter&, int);
bool isTypeFloatByKey(const ByamlIter&, const char*);
bool isTypeStringByIndex(const ByamlIter&, int);
bool isTypeStringByKey(const ByamlIter&, const char*);
bool isTypeArrayByIndex(const ByamlIter&, int);
bool isTypeArrayByKey(const ByamlIter&, const char*);
bool isTypeHashByIndex(const ByamlIter&, int);
bool isTypeHashByKey(const ByamlIter&, const char*);

int getByamlIterDataNum(const ByamlIter&);

struct PrintParams {
    int depth;
    u32 offset;
    PrintParams* parent;
};

void printByamlIter(const u8*);
void printByamlIter(const ByamlIter&);
void printByamlIter_(const ByamlIter&, PrintParams*);
};  // namespace al
