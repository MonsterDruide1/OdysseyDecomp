#pragma once

#include "Project/Anim/AnimPlayerSimple.h"

namespace al {
struct AnimPlayerInitInfo;
struct AnimResInfo;

class AnimPlayerMat : public AnimPlayerSimple {
public:
    static AnimPlayerMat* tryCreate(const AnimPlayerInitInfo*, int);

    virtual void init(const AnimPlayerInitInfo*);
    virtual void setAnimToModel(const AnimResInfo*);

private:
    int mMatType;
    void* _30;
    void* _38;
};

class AnimPlayerVis : public AnimPlayerSimple {
public:
    static AnimPlayerVis* tryCreate(const AnimPlayerInitInfo*, int);

    AnimPlayerVis();

    virtual void init(const AnimPlayerInitInfo*);
    virtual void setAnimToModel(const AnimResInfo*);
};
}  // namespace al
