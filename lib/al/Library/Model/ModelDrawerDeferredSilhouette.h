#pragma once

#include "Library/Model/ModelDrawerBase.h"

namespace al {
enum class SilhouetteDrawCategory : u64 {
    Default,
    Ride,
};

class ModelDrawerDeferredSilhouette : public ModelDrawerBase {
public:
    ModelDrawerDeferredSilhouette(const char* name, SilhouetteDrawCategory category);

    void createTable() override;
    void draw() const override;

private:
    char _50[0x10];
};

static_assert(sizeof(ModelDrawerDeferredSilhouette) == 0x60);

}  // namespace al
