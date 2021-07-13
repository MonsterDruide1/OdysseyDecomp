#include "al/util/StringUtil.h"
#include <cstring>

namespace al
{
    bool isEqualString(const char16_t *pString_0, const char16_t *pString_1)
    {
        unsigned short val;

        while (1)
        {
            val = *pString_0;
            if (val != *pString_1)
            {
                break;
            }

            ++pString_1;
            ++pString_0;

            if (!val)
            {
                return true;
            }
        }

        return false;
    }

    bool isEqualSubString(const char *pString_0, const char *pString_1)
    {
        return strstr(pString_0, pString_1);
    }

    /*bool isEqualSubString(const sead::SafeStringBase<char> &pString_0, const sead::SafeStringBase<char> &pString_1)
    {
        pString_0.assureTerminationImpl_();
        const char* str = pString_0.mStringTop;
        pString_1.assureTerminationImpl_();
        return strstr(str, pString_1.mStringTop);
    }*/

    bool isStartWithString(const char *pString_0, const char *pString_1)
    {
        char val = *pString_1;

        if (!*pString_1)
        {
            return true;
        }

        const char* nextVal = pString_1 + 1;

        while(*pString_0 && *pString_0 == val)
        {
            char e = *nextVal++;
            val = e;
            ++pString_0;

            if (!e)
            {
                return true;
            }
        }

        return false;
    }

    bool isEndWithString(const char *pString_0, const char *pString_1)
    {
        int pString0_Len = strlen(pString_0);
        int pString1_Len = strlen(pString_1);

        if (pString0_Len < pString1_Len)
        {
            return false;
        }
        else
        {
            return isEqualString(&pString_0[pString0_Len - pString1_Len], pString_1);
        }
    }

    bool isEqualString(const char *pString_0, const char *pString_1)
    {
        char val;

        while (1)
        {
            val = *pString_0;
            if (val != *pString_1)
            {
                break;
            }

            ++pString_1;
            ++pString_0;

            if (!val)
            {
                return true;
            }
        }

        return false;
    }
};