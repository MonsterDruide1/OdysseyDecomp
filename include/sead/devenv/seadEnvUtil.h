#pragma once

#include "prim/seadSafeString.h"

namespace sead
{
class EnvUtil
{
public:
    static const SafeString& getRomType();
    static s32 getEnvironmentVariable(BufferedSafeString* out, const SafeString& variable);
    static s32 resolveEnvronmentVariable(BufferedSafeString* out, const SafeString& str);
};
}  // namespace sead
