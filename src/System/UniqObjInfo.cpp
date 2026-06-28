#include "System/UniqObjInfo.h"

#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"

void UniqObjInfo::set(const char* stage_name, const char* obj_id) {
    stageName.format("%s", stage_name);
    objId.format("%s", obj_id);
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
    stageName.clear();
    objId.clear();
}

bool UniqObjInfo::isEmpty() const {
    return stageName.isEmpty();
}

bool UniqObjInfo::isEqual(const char* stage_name, const char* obj_id) const {
    return al::isEqualString(stage_name, stageName.cstr()) &&
           al::isEqualString(obj_id, objId.cstr());
}

bool UniqObjInfo::isEqual(const sead::BufferedSafeString& stage_name,
                          const sead::BufferedSafeString& obj_id) const {
    return isEqual(stage_name.cstr(), obj_id.cstr());
}

bool UniqObjInfo::isEqual(const UniqObjInfo& other) const {
    return isEqual(other.stageName, other.objId);
}

void UniqObjInfo::print() const {}

void UniqObjInfo::fillDummyData() {
    clear();
    for (s32 i = 0; i < stageName.getBufferSize(); i++)
        stageName.append(al::getRandom(1));
    for (s32 i = 0; i < objId.getBufferSize(); i++)
        objId.append(al::getRandom(1));
}
