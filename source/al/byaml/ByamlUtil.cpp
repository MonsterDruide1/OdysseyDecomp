#include "al/byaml/ByamlUtil.h"

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