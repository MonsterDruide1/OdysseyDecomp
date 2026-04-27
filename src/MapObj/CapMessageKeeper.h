#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseSceneObjHolder;
class Resource;
}  // namespace al

class CapMessageEnableChecker;

class CapMessageKeeper {
public:
    struct CapMsgType {
        struct ValueArray {
            ValueArray();
        };

        static const char* text_(s32 value);

        s32 mValue = 0;
    };

    CapMessageKeeper();

    void createAndReadYamlAll();
    bool isShowCapMsg(const al::IUseSceneObjHolder* sceneObjHolder, s32 id);
    bool isShowCapMsgCurrentWorld(const al::IUseSceneObjHolder* sceneObjHolder,
                                  const CapMsgType& type);
    bool tryShowSaveCapMsg(const al::IUseSceneObjHolder* sceneObjHolder, const CapMsgType& type,
                           bool isSave);
    bool tryShowCapMsgPrivate(const al::IUseSceneObjHolder* sceneObjHolder, const CapMsgType& type,
                              const char* messageName, s32 id, bool isDemo, bool isSave);
    bool tryCheckShowCapMsg(const al::IUseSceneObjHolder* sceneObjHolder, const CapMsgType& type,
                            CapMessageEnableChecker* enableChecker, bool isSave);
    void saveCapMsg(const al::IUseSceneObjHolder* sceneObjHolder, s32 id);
    bool tryShowCapMsgPriorityLow(const al::IUseSceneObjHolder* sceneObjHolder,
                                  const CapMsgType& type, s32 delayTime, s32 waitTime);
    bool tryReadYamlOne(const al::Resource* resource, const char* messageName, s32* delayTime,
                        s32* waitTime);
    bool tryShowCapMsgCurrentWorld(const al::IUseSceneObjHolder* sceneObjHolder,
                                   const CapMsgType& type);
    void saveCapMsgCurrentWorld(const al::IUseSceneObjHolder* sceneObjHolder,
                                const CapMsgType& type);
    void saveCapMsg(const al::IUseSceneObjHolder* sceneObjHolder, const char* messageName);
    s32* getCapMessageParam(const CapMsgType& type);

private:
    s32** mCapMessageParamTable = nullptr;
    s32 mDefaultDelay = -1;
    s32 mDefaultWaitFrame = -1;
    void* mParamArray = nullptr;
    s32 mUnused = 0;
    u32 _1c = 0;
};

static_assert(sizeof(CapMessageKeeper) == 0x20, "CapMessageKeeper");

namespace CapMessageKeeperFuncrion {
CapMessageKeeper* getCapMsgKeeper(const al::IUseSceneObjHolder* sceneObjHolder);
}  // namespace CapMessageKeeperFuncrion
