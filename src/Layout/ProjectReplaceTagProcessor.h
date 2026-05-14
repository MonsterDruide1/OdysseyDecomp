#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseMessageSystem;
class IUseSceneObjHolder;
class MessageTag;

class ReplaceTagProcessorBase {
public:
    virtual s32 replacePictureGroup(char16* out, const MessageTag& tag) const;
};
}  // namespace al

class ProjectReplaceTagProcessor : public al::ReplaceTagProcessorBase {
public:
    explicit ProjectReplaceTagProcessor(const al::IUseSceneObjHolder* user);

private:
    const al::IUseSceneObjHolder* mSceneObjHolder = nullptr;
};

static_assert(sizeof(ProjectReplaceTagProcessor) == 0x10);
