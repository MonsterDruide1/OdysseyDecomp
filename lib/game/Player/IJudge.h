#pragma once

class IJudge {
public:
    virtual void reset() = 0;
    virtual void update() = 0;
    virtual bool judge() const = 0;
};
