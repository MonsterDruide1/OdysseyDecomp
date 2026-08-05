#pragma once

#include "Library/Message/ReplaceTagProcessorBase.h"
#include "Library/Scene/IUseSceneObjHolder.h"

class ProjectReplaceTagProcessor : public al::ReplaceTagProcessorBase {
public:
    ProjectReplaceTagProcessor(const al::IUseSceneObjHolder*);

    s32 replaceProjectTag(char16*, const al::MessageTag&,
                          const al::IUseMessageSystem*) const override;

private:
    const al::IUseSceneObjHolder* mSceneObjHolder;
};

static_assert(sizeof(ProjectReplaceTagProcessor) == 0x10);
