#include "al/util/ByamlUtil.h"

namespace alByamlLocalUtil {
/* doesn't match but it's close enough */
const char* getDataTypeString(int type) {
    switch (type) {
    case 0x00:
        return "None";
    case 0xA0:
        return "String";
    case 0xC0:
        return "Array";
    case 0xC1:
        return "Hash";
    case 0xC2:
        return "StringTable";
    case 0xD0:
        return "Bool";
    case 0xD1:
        return "Int";
    case 0xD2:
        return "Float";
    case 0xD3:
        return "UInt";
    case 0xD4:
        return "Int64";
    case 0xD5:
        return "UInt64";
    case 0xD6:
        return "Double";
    case 0xFF:
        return "NULL";
    default:
        return "Unknown";
    }
}
};  // namespace alByamlLocalUtil

namespace al {
bool tryGetByamlU8(unsigned char* pOut, const al::ByamlIter& rIter, const char* pKey) {
    int value = 0;
    bool res = rIter.tryGetIntByKey(&value, pKey);
    bool ret = false;

    if (res) {
        ret = true;
        *pOut = value;
    }

    return ret;
}

bool tryGetByamlU16(unsigned short* pOut, const al::ByamlIter& rIter, const char* pKey) {
    int value = 0;
    bool res = rIter.tryGetIntByKey(&value, pKey);
    bool ret = false;

    if (res) {
        ret = true;
        *pOut = value;
    }

    return ret;
}

bool tryGetByamlS16(short* pOut, const al::ByamlIter& rIter, const char* pKey) {
    int value = 0;
    bool res = rIter.tryGetIntByKey(&value, pKey);
    bool ret = false;

    if (res) {
        ret = true;
        *pOut = value;
    }

    return ret;
}

bool tryGetByamlS32(int* pOut, const al::ByamlIter& rIter, const char* pKey) {
    return rIter.tryGetIntByKey(pOut, pKey);
}

bool tryGetByamlU32(int* pOut, const al::ByamlIter& rIter, const char* pKey) {
    int value = 0;
    bool res = rIter.tryGetIntByKey(&value, pKey);

    if (res) {
        *pOut = value;
    }

    return res & 0x1;
}

bool tryGetByamlS64(long* pOut, const al::ByamlIter& rIter, const char* pKey) {
    return rIter.tryGetInt64ByKey(pOut, pKey);
}

bool tryGetByamlU64(unsigned long* pOut, const al::ByamlIter& rIter, const char* pKey) {
    long value = 0;
    bool res = rIter.tryGetInt64ByKey(&value, pKey);

    if (res) {
        *pOut = value;
    }

    return res & 0x1;
}

bool tryGetByamlF32(float* pOut, const al::ByamlIter& rIter, const char* pKey) {
    float value = 0;
    bool res = rIter.tryGetFloatByKey(&value, pKey);
    bool ret = false;

    if (res) {
        ret = true;
        *pOut = value;
    }

    return ret;
}

bool tryGetByamlV2f(sead::Vector2<float>* pOut, const al::ByamlIter& rIter) {
    float x = 0;
    bool x_ret = rIter.tryGetFloatByKey(&x, "XYZ");
    float y = 0;
    bool y_ret = rIter.tryGetFloatByKey(&y, "YZ");

    *pOut = {x, y};
    return (x_ret | y_ret) & 0x1;
}

bool tryGetByamlV3f(sead::Vector3<float>* pOut, const al::ByamlIter& rIter) {
    float x = 0;
    bool x_ret = rIter.tryGetFloatByKey(&x, "XYZ");
    float y = 0;
    bool y_ret = x_ret | rIter.tryGetFloatByKey(&y, "YZ");
    float z = 0;
    bool z_ret = rIter.tryGetFloatByKey(&z, "Z");

    *pOut = {x, y, z};
    return (y_ret | z_ret) & 0x1;
}

bool tryGetByamlV4f(sead::Vector4<float>* pOut, const al::ByamlIter& rIter) {
    float x = 0;
    bool x_ret = rIter.tryGetFloatByKey(&x, "XYZ");
    float y = 0;
    bool y_ret = x_ret | rIter.tryGetFloatByKey(&y, "YZ");
    float z = 0;
    bool z_ret = y_ret | rIter.tryGetFloatByKey(&z, "Z");
    float w = 0;
    bool w_ret = rIter.tryGetFloatByKey(&w, "W");

    *pOut = {x, y, z, w};
    return (z_ret | w_ret) & 0x1;
}

void getByamlIterByKey(al::ByamlIter* pOut, const al::ByamlIter& rIter, const char* pKey) {
    rIter.tryGetIterByKey(pOut, pKey);
}

void getByamlIterByIndex(al::ByamlIter* pOut, const al::ByamlIter& rIter, int index) {
    rIter.tryGetIterByIndex(pOut, index);
}

bool isTypeBoolByIndex(const al::ByamlIter& rIter, int index) {
    bool ret;
    al::ByamlData data;

    bool valid = rIter.getByamlDataByIndex(&data, index);

    if (valid) {
        ret = (data.getType() & 0xFF) == TYPE_BOOL;
    } else {
        ret = false;
    }

    return ret;
}

bool isTypeBoolByKey(const al::ByamlIter& rIter, const char* pKey) {
    bool ret;
    al::ByamlData data;

    bool valid = rIter.getByamlDataByKey(&data, pKey);

    if (valid) {
        ret = (data.getType() & 0xFF) == TYPE_BOOL;
    } else {
        ret = false;
    }

    return ret;
}
}  // namespace al