#pragma once

#include <prim/seadSafeString.h>

namespace al {
class PlacementInfo;
}

class UniqObjInfo {
public:
    void set(const char* stage_name, const char* obj_id);
    void set(const sead::BufferedSafeString& stage_name, const sead::BufferedSafeString& obj_id);
    void set(const char* stage_name, const al::PlacementInfo* placement_info);
    void clear();
    bool isEmpty() const;
    bool isEqual(const char* stage_name, const char* obj_id) const;
    bool isEqual(const sead::BufferedSafeString& stage_name,
                 const sead::BufferedSafeString& obj_id) const;
    bool isEqual(const UniqObjInfo& other) const;
    void print() const;
    void fillDummyData();

    const char* getStageName() const { return mStageName.cstr(); }

    const char* getObjId() const { return mObjId.cstr(); }

private:
    sead::FixedSafeString<128> mStageName;
    sead::FixedSafeString<128> mObjId;
};

static_assert(sizeof(UniqObjInfo) == 0x130);
