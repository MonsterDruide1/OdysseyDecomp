#pragma once

namespace al {
class Resource;

class BgmDataBase {
public:
    BgmDataBase(const Resource*, const Resource*);
    static al::BgmDataBase* create(const char*, const char*);
};
}