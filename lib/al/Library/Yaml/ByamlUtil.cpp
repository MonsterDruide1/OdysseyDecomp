#include "Library/Yaml/ByamlUtil.h"

#include "Library/Yaml/ByamlData.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {
bool tryGetByamlU8(u8* out, const ByamlIter& iter, const char* key) {
    s32 value = 0;
    if (iter.tryGetIntByKey(&value, key)) {
        *out = value;
        return true;
    }
    return false;
}

bool tryGetByamlU16(u16* out, const ByamlIter& iter, const char* key) {
    s32 value = 0;
    if (iter.tryGetIntByKey(&value, key)) {
        *out = value;
        return true;
    }
    return false;
}

bool tryGetByamlS16(s16* out, const ByamlIter& iter, const char* key) {
    s32 value = 0;
    if (iter.tryGetIntByKey(&value, key)) {
        *out = value;
        return true;
    }
    return false;
}

bool tryGetByamlS32(s32* out, const ByamlIter& iter, const char* key) {
    return iter.tryGetIntByKey(out, key);
}

bool tryGetByamlU32(u32* out, const ByamlIter& iter, const char* key) {
    s32 value = 0;
    bool res = iter.tryGetIntByKey(&value, key);
    if (res)
        *out = value;
    return res;
}

bool tryGetByamlS64(s64* out, const ByamlIter& iter, const char* key) {
    return iter.tryGetInt64ByKey(out, key);
}

bool tryGetByamlU64(u64* out, const ByamlIter& iter, const char* key) {
    u64 value = 0;
    bool res = iter.tryGetUInt64ByKey(&value, key);
    if (res)
        *out = value;
    return res;
}

bool tryGetByamlF32(f32* out, const ByamlIter& iter, const char* key) {
    f32 value = 0;
    if (iter.tryGetFloatByKey(&value, key)) {
        *out = value;
        return true;
    }
    return false;
}

bool tryGetByamlV2f(sead::Vector2f* out, const ByamlIter& iter) {
    f32 x = 0;
    bool x_ret = iter.tryGetFloatByKey(&x, "X");
    f32 y = 0;
    bool y_ret = iter.tryGetFloatByKey(&y, "Y");

    *out = {x, y};
    return x_ret || y_ret;
}

bool tryGetByamlV3f(sead::Vector3f* out, const ByamlIter& iter) {
    bool result = false;
    f32 x = 0;
    result = iter.tryGetFloatByKey(&x, "X");
    f32 y = 0;
    result |= iter.tryGetFloatByKey(&y, "Y");
    f32 z = 0;
    result |= iter.tryGetFloatByKey(&z, "Z");

    *out = {x, y, z};
    return result;
}

bool tryGetByamlV4f(sead::Vector4f* out, const ByamlIter& iter) {
    bool result = false;
    f32 x = 0;
    result = iter.tryGetFloatByKey(&x, "X");
    f32 y = 0;
    result |= iter.tryGetFloatByKey(&y, "Y");
    f32 z = 0;
    result |= iter.tryGetFloatByKey(&z, "Z");
    f32 w = 0;
    result |= iter.tryGetFloatByKey(&w, "W");

    *out = {x, y, z, w};
    return result;
}

bool tryGetByamlScale(sead::Vector3f* out, const ByamlIter& iter) {
    bool result = false;
    f32 x = 1;
    result = iter.tryGetFloatByKey(&x, "X");
    f32 y = 1;
    result |= iter.tryGetFloatByKey(&y, "Y");
    f32 z = 1;
    result |= iter.tryGetFloatByKey(&z, "Z");

    *out = {x, y, z};
    return result;
}

bool tryGetByamlV2s32(sead::Vector2i* out, const ByamlIter& iter) {
    s32 x = 0;
    bool x_ret = iter.tryGetIntByKey(&x, "X");
    s32 y = 0;
    bool y_ret = iter.tryGetIntByKey(&y, "Y");

    *out = {x, y};
    return x_ret || y_ret;
}

bool tryGetByamlV3s32(sead::Vector3i* out, const ByamlIter& iter) {
    bool result = false;
    s32 x = 0;
    result = iter.tryGetIntByKey(&x, "X");
    s32 y = 0;
    result |= iter.tryGetIntByKey(&y, "Y");
    s32 z = 0;
    result |= iter.tryGetIntByKey(&z, "Z");

    *out = {x, y, z};
    return result;
}

