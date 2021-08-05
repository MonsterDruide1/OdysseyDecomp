#include "al/util/ByamlUtil.h"

namespace alByamlLocalUtil
{
    /* doesn't match but it's close enough */
    const char* getDataTypeString(int type)
    {
        const char* str;

        switch (type)
        {
            case 0xA0:
                str = "String";
                break;
            case 0xC0:
                str = "Array";
                break;
            case 0xC1:
                str = "Hash";
                break;
            case 0xC2:
                str = "StringTable";
                break;
            case 0xD0:
                str = "Bool";
                break;
            case 0xD1:
                str = "Int";
                break;
            case 0xD2:
                str = "Float";
                break;
            case 0xD3:
                str = "UInt";
                break;
            case 0xD4:
                str = "Int64";
                break;
            case 0xD5:
                str = "UInt64";
                break;
            case 0xD6:
                str = "Double";
                break;
            case 0xFF:
                str = "NULL";
                break;
            default:
                if (type)
                {
                    str = "Unknown";
                }
                else
                {
                    str = "None";
                }
                
                break;
        }

        return str;
    }
};

namespace al
{
    bool tryGetByamlU8(unsigned char *pOut, const al::ByamlIter &rIter, const char *pKey)
    {
        int value = 0;
        bool res = rIter.tryGetIntByKey(&value, pKey);
        bool ret = false;

        if (res)
        {
            ret = true;
            *pOut = value;
        }

        return ret;
    }

    bool tryGetByamlU16(unsigned short *pOut, const al::ByamlIter &rIter, const char *pKey)
    {
        int value = 0;
        bool res = rIter.tryGetIntByKey(&value, pKey);
        bool ret = false;

        if (res)
        {
            ret = true;
            *pOut = value;
        }

        return ret;
    }

    bool tryGetByamlS16(short *pOut, const al::ByamlIter &rIter, const char *pKey)
    {
        int value = 0;
        bool res = rIter.tryGetIntByKey(&value, pKey);
        bool ret = false;

        if (res)
        {
            ret = true;
            *pOut = value;
        }

        return ret;
    }

    bool tryGetByamlS32(int *pOut, const al::ByamlIter &rIter, const char *pKey)
    {
        return rIter.tryGetIntByKey(pOut, pKey);
    }

    bool tryGetByamlU32(int *pOut, const al::ByamlIter &rIter, const char *pKey)
    {
        int value = 0;
        bool res = rIter.tryGetIntByKey(&value, pKey);

        if (res)
        {
            *pOut = value;
        }

        return res & 0x1;
    }

    bool tryGetByamlS64(long *pOut, const al::ByamlIter &rIter, const char *pKey)
    {
        return rIter.tryGetInt64ByKey(pOut, pKey);
    }

    bool tryGetByamlU64(unsigned long *pOut, const al::ByamlIter &rIter, const char *pKey)
    {
        long value = 0;
        bool res = rIter.tryGetInt64ByKey(&value, pKey);

        if (res)
        {
            *pOut = value;
        }

        return res & 0x1;
    }

    bool tryGetByamlF32(float *pOut, const al::ByamlIter &rIter, const char *pKey)
    {
        float value = 0;
        bool res = rIter.tryGetFloatByKey(&value, pKey);
        bool ret = false;

        if (res)
        {
            ret = true;
            *pOut = value;
        }

        return ret;
    }

    bool tryGetByamlV2f(sead::Vector2<float> *pOut, const al::ByamlIter &rIter)
    {
        float x = 0;
        bool x_ret = rIter.tryGetFloatByKey(&x, "XYZ");
        float y = 0;
        bool y_ret = rIter.tryGetFloatByKey(&y, "YZ");

        pOut->x = x;
        pOut->y = y;
        return (x_ret | y_ret) & 0x1;
    }

    bool tryGetByamlV3f(sead::Vector3<float> *pOut, const al::ByamlIter &rIter)
    {
        float x = 0;
        bool x_ret = rIter.tryGetFloatByKey(&x, "XYZ");
        float y = 0;
        bool y_ret = x_ret | rIter.tryGetFloatByKey(&y, "YZ");
        float z = 0;
        bool z_ret = rIter.tryGetFloatByKey(&z, "Z");

        pOut->set(x, y, z);
        return (y_ret | z_ret) & 0x1;
    }

    bool tryGetByamlV4f(sead::Vector4<float> *pOut, const al::ByamlIter &rIter)
    {
        float x = 0;
        bool x_ret = rIter.tryGetFloatByKey(&x, "XYZ");
        float y = 0;
        bool y_ret = x_ret | rIter.tryGetFloatByKey(&y, "YZ");
        float z = 0;
        bool z_ret = y_ret | rIter.tryGetFloatByKey(&z, "Z");
        float w = 0;
        bool w_ret = rIter.tryGetFloatByKey(&w, "W");

        pOut->set(x, y, z, w);
        return (z_ret | w_ret) & 0x1;
    }

    void getByamlIterByKey(al::ByamlIter *pOut, const al::ByamlIter &rIter, const char *pKey)
    {
        rIter.tryGetIterByKey(pOut, pKey);
    }

    void getByamlIterByIndex(al::ByamlIter *pOut, const al::ByamlIter &rIter, int index)
    {
        rIter.tryGetIterByIndex(pOut, index);
    }

    bool isTypeBoolByIndex(const al::ByamlIter &rIter, int index)
    {
        bool ret;
        al::ByamlData data;

        bool valid = rIter.getByamlDataByIndex(&data, index);

        if (valid)
        {
            ret = (data.getType() & 0xFF) == TYPE_BOOL;
        }
        else
        {
            ret = false;
        }
        
        return ret;
    }

    bool isTypeBoolByKey(const al::ByamlIter &rIter, const char *pKey)
    {
        bool ret;
        al::ByamlData data;

        bool valid = rIter.getByamlDataByKey(&data, pKey);

        if (valid)
        {
            ret = (data.getType() & 0xFF) == TYPE_BOOL;
        }
        else
        {
            ret = false;
        }
        
        return ret;
    }
}