#include "Item/Coin.h"

#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/Nature/NatureUtil.h"

const sead::Vector3f appearAboveVelocity(0.0f, 25.0f, 0.0f);

const sead::Vector3f& CoinFunction::getAppearAboveVelocity() {
    return appearAboveVelocity;
}

void CoinFunction::appearCoinBlowVeryLittle(Coin* coin, const sead::Vector3f& direction) {
    bool isInWater = al::isInWater(coin) || al::isInWaterArea(coin);

    sead::Vector3f velocity = (isInWater ? 12.0f : 3.5f) * direction;
    sead::Vector3f verticalVelocity = (isInWater ? 35.0f : 30.0f) * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 600);
}

void CoinFunction::appearCoinBlowLittle(Coin* coin, const sead::Vector3f& direction) {
    bool isInWater = al::isInWater(coin) || al::isInWaterArea(coin);

    sead::Vector3f velocity = (isInWater ? 15.0f : 4.5f) * direction;
    sead::Vector3f verticalVelocity = (isInWater ? 40.0f : 30.0f) * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 600);
}

void CoinFunction::appearCoinBlowMiddle(Coin* coin, const sead::Vector3f& direction) {
    bool isInWater = al::isInWater(coin) || al::isInWaterArea(coin);

    sead::Vector3f velocity = (isInWater ? 18.0f : 6.0f) * direction;
    sead::Vector3f verticalVelocity = (isInWater ? 45.0f : 35.0f) * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 600);
}

void CoinFunction::appearCoinBlowLarge(Coin* coin, const sead::Vector3f& direction) {
    bool isInWater = al::isInWater(coin) || al::isInWaterArea(coin);

    sead::Vector3f velocity = (isInWater ? 23.0f : 7.5f) * direction;
    sead::Vector3f verticalVelocity = (isInWater ? 50.0f : 40.0f) * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 600);
}

void CoinFunction::appearCoinBlowLargeEnemy(Coin* coin, const sead::Vector3f& direction) {
    sead::Vector3f velocity = 3.5f * direction;
    sead::Vector3f verticalVelocity = 30.0f * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 1200);
}

void CoinFunction::appearCoinBlowBoss(Coin* coin, const sead::Vector3f& direction) {
    sead::Vector3f velocity = 8.0f * direction;
    sead::Vector3f verticalVelocity = 40.0f * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 1200);
}

void CoinFunction::appearCoinBlowGk(Coin* coin, const sead::Vector3f& direction) {
    sead::Vector3f velocity = 7.0f * direction;
    sead::Vector3f verticalVelocity = 40.0f * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 1200);
}
