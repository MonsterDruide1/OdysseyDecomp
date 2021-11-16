#pragma once

#include <math/seadVector.h>
#include "al/byaml/ByamlIter.h"

namespace sead {
class WriteStream;
}

namespace alByamlLocalUtil {
const char* getDataTypeString(int);
void writeU24(sead::WriteStream*, int);
};  // namespace alByamlLocalUtil

namespace al {
bool tryGetByamlU8(unsigned char*, const al::ByamlIter&, const char*);
bool tryGetByamlU16(unsigned short*, const al::ByamlIter&, const char*);
bool tryGetByamlS16(short*, const al::ByamlIter&, const char*);
bool tryGetByamlS32(int*, const al::ByamlIter&, const char*);
bool tryGetByamlU32(unsigned int*, const al::ByamlIter&, const char*);
bool tryGetByamlS64(long*, const al::ByamlIter&, const char*);
bool tryGetByamlU64(unsigned long*, const al::ByamlIter&, const char*);
bool tryGetByamlF32(float*, const al::ByamlIter&, const char*);
bool tryGetByamlV2f(sead::Vector2<float>*, const al::ByamlIter&);
bool tryGetByamlV3f(sead::Vector3<float>*, const al::ByamlIter&);
bool tryGetByamlV4f(sead::Vector4<float>*, const al::ByamlIter&);
bool tryGetByamlScale(sead::Vector3<float>*, const al::ByamlIter&);
bool tryGetByamlV2s32(sead::Vector2<int>*, const al::ByamlIter&);
bool tryGetByamlV3s32(sead::Vector3<int>*, const al::ByamlIter&);

void getByamlIterByKey(al::ByamlIter*, const al::ByamlIter&, const char*);
void getByamlIterByIndex(al::ByamlIter*, const al::ByamlIter&, int);
bool isTypeBoolByIndex(const al::ByamlIter&, int);
bool isTypeBoolByKey(const al::ByamlIter&, const char*);
bool isTypeIntByIndex(const al::ByamlIter&, int);
bool isTypeIntByKey(const al::ByamlIter&, const char*);
bool isTypeFloatByIndex(const al::ByamlIter&, int);
bool isTypeFloatByKey(const al::ByamlIter&, const char*);
bool isTypeStringByIndex(const al::ByamlIter&, int);
bool isTypeStringByKey(const al::ByamlIter&, const char*);
bool isTypeArrayByIndex(const al::ByamlIter&, int);
bool isTypeArrayByKey(const al::ByamlIter&, const char*);
bool isTypeHashByIndex(const al::ByamlIter&, int);
bool isTypeHashByKey(const al::ByamlIter&, const char*);
};  // namespace al