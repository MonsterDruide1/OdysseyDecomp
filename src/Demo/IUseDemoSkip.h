#pragma once

class IUseDemoSkip {
public:
    virtual bool isFirstDemo() const = 0;
    virtual bool isEnableSkipDemo() const = 0;
    virtual void skipDemo() = 0;

    virtual void updateOnlyDemoGraphics() {}
};
