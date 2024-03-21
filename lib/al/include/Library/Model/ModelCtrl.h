#pragma once

#include <math/seadMatrix.h>

namespace al {
class Resource;

class ModelCtrl {
public:
    ModelCtrl();
    ~ModelCtrl();

    void initResource(Resource*, Resource*);
    void show();
    void hide();
};
}  // namespace al
