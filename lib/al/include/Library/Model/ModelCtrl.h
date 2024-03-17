#pragma once

#include <math/seadMatrix.h>

namespace al {
class Resource;

class ModelCtrl {
public:
    ModelCtrl();
    ~ModelCtrl();

    void initialize(nn::g3d::ResModel*, int, int, sead::Head*, ShaderHolder);
    void initResource(Resource*, Resource*);
    void show();
    void hide();
    
    sead::Matrix34f* mMatrix;
};
}
