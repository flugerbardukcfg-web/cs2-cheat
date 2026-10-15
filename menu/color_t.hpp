#include <imgui/imgui.h>
#include <stdint.h>
struct color_t {
   public:
    float r, g, b, a;
    constexpr color_t(const int r, const int g, const int b, const int a = 255)
        : r(static_cast<uint8_t>(r)),
          g(static_cast<uint8_t>(g)),
          b(static_cast<uint8_t>(b)),
          a(static_cast<uint8_t>(a)) {}
    inline ImColor to_im_color(float alpha = 1.f, bool use_gl_alpha = true) {
        return ImColor(
            r, g, b,
            (a * (use_gl_alpha ? ImGui::GetStyle().Alpha : 1.f)) * alpha);
    }

    inline ImVec4 to_vec4(float alpha = 1.f, bool use_gl_alpha = true) {
        return ImVec4(
            r, g, b,
            (a * (use_gl_alpha ? ImGui::GetStyle().Alpha : 1.f)) * alpha);
    }
};
