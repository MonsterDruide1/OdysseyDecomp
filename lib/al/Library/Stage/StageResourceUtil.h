#pragma once

#include <prim/seadSafeString.h>

namespace al {

void makeStageDataArchivePath(sead::BufferedSafeString* archivePath, const char* stageName,
                              const char* resourceType);

}
