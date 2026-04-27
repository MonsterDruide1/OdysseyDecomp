#include "Scene/ProjectCameraPoserFactory.h"

#include "Library/Camera/CameraPoserFix.h"
#include "Library/Camera/CameraPoserFixPoint.h"
#include "Library/Camera/CreateCameraPoserFunction.h"
#include "Library/Play/Camera/CameraPoserBossBattle.h"
#include "Library/Play/Camera/CameraPoserEntrance.h"
#include "Library/Play/Camera/CameraPoserKinopioBrigade.h"
#include "Library/Play/Camera/CameraPoserLookBoard.h"
#include "Library/Play/Camera/CameraPoserLookDown.h"
#include "Library/Play/Camera/CameraPoserRace.h"
#include "Library/Play/Camera/CameraPoserRailMoveLookAt.h"
#include "Library/Play/Camera/CameraPoserRailMoveMovie.h"
#include "Library/Play/Camera/CameraPoserSubjective.h"
#include "Library/Play/Camera/CameraPoserTalk.h"
#include "Library/Play/Camera/CameraPoserTower.h"
#include "Library/Play/Camera/KeyMoveCameraFix.h"
#include "Library/Play/Camera/KeyMoveCameraRailMove.h"
#include "Library/Play/Camera/KeyMoveCameraZoom.h"

#include "Camera/CameraPoserFollowLimit.h"

const al::NameToCreator<al::CameraPoserCreatorFunction> sProjectCameraPoserFactoryEntries1[] = {
    {"制限付きフォロー", al::createCameraPoserFunction<CameraPoserFollowLimit>},
    {"制限付き平行", al::createCameraPoserFunction<CameraPoserFollowLimit>},
    {"2D平行", al::createCameraPoserFunction<CameraPoserFollowLimit>},
    {"固定", al::createCameraPoserFunction<al::CameraPoserFix>},
    {"完全固定", al::createCameraPoserFunction<al::CameraPoserFix>},
    {"出入口専用固定", al::createCameraPoserFunction<al::CameraPoserFix>},
    {"定点", al::createCameraPoserFunction<al::CameraPoserFixPoint>},
    {"その場定点", al::createCameraPoserFunction<al::CameraPoserFixPoint>},
    {"完全追従定点", al::createCameraPoserFunction<al::CameraPoserFixPoint>},
    {"レース", al::createCameraPoserFunction<al::CameraPoserRace>},
    {"レール移動", al::createCameraPoserFunction<al::CameraPoserRailMoveLookAt>},
    {"キノピオ探検隊", al::createCameraPoserFunction<al::CameraPoserKinopioBrigade>},
    {"会話用2点間", al::createCameraPoserFunction<al::CameraPoserTalk>},
    {"映像撮影レール", al::createCameraPoserFunction<al::CameraPoserRailMoveMovie>}};

const al::NameToCreator<al::CameraPoserCreatorFunction> sProjectCameraPoserFactoryEntries2[] = {
    {"ボス戦カメラ", al::createCameraPoserFunction<al::CameraPoserBossBattle>},
    {"スタート", al::createCameraPoserFunction<al::CameraPoserEntrance>},
    {"看板用2点間", al::createCameraPoserFunction<al::CameraPoserLookBoard>},
    {"見下ろし", al::createCameraPoserFunction<al::CameraPoserLookDown>},
    {"主観", al::createCameraPoserFunction<al::CameraPoserSubjective>},
    {"塔", al::createCameraPoserFunction<al::CameraPoserTower>},
    {"キー移動固定", al::createCameraPoserFunction<al::KeyMoveCameraFix>},
    {"キー移動レール移動", al::createCameraPoserFunction<al::KeyMoveCameraRailMove>},
    {"キー移動ズーム", al::createCameraPoserFunction<al::KeyMoveCameraZoom>},
    /* ScenarioStartCameraPoserSimpleZoom */ {"シナリオ紹介シンプルズームカメラ", nullptr},
    /* ScenarioStartCameraPoserRailMove */ {"シナリオ紹介レール移動カメラ", nullptr}};

ProjectCameraPoserFactory::ProjectCameraPoserFactory()
    : CameraPoserFactory("プロジェクトカメラファクトリー") {
    alCameraPoserFactoryFunction::initAndCreateTableFromOtherTable(
        this, sProjectCameraPoserFactoryEntries1, sProjectCameraPoserFactoryEntries2);
}
