#include "Scene/ProjectCameraPoserFactory.h"

// TODO remove pragma once it is used
#include "Library/Camera/CreateCameraPoserFunction.h" // IWYU pragma: keep
#include "Library/Factory/Factory.h"

const al::NameToCreator<al::CameraPoserCreatorFunction> sProjectCameraPoserFactoryEntries1[] = {
    /* CameraPoserFollowLimit */ {"制限付きフォロー", nullptr},
    /* CameraPoserFollowLimit */ {"制限付き平行", nullptr},
    /* CameraPoserFollowLimit */ {"2D平行", nullptr},
    /* al::CameraPoserFix */ {"固定", nullptr},
    /* al::CameraPoserFix */ {"完全固定", nullptr},
    /* al::CameraPoserFix */ {"出入口専用固定", nullptr},
    /* al::CameraPoserFixPoint */ {"定点", nullptr},
    /* al::CameraPoserFixPoint */ {"その場定点", nullptr},
    /* al::CameraPoserFixPoint */ {"完全追従定点", nullptr},
    /* al::CameraPoserRace */ {"レース", nullptr},
    /* al::CameraPoserRailMoveLookAt */ {"レール移動", nullptr},
    /* al::CameraPoserKinopioBrigade */ {"キノピオ探検隊", nullptr},
    /* al::CameraPoserTalk */ {"会話用2点間", nullptr},
    /* al::CameraPoserRailMoveMovie */ {"映像撮影レール", nullptr}};

const al::NameToCreator<al::CameraPoserCreatorFunction> sProjectCameraPoserFactoryEntries2[] = {
    /* al::CameraPoserBossBattle */ {"ボス戦カメラ", nullptr},
    /* al::CameraPoserEntrance */ {"スタート", nullptr},
    /* al::CameraPoserLookBoard */ {"看板用2点間", nullptr},
    /* al::CameraPoserLookDown */ {"見下ろし", nullptr},
    /* al::CameraPoserSubjective */ {"主観", nullptr},
    /* al::CameraPoserTower */ {"塔", nullptr},
    /* al::KeyMoveCameraFix */ {"キー移動固定", nullptr},
    /* al::KeyMoveCameraRailMove */ {"キー移動レール移動", nullptr},
    /* al::KeyMoveCameraZoom */ {"キー移動ズーム", nullptr},
    /* ScenarioStartCameraPoserSimpleZoom */ {"シナリオ紹介シンプルズームカメラ", nullptr},
    /* ScenarioStartCameraPoserRailMove */ {"シナリオ紹介レール移動カメラ", nullptr}};

ProjectCameraPoserFactory::ProjectCameraPoserFactory()
    : CameraPoserFactory("プロジェクトカメラファクトリー") {
    alCameraPoserFactoryFunction::initAndCreateTableFromOtherTable(
        this, sProjectCameraPoserFactoryEntries1, sProjectCameraPoserFactoryEntries2);
}
