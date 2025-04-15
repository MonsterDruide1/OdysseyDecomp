#include "System/UniqObjInfo.h"

#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"

void UniqObjInfo::set(const char* stage_name, const char* obj_id) {
    mStageName.format("%s", stage_name);
    mObjId.format("%s", obj_id);
}

void UniqObjInfo::set(const sead::BufferedSafeString& stage_name,
                      const sead::BufferedSafeString& obj_id) {
    set(stage_name.cstr(), obj_id.cstr());
}

void UniqObjInfo::set(const char* stage_name, const al::PlacementInfo* placement_info) {
    al::PlacementId placement_id;
    al::getPlacementId(&placement_id, *placement_info);
    set(stage_name, al::makeStringPlacementId(&placement_id).cstr());
}

void UniqObjInfo::clear() {
    mStageName.clear();
    mObjId.clear();
}

bool UniqObjInfo::isEmpty() const {
    return mStageName.isEmpty();
}

bool UniqObjInfo::isEqual(const char* stage_name, const char* obj_id) const {
    return al::isEqualString(stage_name, mStageName.cstr()) &&
           al::isEqualString(obj_id, mObjId.cstr());
}

bool UniqObjInfo::isEqual(const sead::BufferedSafeString& stage_name,
                          const sead::BufferedSafeString& obj_id) const {
    return isEqual(stage_name.cstr(), obj_id.cstr());
}

bool UniqObjInfo::isEqual(const UniqObjInfo& other) const {
    return isEqual(other.mStageName, other.mObjId);
}

void UniqObjInfo::print() const {}

void UniqObjInfo::fillDummyData() {
    clear();
    for (s32 i = 0; i < mStageName.getBufferSize(); i++)
        mStageName.append(al::getRandom(1));
    for (s32 i = 0; i < mObjId.getBufferSize(); i++)
        mObjId.append(al::getRandom(1));
}
