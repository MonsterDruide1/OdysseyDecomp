#pragma once

#include "Library/Model/ModelDrawerBase.h"

namespace al {
class ModelCtrl;

class ModelDrawerForward : public ModelDrawerBase {
public:
    ModelDrawerForward(const char* name, bool, bool, bool, bool, bool, bool);

    void createTable() override;
    void draw() const override;
    void addModel(ModelCtrl* model) override;
    void removeModel(ModelCtrl* model) override;

private:
    bool _50;
    bool _51;
    bool _52;
    bool _53;
    bool _54;
    bool _55;
    bool _56;
    bool _57;
};

static_assert(sizeof(ModelDrawerForward) == 0x58);

}  // namespace al
