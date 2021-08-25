#include "al/control/PlayerActionTurnControl.h"

#include <math.h>
#include "al/util/MathUtil.h"

void PlayerActionTurnControl::setup(float a1, float a2, float a3, float a4, int a5, int a6, int a7){
    values[0] = a1;
    values[1] = a2;
    values[2] = a3;
    values[3] = a4;
    values2[0] = a5;
    values2[1] = a6;
    values2[2] = a7;
}

void PlayerActionTurnControl::reset(){
    someThings[1] = 0;
    placeholder4[0] = 0;
    something = 0;
    val = 0;
    someVector = {0,0,0};
    someVector2 = {0,0,0};
    someVectorTurnPowerRate = {0,0,0};
}

void PlayerActionTurnControl::calcTilt(sead::Vector3f* a1, const sead::Vector3f& a2, float a3) {
    float turnPowerRate = calcTurnPowerRate(a2);
    float v13 = (turnPowerRate * a3) * 0.017453f * 0.5f;
    float v14 = std::cosf(v13);
    float v15 = std::sinf(v13);


  float v18 = someVector3.x * v15;
  float v19 = someVector3.y * v15;
  float v20 = someVector3.z * v15;
  //sead::Vector3f newSomeVector3 = someVector3 * v15;

  float v21 = (v19 * a2.z) - (v20 * a2.y) + (v14 * a2.x);
  float v22 = -(v18 * a2.x) - (v19 * a2.y);
  float v23 = (v14 * a2.y) + ((v20 * a2.x) - (v18 * a2.z));
  float v24 = (v14 * a2.z) + ((v18 * a2.y) - (v19 * a2.x));
  float v25 = v22 - (a2.z * v20);
  a1->x = (v19 * v24) + ((v14 * v21) - (v20 * v23)) - (v18 * v25);
  a1->y = (v20 * v21) + (v14 * v23) - (v18 * v24) - (v19 * v25);
  a1->z = (v14 * v24) + ((v18 * v23) - (v19 * v21)) - (v20 * v25);


  /*float v7; // s10
  float v8; // s11
  float v10; // s12
  float v11; // s0
  float v12; // s0
  float v13; // s8
  float v14; // s9
  float v15; // s0
  float v16; // s1
  float v17; // s3
  float v18; // s4
  float v19; // s5
  float v20; // s0
  float v21; // s6
  float v22; // s2
  float v23; // s16
  float v24; // s3
  float v25; // s1

  v7 = this->someVector3.x;
  v8 = this->someVector3.y;
  v10 = this->someVector3.z;
  v12 = calcTurnPowerRate(a2);
  v13 = (float)((float)(v12 * a3) * 0.017453f) * 0.5f;
  v14 = std::cosf(v13);
  v15 = std::sinf(v13);
  float a2x = a2.x;
  v17 = a2.y;
  v16 = a2.z;
  v18 = v7 * v15;
  v19 = v8 * v15;
  v20 = v10 * v15;
  
  float v211 = v19 * v16;
  float v212 = v20 * v17;
  float v213 = v211 - v212;
  v21 = v213 + (v14 * a2x);

  float v221 = (v18 * a2x);
  float v222 = -v221;
  float v223 = (v19 * v17);
  v22 = v222 - v223;

  float v231 = (v20 * a2x);
  float v232 = (v16 * v18);
  v23 = (v14 * v17) + (v231 - v232);

  float v241 = (v18 * v17) - (v19 * a2x);
  float v242 = (v14 * v16);
  v24 = v242 + v241;

  v25 = v22 - (v16 * v20);
  a1->x = ((v19 * v24) + ((v14 * v21) - (v20 * v23))) - (v18 * v25);
  a1->y = (((v20 * v21) + (v14 * v23)) - (v18 * v24)) - (v19 * v25);
  a1->z = ((v14 * v24) + ((v18 * v23) - (v19 * v21))) - (v20 * v25);
/**/

}

float PlayerActionTurnControl::calcTurnPowerRate(const sead::Vector3f& a1) const {
    float result = al::easeIn(al::calcRate01(val, values[0], values[2]));
    if(sead::dot(a1, someVectorTurnPowerRate) > 0){
        return -result;
    }
    return result;
}
