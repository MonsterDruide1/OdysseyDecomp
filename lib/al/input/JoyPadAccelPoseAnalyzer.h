#pragma once

#include <math/seadVector.h>

namespace al {

class JoyPadAccelPoseAnalyzer {
public:
    class HistoryInfo {
    public:
        void calcHistory(sead::Vector3f const&, sead::Vector3f const&, float);

        unsigned char padding_0[0x60];  // 0x0 - 0x60
        float hist0;                    // 0x60
        unsigned char padding_1[0x70 - 0x64];
        float hist1;  // 0x70
        unsigned char padding_2[0x1c];
    };

    class PoseAxisDir {
    public:
        void calcHistory(sead::Vector3f const&, sead::Vector3f const&);

        unsigned char padding_1[0x88];
        sead::Vector2f unkVec0;  // 0x18
        unsigned char padding_0[0x98 - (0x88 + 0x8)];
    };

    bool isSwingAnyHand() const;
    bool isSwingDoubleHandSameDir() const;
    bool isSwingDoubleHand() const;
    bool isSwingDoubleHandReverseDir() const;
    bool isSwingDoubleHandReverseInsideDir() const;
    bool isSwingDoubleHandReverseOutsideDir() const;
    bool isSwingLeftHand() const;
    bool isSwingRightHand() const;
    void setSwingBorder(float, float);
    void update();
    JoyPadAccelPoseAnalyzer getSwingDirDoubleHandSameDir() const;

    int mControllerPort;               // 0x0 port of the controller
    int mAccelDeviceNum;               // 0x4 number of accelerometers
    bool gap00;                        // 0x8 unknown
    bool mSwingLeft;                   // 0x9 shaking the left joycon
    bool mSwingRight;                  // 0xa shaking the right joycon
    bool mSwingAny;                    // 0xb shaking anything
    sead::Vector2f mSwingBorder;       // 0xc Border to trigger a motion shake
    sead::Vector2f mAccelLeftVel;      // 0x14 Accelerometer of the Left Joycon
    sead::Vector2f mAccelRightVel;     // 0x1c Accelerometer of the Right Joycon
    sead::Vector2f mAccelCombinedVel;  // 0x24 Combined vectors of the left and right joycons
    sead::Vector2f mAccelLeftAccel;    // 0x2c Acceleration of the left joycon
    sead::Vector2f mAccelRightAccel;   // 0x34 Acceleration of the right joycon
    sead::Vector2f mGyroRoll;  // 0x3c Roll of the gyroscope, each component is left/right joycon.
    unsigned char padding_0[0x4];  // 0x44 padding because there is nothing here
    HistoryInfo mHistoryLeft;      // 0x48 History info of the left joycon
    HistoryInfo mHistoryRight;     // 0xd8 History info of the right joycon
    bool gap01;                    // 0x168 unknown bool
    unsigned char padding_1[0x7];  // 0x16a padding because there is nothing here
    PoseAxisDir unknown0;          // 0x170
    PoseAxisDir unknown1;          // 0x208
    PoseAxisDir unknown2;          // 0x2a0
};

}  // namespace al
