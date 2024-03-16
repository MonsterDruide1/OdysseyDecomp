#pragma once

struct BgmUserInfo;

class alBgmFunction{
public:
    static al::BgmUserInfo* tryFindBgmUserInfo(const al::BgmDataBase*, const char*);
};