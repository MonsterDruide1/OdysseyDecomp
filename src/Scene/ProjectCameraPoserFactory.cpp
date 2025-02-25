#include "Scene/ProjectCameraPoserFactory.h"

#include "Library/Camera/CreateCameraPoserFunction.h"

// clang-format off
const al::NameToCreator<al::CameraPoserCreatorFunction> sProjectCameraPoserFactoryEntries1[] = {
    {"制限付きフォロー", nullptr},	// CameraPoserFollowLimit
    {"制限付き平行", nullptr}, 	// CameraPoserFollowLimit
    {"2D平行", nullptr},      	// CameraPoserFollowLimit
    {"固定", nullptr},         	// al::CameraPoserFix
    {"完全固定", nullptr},      	// al::CameraPoserFix
    {"出入口専用固定", nullptr}, // al::CameraPoserFix
    {"定点", nullptr},         	// al::CameraPoserFixPoint
    {"その場定点", nullptr},    	// al::CameraPoserFixPoint
    {"完全追従定点", nullptr},  	// al::CameraPoserFixPoint
    {"レース", nullptr},        	// al::CameraPoserRace
    {"レール移動", nullptr},     	// al::CameraPoserRailMoveLookAt
    {"キノピオ探検隊", nullptr}, 	// al::CameraPoserKinopioBrigade
    {"会話用2点間", nullptr},   	// al::CameraPoserTalk
    {"映像撮影レール", nullptr} 	// al::CameraPoserRailMoveMovie
};

const al::NameToCreator<al::CameraPoserCreatorFunction> sProjectCameraPoserFactoryEntries2[] = {
    {"ボス戦カメラ", nullptr},              	// al::CameraPoserBossBattle
    {"スタート", nullptr},                		// al::CameraPoserEntrance
    {"看板用2点間", nullptr},          		// al::CameraPoserLookBoard
    {"見下ろし", nullptr},                	// al::CameraPoserLookDown
    {"主観", nullptr},                 		// al::CameraPoserSubjective
    {"塔", nullptr},                   		// al::CameraPoserTower
    {"キー移動固定", nullptr},             	// al::KeyMoveCameraFix
    {"キー移動レール移動", nullptr},          	// al::KeyMoveCameraRailMove
    {"キー移動ズーム", nullptr},             	// al::KeyMoveCameraZoom
    {"シナリオ紹介シンプルズームカメラ", nullptr},	// ScenarioStartCameraPoserSimpleZoom
    {"シナリオ紹介レール移動カメラ", nullptr}		// ScenarioStartCameraPoserRailMove
};
// clang-format on

ProjectCameraPoserFactory::ProjectCameraPoserFactory()
    : CameraPoserFactory("プロジェクトカメラファクトリー") {
    alCameraPoserFactoryFunction::initAndCreateTableFromOtherTable(
        this, sProjectCameraPoserFactoryEntries1, sProjectCameraPoserFactoryEntries2);
}
