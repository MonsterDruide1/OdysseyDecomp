#include "Library/Yaml/ByamlUtil.h"
#include "Library/Yaml/ByamlData.h"
#include "Library/Yaml/ByamlIter.h"
#include "math/seadBoundBox.h"

namespace al {
bool tryGetByamlU8(u8* pOut, const ByamlIter& rIter, const char* pKey) {
    int value = 0;
    if (rIter.tryGetIntByKey(&value, pKey)) {
        *pOut = value;
        return true;
    }
    return false;
}

bool tryGetByamlU16(u16* pOut, const ByamlIter& rIter, const char* pKey) {
    int value = 0;
    if (rIter.tryGetIntByKey(&value, pKey)) {
        *pOut = value;
        return true;
    }
    return false;
}

bool tryGetByamlS16(s16* pOut, const ByamlIter& rIter, const char* pKey) {
    int value = 0;
    if (rIter.tryGetIntByKey(&value, pKey)) {
        *pOut = value;
        return true;
    }
    return false;
}

bool tryGetByamlS32(s32* pOut, const ByamlIter& rIter, const char* pKey) {
    return rIter.tryGetIntByKey(pOut, pKey);
}

bool tryGetByamlU32(u32* pOut, const ByamlIter& rIter, const char* pKey) {
    int value = 0;
    bool res = rIter.tryGetIntByKey(&value, pKey);
    if (res) {
        *pOut = value;
    }
    return res;
}

bool tryGetByamlS64(s64* pOut, const ByamlIter& rIter, const char* pKey) {
    return rIter.tryGetInt64ByKey(pOut, pKey);
}

bool tryGetByamlU64(u64* pOut, const ByamlIter& rIter, const char* pKey) {
    u64 value = 0;
    bool res = rIter.tryGetUInt64ByKey(&value, pKey);
    if (res) {
        *pOut = value;
    }
    return res;
}

bool tryGetByamlF32(float* pOut, const ByamlIter& rIter, const char* pKey) {
    float value = 0;
    if (rIter.tryGetFloatByKey(&value, pKey)) {
        *pOut = value;
        return true;
    }
    return false;
}

bool tryGetByamlV2f(sead::Vector2f* pOut, const ByamlIter& rIter) {
    float x = 0;
    bool x_ret = rIter.tryGetFloatByKey(&x, "X");
    float y = 0;
    bool y_ret = rIter.tryGetFloatByKey(&y, "Y");

    *pOut = {x, y};
    return x_ret || y_ret;
}

bool tryGetByamlV3f(sead::Vector3f* pOut, const ByamlIter& rIter) {
    bool result = false;
    float x = 0;
    result = rIter.tryGetFloatByKey(&x, "X");
    float y = 0;
    result |= rIter.tryGetFloatByKey(&y, "Y");
    float z = 0;
    result |= rIter.tryGetFloatByKey(&z, "Z");

    *pOut = {x, y, z};
    return result;
}

bool tryGetByamlV4f(sead::Vector4f* pOut, const ByamlIter& rIter) {
    bool result = false;
    float x = 0;
    result = rIter.tryGetFloatByKey(&x, "X");
    float y = 0;
    result |= rIter.tryGetFloatByKey(&y, "Y");
    float z = 0;
    result |= rIter.tryGetFloatByKey(&z, "Z");
    float w = 0;
    result |= rIter.tryGetFloatByKey(&w, "W");

    *pOut = {x, y, z, w};
    return result;
}

bool tryGetByamlScale(sead::Vector3f* pOut, const ByamlIter& rIter) {
    bool result = false;
    float x = 1;
    result = rIter.tryGetFloatByKey(&x, "X");
    float y = 1;
    result |= rIter.tryGetFloatByKey(&y, "Y");
    float z = 1;
    result |= rIter.tryGetFloatByKey(&z, "Z");

    *pOut = {x, y, z};
    return result;
}

bool tryGetByamlV2s32(sead::Vector2i* pOut, const ByamlIter& rIter) {
    s32 x = 0;
    bool x_ret = rIter.tryGetIntByKey(&x, "X");
    s32 y = 0;
    bool y_ret = rIter.tryGetIntByKey(&y, "Y");

    *pOut = {x, y};
    return x_ret || y_ret;
}

bool tryGetByamlV3s32(sead::Vector3i* pOut, const ByamlIter& rIter) {
    bool result = false;
    s32 x = 0;
    result = rIter.tryGetIntByKey(&x, "X");
    s32 y = 0;
    result |= rIter.tryGetIntByKey(&y, "Y");
    s32 z = 0;
    result |= rIter.tryGetIntByKey(&z, "Z");

    *pOut = {x, y, z};
    return result;
}

bool tryGetByamlBox3f(sead::BoundBox3f* pOut, const ByamlIter& rIter) {
    sead::Vector3f min, max;
    if (!tryGetByamlV3f(&min, rIter, "Min"))
        return false;
    if (!tryGetByamlV3f(&max, rIter, "Max"))
        return false;
    *pOut = {min, max};
    return true;
}

bool tryGetByamlV3f(sead::Vector3f* pOut, const ByamlIter& sIter, const char* key) {
    ByamlIter rIter;
    if (!sIter.tryGetIterByKey(&rIter, key))
        return false;

    return tryGetByamlV3f(pOut, rIter);
}

bool tryGetByamlV2f(sead::Vector2f* pOut, const ByamlIter& sIter, const char* key) {
    ByamlIter rIter;
    if (!sIter.tryGetIterByKey(&rIter, key))
        return false;

    return tryGetByamlV2f(pOut, rIter);
}

bool tryGetByamlV4f(sead::Vector4f* pOut, const ByamlIter& sIter, const char* key) {
    ByamlIter rIter;
    if (!sIter.tryGetIterByKey(&rIter, key))
        return false;

    return tryGetByamlV4f(pOut, rIter);
}

bool tryGetByamlScale(sead::Vector3f* pOut, const ByamlIter& sIter, const char* key) {
    ByamlIter rIter;
    if (!sIter.tryGetIterByKey(&rIter, key))
        return false;

    return tryGetByamlScale(pOut, rIter);
}

bool tryGetByamlV2s32(sead::Vector2i* pOut, const ByamlIter& sIter, const char* key) {
    ByamlIter rIter;
    if (!sIter.tryGetIterByKey(&rIter, key))
        return false;

    return tryGetByamlV2s32(pOut, rIter);
}

bool tryGetByamlV3s32(sead::Vector3i* pOut, const ByamlIter& sIter, const char* key) {
    ByamlIter rIter;
    if (!sIter.tryGetIterByKey(&rIter, key))
        return false;

    return tryGetByamlV3s32(pOut, rIter);
}

bool tryGetByamlBox3f(sead::BoundBox3f* pOut, const ByamlIter& sIter, const char* key) {
    ByamlIter rIter;
    if (!sIter.tryGetIterByKey(&rIter, key))
        return false;

    return tryGetByamlBox3f(pOut, rIter);
}

bool tryGetByamlString(const char** pOut, const ByamlIter& rIter, const char* key) {
    return rIter.tryGetStringByKey(pOut, key);
}

bool tryGetByamlColor(sead::Color4f* pOut, const ByamlIter& rIter) {
    bool result = false;
    float r = 0;
    result = rIter.tryGetFloatByKey(&r, "R");
    float g = 0;
    result |= rIter.tryGetFloatByKey(&g, "G");
    float b = 0;
    result |= rIter.tryGetFloatByKey(&b, "B");
    float a = 0;
    result |= rIter.tryGetFloatByKey(&a, "A");

    *pOut = {r, g, b, a};
    return result;
}

bool tryGetByamlColor(sead::Color4f* pOut, const ByamlIter& sIter, const char* key) {
    ByamlIter rIter;
    if (!sIter.tryGetIterByKey(&rIter, key))
        return false;

    return tryGetByamlColor(pOut, rIter);
}

bool tryGetByamlBool(bool* pOut, const ByamlIter& rIter, const char* key) {
    return rIter.tryGetBoolByKey(pOut, key);
}

const char* tryGetByamlKeyStringOrNULL(const ByamlIter& rIter, const char* key) {
    const char* val = nullptr;
    if (rIter.tryGetStringByKey(&val, key))
        return val;
    return nullptr;
}

s32 tryGetByamlKeyIntOrZero(const ByamlIter& rIter, const char* key) {
    int val = 0;
    if (rIter.tryGetIntByKey(&val, key))
        return val;
    return 0;
}

u32 tryGetByamlKeyU32OrZero(const ByamlIter& rIter, const char* key) {
    u32 val = 0;
    if (rIter.tryGetUIntByKey(&val, key))
        return val;
    return 0;
}

float tryGetByamlKeyFloatOrZero(const ByamlIter& rIter, const char* key) {
    float val = 0;
    if (rIter.tryGetFloatByKey(&val, key))
        return val;
    return 0;
}

bool tryGetByamlKeyBoolOrFalse(const ByamlIter& rIter, const char* key) {
    bool val = false;
    if (rIter.tryGetBoolByKey(&val, key))
        return val;
    return false;
}

bool tryGetByamlIterByKey(ByamlIter* pOut, const ByamlIter& rIter, const char* key) {
    return rIter.tryGetIterByKey(pOut, key);
}

bool tryGetByamlKeyAndIntByIndex(const char** key, int* pOut, const ByamlIter& rIter, int index) {
    ByamlData data;
    if (!rIter.getByamlDataAndKeyName(&data, key, index))
        return false;
    if (!rIter.tryConvertInt(pOut, &data))
        return false;
    return true;
}

const char* getByamlKeyString(const ByamlIter& rIter, const char* key) {
    return tryGetByamlKeyStringOrNULL(rIter, key);
}
s32 getByamlKeyInt(const ByamlIter& rIter, const char* key) {
    return tryGetByamlKeyIntOrZero(rIter, key);
}
float getByamlKeyFloat(const ByamlIter& rIter, const char* key) {
    return tryGetByamlKeyFloatOrZero(rIter, key);
}
bool getByamlKeyBool(const ByamlIter& rIter, const char* key) {
    return tryGetByamlKeyBoolOrFalse(rIter, key);
}

void getByamlIterByKey(ByamlIter* pOut, const ByamlIter& rIter, const char* pKey) {
    rIter.tryGetIterByKey(pOut, pKey);
}

void getByamlIterByIndex(ByamlIter* pOut, const ByamlIter& rIter, int index) {
    rIter.tryGetIterByIndex(pOut, index);
}

bool isTypeBoolByIndex(const ByamlIter& rIter, int index) {
    return isTypeByIndex<ByamlDataType::TYPE_BOOL>(rIter, index);
}
bool isTypeBoolByKey(const ByamlIter& rIter, const char* pKey) {
    return isTypeByKey<ByamlDataType::TYPE_BOOL>(rIter, pKey);
}

bool isTypeIntByIndex(const ByamlIter& rIter, int index) {
    return isTypeByIndex<ByamlDataType::TYPE_INT>(rIter, index);
}
bool isTypeIntByKey(const ByamlIter& rIter, const char* pKey) {
    return isTypeByKey<ByamlDataType::TYPE_INT>(rIter, pKey);
}

bool isTypeFloatByIndex(const ByamlIter& rIter, int index) {
    return isTypeByIndex<ByamlDataType::TYPE_FLOAT>(rIter, index);
}
bool isTypeFloatByKey(const ByamlIter& rIter, const char* pKey) {
    return isTypeByKey<ByamlDataType::TYPE_FLOAT>(rIter, pKey);
}

bool isTypeStringByIndex(const ByamlIter& rIter, int index) {
    return isTypeByIndex<ByamlDataType::TYPE_STRING>(rIter, index);
}
bool isTypeStringByKey(const ByamlIter& rIter, const char* pKey) {
    return isTypeByKey<ByamlDataType::TYPE_STRING>(rIter, pKey);
}

bool isTypeArrayByIndex(const ByamlIter& rIter, int index) {
    return isTypeByIndex<ByamlDataType::TYPE_ARRAY>(rIter, index);
}
bool isTypeArrayByKey(const ByamlIter& rIter, const char* pKey) {
    return isTypeByKey<ByamlDataType::TYPE_ARRAY>(rIter, pKey);
}

bool isTypeHashByIndex(const ByamlIter& rIter, int index) {
    return isTypeByIndex<ByamlDataType::TYPE_HASH>(rIter, index);
}
bool isTypeHashByKey(const ByamlIter& rIter, const char* pKey) {
    return isTypeByKey<ByamlDataType::TYPE_HASH>(rIter, pKey);
}

int getByamlIterDataNum(const ByamlIter& rIter) {
    return rIter.getSize();
}

void printByamlIter(const u8* data) {
    ByamlIter iter = {data};
    printByamlIter(iter);
}
void printByamlIter(const ByamlIter& iter) {
    u32 data_off = iter.getHeader()->getDataOffset();
    PrintParams param = {0, data_off, nullptr};
    printByamlIter_(iter, &param);
}
void printByamlIter_(const ByamlIter& iter, PrintParams* param) {
    int size = iter.getSize();
    for (int i = 0; i < size; ++i) {
        ByamlData entry;
        if (iter.isTypeArray()) {
            iter.getByamlDataByIndex(&entry, i);
        } else if (iter.isTypeHash()) {
            const char* data = nullptr;
            iter.getByamlDataAndKeyName(&entry, &data, i);
        }
        if (entry.getType() == ByamlDataType::TYPE_HASH ||
            entry.getType() == ByamlDataType::TYPE_ARRAY) {
            u32 v6 = entry.getValue();
            if (param) {
                PrintParams* sub_param = param;
                while (sub_param) {
                    if (sub_param->offset == v6)
                        goto loop;
                    sub_param = sub_param->parent;
                }
            }
            ByamlIter a1 = {};
            if (iter.tryGetIterByIndex(&a1, i)) {
                int depth = param->depth + 1;
                u32 value = entry.getValue();
                PrintParams new_param = {depth, value, param};
                printByamlIter_(a1, &new_param);
            }
        } else {
            u8 type = entry.getType();
            if (type == ByamlDataType::TYPE_FLOAT) {
                float data;
                iter.tryConvertFloat(&data, &entry);
            } else if (type == ByamlDataType::TYPE_INT) {
                int data;
                iter.tryConvertInt(&data, &entry);
            } else if (type == ByamlDataType::TYPE_STRING) {
                const char* data;
                iter.tryConvertString(&data, &entry);
            } else if (type == ByamlDataType::TYPE_BOOL) {
                bool data;
                iter.tryConvertBool(&data, &entry);
            }
        }
    loop:;
    }
}
}  // namespace al
