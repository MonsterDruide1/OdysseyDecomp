#include "al/util/RandomUtil.h"

#include <random/seadGlobalRandom.h>

namespace al {

    float getRandom(){
        unsigned int random = (sead::GlobalRandom::instance()->getU32() >> 9) | 0x3F800000;
        return (*reinterpret_cast<float*>(&random)) - 1;
    }
    float getRandom(float factor){
        return getRandom(0.f, factor);
    }
    float getRandom(float min, float max){
        return (getRandom()*(max-min)) + min;
    }

    int getRandom(int factor){
        return getRandom(0, factor);
    }
    int getRandom(int min, int max){
        return (int)getRandom((float)min, (float)max);
    }

    float getRandomDegree(){
        return getRandom(360.f);
    }
    float getRandomRadian(){
        return getRandom(6.2832f);
    }

    void getRandomVector(sead::Vector3f* vec, float factor){
        float x = (getRandom()*(factor+factor)) - factor;
        float y = (getRandom()*(factor+factor)) - factor;
        float z = (getRandom()*(factor+factor)) - factor;
        vec->x = x;
        vec->y = y;
        vec->z = z;
    }

    void getRandomDir(sead::Vector3f* vec){
        getRandomVector(vec, 10.f);
        while(sead::dot(*vec, *vec) > 0.000001f)
            getRandomVector(vec, 10.f);
        vec->normalize();
    }

}
