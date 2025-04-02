#pragma once

#include <prim/seadSafeString.h>

namespace al {
class HtmlViewer;
}
class GameDataHolderAccessor;

namespace HelpFunction {
void callHelp(const al::HtmlViewer*, GameDataHolderAccessor, bool, bool, bool,
              sead::BufferedSafeString*);
}
