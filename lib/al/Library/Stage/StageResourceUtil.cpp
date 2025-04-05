#include "Library/Stage/StageResourceUtil.h"

namespace al {

void makeStageDataArchivePath(sead::BufferedSafeString* archivePath, const char* stageName,
                              const char* resourceType) {
    archivePath->format("StageData/%s%s", stageName, resourceType);
}

}  // namespace al
