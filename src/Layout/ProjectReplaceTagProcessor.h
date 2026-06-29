#pragma once

#include <basis/seadTypes.h>
#include <cstdarg>

namespace al {
class IUseMessageSystem;
class IUseSceneObjHolder;
class MessageTag;

class ReplaceTagProcessorBase {
public:
    virtual s32 replacePictureGroup(char16* dst, const MessageTag& messageTag) const;
    virtual s32 replaceNumberGroup(char16* dst, const MessageTag& messageTag,
                                   std::va_list args) const;
    virtual s32 replaceStringGroup(char16* dst, const MessageTag& messageTag,
                                   std::va_list args) const;
    virtual s32 replaceTagLabel(char16* dst, const MessageTag& messageTag) const;
    virtual s32 replaceProjectTag(char16* dst, const MessageTag& messageTag,
                                  const IUseMessageSystem* messageSystem) const;
};

static_assert(sizeof(ReplaceTagProcessorBase) == 0x8);
}  // namespace al

class ProjectReplaceTagProcessor : public al::ReplaceTagProcessorBase {
public:
    explicit ProjectReplaceTagProcessor(const al::IUseSceneObjHolder* sceneObjHolder);

    s32 replaceProjectTag(char16* dst, const al::MessageTag& messageTag,
                          const al::IUseMessageSystem* messageSystem) const override;

private:
    const al::IUseSceneObjHolder* mSceneObjHolder = nullptr;
};

static_assert(sizeof(ProjectReplaceTagProcessor) == 0x10);