bool tryGetByamlBox3f(sead::BoundBox3f* out, const ByamlIter& iter) {
    sead::Vector3f min, max;
    if (!tryGetByamlV3f(&min, iter, "Min"))
        return false;
    if (!tryGetByamlV3f(&max, iter, "Max"))
        return false;
    *out = {min, max};
    return true;
}

bool tryGetByamlV3f(sead::Vector3f* out, const ByamlIter& iter, const char* key) {
    ByamlIter child;
    if (!iter.tryGetIterByKey(&child, key))
        return false;

    return tryGetByamlV3f(out, child);
}

bool tryGetByamlV2f(sead::Vector2f* out, const ByamlIter& iter, const char* key) {
    ByamlIter child;
    if (!iter.tryGetIterByKey(&child, key))
        return false;

    return tryGetByamlV2f(out, child);
}

bool tryGetByamlV4f(sead::Vector4f* out, const ByamlIter& iter, const char* key) {
    ByamlIter child;
    if (!iter.tryGetIterByKey(&child, key))
        return false;

    return tryGetByamlV4f(out, child);
}

bool tryGetByamlScale(sead::Vector3f* out, const ByamlIter& iter, const char* key) {
    ByamlIter child;
    if (!iter.tryGetIterByKey(&child, key))
        return false;

    return tryGetByamlScale(out, child);
}

bool tryGetByamlV2s32(sead::Vector2i* out, const ByamlIter& iter, const char* key) {
    ByamlIter child;
    if (!iter.tryGetIterByKey(&child, key))
        return false;

    return tryGetByamlV2s32(out, child);
}

bool tryGetByamlV3s32(sead::Vector3i* out, const ByamlIter& iter, const char* key) {
    ByamlIter child;
    if (!iter.tryGetIterByKey(&child, key))
        return false;

    return tryGetByamlV3s32(out, child);
}

bool tryGetByamlBox3f(sead::BoundBox3f* out, const ByamlIter& iter, const char* key) {
    ByamlIter child;
    if (!iter.tryGetIterByKey(&child, key))
        return false;

    return tryGetByamlBox3f(out, child);
}

bool tryGetByamlString(const char** out, const ByamlIter& iter, const char* key) {
    return iter.tryGetStringByKey(out, key);
}

bool tryGetByamlColor(sead::Color4f* out, const ByamlIter& iter) {
    bool result = false;
    f32 r = 0;
    result = iter.tryGetFloatByKey(&r, "R");
    f32 g = 0;
    result |= iter.tryGetFloatByKey(&g, "G");
    f32 b = 0;
    result |= iter.tryGetFloatByKey(&b, "B");
    f32 a = 0;
    result |= iter.tryGetFloatByKey(&a, "A");

    *out = {r, g, b, a};
    return result;
}

bool tryGetByamlColor(sead::Color4f* out, const ByamlIter& iter, const char* key) {
    ByamlIter child;
    if (!iter.tryGetIterByKey(&child, key))
        return false;

    return tryGetByamlColor(out, child);
}

bool tryGetByamlBool(bool* out, const ByamlIter& iter, const char* key) {
    return iter.tryGetBoolByKey(out, key);
}

const char* tryGetByamlKeyStringOrNULL(const ByamlIter& iter, const char* key) {
    const char* val = nullptr;
    if (iter.tryGetStringByKey(&val, key))
        return val;
    return nullptr;
}

s32 tryGetByamlKeyIntOrZero(const ByamlIter& iter, const char* key) {
    s32 val = 0;
    if (iter.tryGetIntByKey(&val, key))
        return val;
    return 0;
}

u32 tryGetByamlKeyU32OrZero(const ByamlIter& iter, const char* key) {
    u32 val = 0;
    if (iter.tryGetUIntByKey(&val, key))
        return val;
    return 0;
}

f32 tryGetByamlKeyFloatOrZero(const ByamlIter& iter, const char* key) {
    f32 val = 0;
    if (iter.tryGetFloatByKey(&val, key))
        return val;
    return 0;
}

bool tryGetByamlKeyBoolOrFalse(const ByamlIter& iter, const char* key) {
    bool val = false;
    if (iter.tryGetBoolByKey(&val, key))
        return val;
    return false;
}

bool tryGetByamlIterByKey(ByamlIter* out, const ByamlIter& iter, const char* key) {
    return iter.tryGetIterByKey(out, key);
}

