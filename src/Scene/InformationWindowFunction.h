#pragma once

#include "Library/Base/StringUtil.h"

namespace InformationWindowFunction {
bool isExistMovieFile(const char* name);
al::StringTmp<64> getMovieFilePath(const char* name);
}  // namespace InformationWindowFunction
