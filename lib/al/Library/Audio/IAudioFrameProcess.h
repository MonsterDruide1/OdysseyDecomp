#pragma once

namespace aal {

class IAudioFrameProcess {
public:
    virtual ~IAudioFrameProcess();

private:
    virtual void audioFrameProcess_();
};

}  // namespace aal
