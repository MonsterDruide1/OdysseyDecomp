#pragma once

#include "Library/Model/ModelDrawerBase.h"

namespace al {
class ModelCtrl;

class ModelDrawerDeferred : public ModelDrawerBase {
public:
    ModelDrawerDeferred(const char* name, bool, bool, bool, bool);

    void createTable() override;
    void draw() const override;
    void addModel(ModelCtrl* model) override;
    void removeModel(ModelCtrl* model) override;

private:
    bool _50;
    bool _51;
    bool _52;
    bool _53;
};

static_assert(sizeof(ModelDrawerDeferred) == 0x58);

}  // namespace al
