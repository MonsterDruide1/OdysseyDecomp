#pragma once

#include "al/byaml/ByamlIter.h"
#include "sead/seadVector.h"

namespace alByamlLocalUtil
{
    const char* getDataTypeString(int);
};

namespace al
{
    bool tryGetByamlU8(unsigned char *, const al::ByamlIter &, const char *);
    bool tryGetByamlU16(unsigned short *, const al::ByamlIter &, const char *);
    bool tryGetByamlS16(short *, const al::ByamlIter &, const char *);
    bool tryGetByamlS32(int *, const al::ByamlIter &, const char *);
    bool tryGetByamlU32(unsigned int *, const al::ByamlIter &, const char *);
    bool tryGetByamlS64(long *, const al::ByamlIter &, const char *);
    bool tryGetByamlU64(unsigned long *, const al::ByamlIter &, const char *);
    bool tryGetByamlF32(float *, const al::ByamlIter &, const char *);
    bool tryGetByamlV2f(sead::Vector2<float> *, const al::ByamlIter &);
    bool tryGetByamlV3f(sead::Vector3<float> *, const al::ByamlIter &);
    bool tryGetByamlV4f(sead::Vector4<float> *, const al::ByamlIter &);
    bool tryGetByamlScale(sead::Vector3<float> *, const al::ByamlIter &);
    bool tryGetByamlV2s32(sead::Vector2<int> *, const al::ByamlIter &);
    bool tryGetByamlV3s32(sead::Vector3<int> *, const al::ByamlIter &);
};