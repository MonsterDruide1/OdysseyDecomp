#include "Project/Memory/SceneHeapSetter.h"

#include <heap/seadHeapMgr.h>

#include "Library/System/SystemKit.h"
#include "Project/Memory/MemorySystem.h"

#include "System/ProjectInterface.h"

namespace al {
SceneHeapSetter::SceneHeapSetter()
    : sead::ScopedCurrentHeapSetter(
          alProjectInterface::getSystemKit()->getMemorySystem()->getSceneHeap()),
      mSceneHeap(alProjectInterface::getSystemKit()->getMemorySystem()->getSceneHeap()) {}
}  // namespace al
