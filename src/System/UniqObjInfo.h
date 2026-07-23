#pragma once

#include <prim/seadSafeString.h>

namespace al {
class PlacementInfo;
}

struct UniqObjInfo {
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

    sead::FixedSafeString<128> stageName;
    sead::FixedSafeString<128> objId;
};

static_assert(sizeof(UniqObjInfo) == 0x130);
