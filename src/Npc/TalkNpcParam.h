#pragma once

// Note: only the first field is known so far (used by TalkNpcCap::initAttach
// as the model resource name suffix). The rest of TalkNpcParam's layout is
// still NotDecompiled.
class TalkNpcParam {
public:
    const char* mModelName;
};
