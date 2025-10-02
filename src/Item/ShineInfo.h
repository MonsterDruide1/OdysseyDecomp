#pragma once

#include <prim/seadSafeString.h>

namespace al {
class IUseMessageSystem;
class LiveActor;
struct ActorInitInfo;
}  // namespace al
class QuestInfo;
class UniqObjInfo;

struct ShineData {
    char stageName[128];
    char objId[128];
};

class ShineInfo {
public:
    enum Type {
        Type_Shine,
        Type_GrandShine,
    };

    ShineInfo();
    void clear();
    void init(const char* stage_name, const char* obj_id, const char* label,
              const QuestInfo* quest_info, s32 type);
    void initForHintPhoto(const al::LiveActor* actor, const al::IUseMessageSystem*,
                          const al::ActorInitInfo& info);
    bool isEmpty() const;
    bool isEqual(const ShineInfo* other) const;
    bool isEqual(const ShineData* other) const;
    void writeShineData(ShineData* data) const;
    void readShineData(const ShineData* data);
    void readShineData(const UniqObjInfo* obj_info);
    void copyShineInfo(const ShineInfo* src);

    const char* getStageName() const { return mStageName.cstr(); }

    const char* getObjId() const { return mObjId.cstr(); }

    Type getType() const { return mType; }

    void setType(Type type) { mType = type; }

    const QuestInfo* getQuestInfo() const { return mQuestInfo; }

    bool isShop() const { return mIsShop; }

    u64 getGetTime() const { return mGetTime; }

    void setGetTime(u64 get_time) { mGetTime = get_time; }

private:
    sead::FixedSafeString<128> mStageName;
    sead::FixedSafeString<128> mObjId;
    sead::FixedSafeString<128> mLabel;
    Type mType;
    const QuestInfo* mQuestInfo;
    bool mIsShop;
    u64 mGetTime;
};

static_assert(sizeof(ShineInfo) == 0x1e8);