bool tryGetByamlKeyAndIntByIndex(const char** key, s32* out, const ByamlIter& iter, s32 index) {
    ByamlData data;
    if (!iter.getByamlDataAndKeyName(&data, key, index))
        return false;
    if (!iter.tryConvertInt(out, &data))
        return false;
    return true;
}

const char* getByamlKeyString(const ByamlIter& iter, const char* key) {
    return tryGetByamlKeyStringOrNULL(iter, key);
}

s32 getByamlKeyInt(const ByamlIter& iter, const char* key) {
    return tryGetByamlKeyIntOrZero(iter, key);
}

f32 getByamlKeyFloat(const ByamlIter& iter, const char* key) {
    return tryGetByamlKeyFloatOrZero(iter, key);
}

bool getByamlKeyBool(const ByamlIter& iter, const char* key) {
    return tryGetByamlKeyBoolOrFalse(iter, key);
}

void getByamlIterByKey(ByamlIter* out, const ByamlIter& iter, const char* key) {
    iter.tryGetIterByKey(out, key);
}

void getByamlIterByIndex(ByamlIter* out, const ByamlIter& iter, s32 index) {
    iter.tryGetIterByIndex(out, index);
}

bool isTypeBoolByIndex(const ByamlIter& iter, s32 index) {
    return isTypeByIndex<ByamlDataType::Bool>(iter, index);
}

bool isTypeBoolByKey(const ByamlIter& iter, const char* key) {
    return isTypeByKey<ByamlDataType::Bool>(iter, key);
}

bool isTypeIntByIndex(const ByamlIter& iter, s32 index) {
    return isTypeByIndex<ByamlDataType::Int>(iter, index);
}

bool isTypeIntByKey(const ByamlIter& iter, const char* key) {
    return isTypeByKey<ByamlDataType::Int>(iter, key);
}

bool isTypeFloatByIndex(const ByamlIter& iter, s32 index) {
    return isTypeByIndex<ByamlDataType::Float>(iter, index);
}

bool isTypeFloatByKey(const ByamlIter& iter, const char* key) {
    return isTypeByKey<ByamlDataType::Float>(iter, key);
}

bool isTypeStringByIndex(const ByamlIter& iter, s32 index) {
    return isTypeByIndex<ByamlDataType::String>(iter, index);
}

bool isTypeStringByKey(const ByamlIter& iter, const char* key) {
    return isTypeByKey<ByamlDataType::String>(iter, key);
}

bool isTypeArrayByIndex(const ByamlIter& iter, s32 index) {
    return isTypeByIndex<ByamlDataType::Array>(iter, index);
}

bool isTypeArrayByKey(const ByamlIter& iter, const char* key) {
    return isTypeByKey<ByamlDataType::Array>(iter, key);
}

bool isTypeHashByIndex(const ByamlIter& iter, s32 index) {
    return isTypeByIndex<ByamlDataType::Hash>(iter, index);
}

bool isTypeHashByKey(const ByamlIter& iter, const char* key) {
    return isTypeByKey<ByamlDataType::Hash>(iter, key);
}

s32 getByamlIterDataNum(const ByamlIter& iter) {
    return iter.getSize();
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
    s32 size = iter.getSize();
    for (s32 i = 0; i < size; ++i) {
        ByamlData entry;
        if (iter.isTypeArray()) {
            iter.getByamlDataByIndex(&entry, i);
        } else if (iter.isTypeHash()) {
            const char* data = nullptr;
            iter.getByamlDataAndKeyName(&entry, &data, i);
        }
        if (entry.getType() == ByamlDataType::Hash || entry.getType() == ByamlDataType::Array) {
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
                s32 depth = param->depth + 1;
                u32 value = entry.getValue();
                PrintParams new_param = {depth, value, param};
                printByamlIter_(a1, &new_param);
            }
        } else {
            ByamlDataType type = entry.getType();
            if (type == ByamlDataType::Float) {
                f32 data;
                iter.tryConvertFloat(&data, &entry);
            } else if (type == ByamlDataType::Int) {
                s32 data;
                iter.tryConvertInt(&data, &entry);
            } else if (type == ByamlDataType::String) {
                const char* data;
                iter.tryConvertString(&data, &entry);
            } else if (type == ByamlDataType::Bool) {
                bool data;
                iter.tryConvertBool(&data, &entry);
            }
        }
    loop:;
    }
}
}  // namespace al
