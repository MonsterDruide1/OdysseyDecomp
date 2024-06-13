#pragma once

#include "Project/Anim/AnimPlayerSimple.h"

namespace al {
struct AnimPlayerInitInfo;
struct AnimResInfo;

class AnimPlayerMat : public AnimPlayerSimple {
public:
    static AnimPlayerMat* tryCreate(const AnimPlayerInitInfo*, s32);

    void init(const AnimPlayerInitInfo*) override;
    void setAnimToModel(const AnimResInfo*) override;

private:
    s32 mMatType;
    void* _30;
    void* _38;
};

class AnimPlayerVis : public AnimPlayerSimple {
public:
    static AnimPlayerVis* tryCreate(const AnimPlayerInitInfo*, s32);

    AnimPlayerVis();

    void init(const AnimPlayerInitInfo*) override;
    void setAnimToModel(const AnimResInfo*) override;
};
}  // namespace al
