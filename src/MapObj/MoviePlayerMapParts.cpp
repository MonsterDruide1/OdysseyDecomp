#include "MapObj/MoviePlayerMapParts.h"

#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Library/Shader/ForwardRendering/ShaderSamplerSetter.h"
#include "Library/Texture/TextureUtil.h"
#include "Util/MoviePlayer.h"

MoviePlayerMapParts::MoviePlayerMapParts(const char* name) : al::LiveActor(name) {}

void MoviePlayerMapParts::init(const al::ActorInitInfo& info) {
    al::initMapPartsActor(this, info, nullptr);
    mMoviePlayer = al::createSceneObj<MoviePlayer>(this);
    mMovieTextureReplacer = new al::TextureReplacer(mMoviePlayer->getTexture());
    mWhiteTextureReplacer = new al::TextureReplacer(al::getWhite2DTexture());
    mWhiteTextureReplacer->replace(this, "Snow_Cake-Snow_Cake.png", "Snow_Cake");

    mMoviePath.format("content:/MovieData/SnowWithShip.mp4");
    mSwitchMoviePath.format("content:/MovieData/SnowWithShip.mp4");

    const char* movieName = nullptr;
    if (al::tryGetStringArg(&movieName, info, "MovieName"))
        mMoviePath.format("content:/MovieData/%s.mp4", movieName);

    const char* switchMovieName = nullptr;
    if (al::tryGetStringArg(&switchMovieName, info, "SwitchMovieName"))
        mSwitchMoviePath.format("content:/MovieData/%s.mp4", switchMovieName);

    al::registActorToDemoInfo(this, info);
    al::trySyncStageSwitchAppearAndKill(this);
}

bool MoviePlayerMapParts::tryPlayMovie() {
    if (mMoviePlayer->isPlay() && !mMoviePlayer->isLooped())
        return false;

    const char* moviePath = nullptr;
    if (al::isValidStageSwitch(this, "SwitchMovieChange") &&
        al::isOnStageSwitch(this, "SwitchMovieChange")) {
        moviePath = mSwitchMoviePath.cstr();
    } else {
        moviePath = mMoviePath.cstr();
    }

    if (mCurrentMoviePath == moviePath)
        return false;

    mIsNeedTextureUpdate = true;
    mCurrentMoviePath = moviePath;
    mMoviePlayer->play(moviePath);
    mWhiteTextureReplacer->replace(this, "Snow_Cake-Snow_Cake.png", "Snow_Cake");
    mWhiteTextureReplacer->update();
    al::recreateModelDisplayList(this);
    return true;
}

void MoviePlayerMapParts::appear() {
    mCurrentMoviePath = nullptr;
    tryPlayMovie();
    al::LiveActor::appear();
}

void MoviePlayerMapParts::kill() {
    mIsNeedTextureUpdate = false;
    mMoviePlayer->stop();
    al::LiveActor::kill();
}

void MoviePlayerMapParts::startClipped() {
    mIsNeedTextureUpdate = false;
    mMoviePlayer->stop();
    al::LiveActor::startClipped();
}

void MoviePlayerMapParts::endClipped() {
    mCurrentMoviePath = nullptr;
    tryPlayMovie();
    al::LiveActor::endClipped();
}

void MoviePlayerMapParts::control() {
    tryPlayMovie();
    mMoviePlayer->update();

    if (!mMoviePlayer->isDecode())
        return;

    if (mIsNeedTextureUpdate) {
        mMovieTextureReplacer->replace(this, "Snow_Cake-Snow_Cake.png", "Snow_Cake");
        al::recreateModelDisplayList(this);
        mIsNeedTextureUpdate = false;
    }

    mMovieTextureReplacer->update();
}
