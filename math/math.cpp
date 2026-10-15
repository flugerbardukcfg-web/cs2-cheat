#include "math.hpp"

#include "../memory/memory.hpp"
#include <DirectXMath.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#define M_PI 3.14159265358979323846
#define RAD2DEG(radian) (float)radian * (180.f / (float)M_PI)
#define DEG2RAD(degree) (float)degree*((float)M_PI / 180.f)
static VMatrix g_viewMatrix;

bool math::WorldToScreen(const Vector& in, ImVec2& out) {
    if (!ImGui::GetCurrentContext()) return false;

    const float z = g_viewMatrix[3][0] * in.x + g_viewMatrix[3][1] * in.y +
                    g_viewMatrix[3][2] * in.z + g_viewMatrix[3][3];
    if (z < 0.001f) return false;

    out = ImGui::GetIO().DisplaySize * 0.5f;
    out.x *= 1.0f + (g_viewMatrix[0][0] * in.x + g_viewMatrix[0][1] * in.y +
                     g_viewMatrix[0][2] * in.z + g_viewMatrix[0][3]) /
                        z;
    out.y *= 1.0f - (g_viewMatrix[1][0] * in.x + g_viewMatrix[1][1] * in.y +
                     g_viewMatrix[1][2] * in.z + g_viewMatrix[1][3]) /
                        z;

     
    out = ImFloor(out);
    return true;
}

void math::UpdateViewMatrix(VMatrix* pViewMatrix) {
    if (!pViewMatrix) return;
    g_viewMatrix = *pViewMatrix;
}

void math::angle_vectors(const QAngle_t& angles, Vector* forward, Vector* right,
                         Vector* up) {
    float sr, sp, sy, cr, cp, cy;

    DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles.y));
    DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles.x));
    DirectX::XMScalarSinCos(&sr, &cr, DEG2RAD(angles.z));

    if (forward) {
        forward->x = (cp * cy);
        forward->y = (cp * sy);
        forward->z = (-sp);
    }

    if (right) {
        right->x = (-1.f * sr * sp * cy + -1.f * cr * -sy);
        right->y = (-1.f * sr * sp * sy + -1.f * cr * cy);
        right->z = (-1.f * sr * cp);
    }

    if (up) {
        up->x = (cr * sp * cy + -sr * -sy);
        up->y = (cr * sp * sy + -sr * cy);
        up->z = (cr * cp);
    }
}
void math::anglevectors(const QAngle_t& angles, Vector& forward) {
    float sx{}, sy{}, cx{}, cy{};

    DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles.y));
    DirectX::XMScalarSinCos(&sx, &cx, DEG2RAD(angles.x));

    forward.x = cx * cy;
    forward.y = cx * sy;
    forward.z = -sx;
}
void math::NormalizeAngle(QAngle_t angle) {
    angle.x = std::remainderf(angle.x, 180.f);
    angle.y = std::remainderf(angle.y, 360.f);
}
Vector cross_product(const Vector& a, const Vector& b) {
    return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                  a.x * b.y - a.y * b.x);
}

void math::vector_angles(const Vector& forward, Vector& angles)
{
    float yaw, pitch;

    if (forward.y == 0.f && forward.x == 0.f) {
        yaw = 0.f;
        if (forward.z > 0)
            pitch = 270.f;
        else
            pitch = 90.f;
    } else {
        yaw = (float)(std::atan2f(forward.y, forward.x) * 180.f / (float)M_PI);
        if (yaw < 0.f) yaw += 360.f;

        auto tmp = std::sqrtf(forward.x * forward.x + forward.y * forward.y);
        pitch = (float)(std::atan2f(-forward.x, tmp) * 180.f / (float)M_PI);
        if (pitch < 0.f) pitch += 360.f;
    }

    angles.x = pitch;
    angles.y = yaw;
    angles.z = 0.f;
}
float math::normalize_yaw(float yaw) {
    while (yaw < -180.f) yaw += 360.f;
    while (yaw > 180.f) yaw -= 360.f;
    return yaw;
}
