#pragma once

#include <basis/seadTypes.h>

namespace al {
class ModelCtrl;
class MaterialCategoryKeeper;
class ByamlIter;
class Resource;

class ModelMaterialCategory {
public:
    static void tryCreate(ModelCtrl*, const Resource*, const char*, MaterialCategoryKeeper*);
    static void tryCreate(ModelCtrl*, const char*, MaterialCategoryKeeper*);

    ModelMaterialCategory(ModelCtrl*, MaterialCategoryKeeper*);
    void init(const char*);
    void init(const ByamlIter&);
    s32 getCategoryIdFromMaterialName(const char*) const;
    s32 getCategoryIdFromMaterialIndex(s32) const;
};

}  // namespace al
