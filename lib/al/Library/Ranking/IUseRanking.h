#pragma once

namespace al {
class RankingDirector;

class IUseRanking {
public:
    virtual ~IUseRanking();

    virtual RankingDirector* getRankingDirector() const;
    virtual bool isAvailableRanking() const;
};
}  // namespace al
