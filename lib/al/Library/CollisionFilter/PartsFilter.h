#pragma once

namespace al {

class CollisionParts;
class LiveActor;

class CollisionPartsFilterBase {
public:
    virtual bool isInvalidParts(const CollisionParts& collisionParts) const = 0;
};

class CollisionPartsFilterActor : public CollisionPartsFilterBase {
public:
    CollisionPartsFilterActor(const LiveActor* actor) : mActor(actor) {}

    bool isInvalidParts(const CollisionParts& collisionParts) const override;

private:
    const LiveActor* mActor;
    bool mIsCompareEqual = true;
};

class CollisionPartsFilterSubActor : public CollisionPartsFilterBase {
public:
    CollisionPartsFilterSubActor(const LiveActor* actor) : mActor(actor) {}

    bool isInvalidParts(const CollisionParts& collisionParts) const override;

private:
    const LiveActor* mActor;
};

class CollisionPartsFilterSpecialPurpose : public CollisionPartsFilterBase {
public:
    CollisionPartsFilterSpecialPurpose(const char* specialPurpose)
        : mSpecialPurpose(specialPurpose) {}

    bool isInvalidParts(const CollisionParts& collisionParts) const override;

private:
    const char* mSpecialPurpose;
};

class CollisionPartsFilterIgnoreOptionalPurpose : public CollisionPartsFilterBase {
public:
    CollisionPartsFilterIgnoreOptionalPurpose(const char* specialPurpose)
        : mSpecialPurpose(specialPurpose) {}

    bool isInvalidParts(const CollisionParts& collisionParts) const override;

private:
    const char* mSpecialPurpose;
};

class CollisionPartsFilterMergePair : public CollisionPartsFilterBase {
public:
    CollisionPartsFilterMergePair(CollisionPartsFilterBase* firstFilter,
                                  CollisionPartsFilterBase* secondFilter)
        : mFirstFilter(firstFilter), mSecondFilter(secondFilter) {}

    bool isInvalidParts(const CollisionParts& collisionParts) const override;

private:
    CollisionPartsFilterBase* mFirstFilter;
    CollisionPartsFilterBase* mSecondFilter;
};

}  // namespace al
