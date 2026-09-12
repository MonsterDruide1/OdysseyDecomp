#pragma once

#include <basis/seadTypes.h>
#include <nn/ui2d/Pane.h>

namespace eui {
class TextBoxEx {
public:
    u8 _0[0x11c];
    u8 mDirtyFlags;
};
}  // namespace eui

namespace nn::ui2d {
template <typename T, typename U>
T DynamicCast(U* pane);

template <>
eui::TextBoxEx* DynamicCast<eui::TextBoxEx*, nn::ui2d::Pane>(nn::ui2d::Pane* pane);
}  // namespace nn::ui2d
