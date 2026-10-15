#pragma once
#include "types/qangle.hpp"
#include "types/vector.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#define deg_to_rad(x) ((float)(x) * (float)(M_PI / 180.f))
#define rad_to_deg(x) ((float)(x) * (float)(180.f / M_PI))
struct ImVec2;

namespace math {
    bool WorldToScreen(const Vector& in, ImVec2& out);
    void UpdateViewMatrix(VMatrix* pViewMatrix);
    void angle_vectors(const QAngle_t& angles, Vector* forward, Vector* right,
                       Vector* up);
    
    void NormalizeAngle(QAngle_t angle);
    void vector_angles(const Vector& forward, Vector& angles);
    void anglevectors(const QAngle_t& angles, Vector& forward);
    float normalize_yaw(float yaw);
}   
