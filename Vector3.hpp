#pragma once

#include <cmath>
#include <format>
#include <iostream>

constexpr float deg2rad(float deg) noexcept
{
    return deg * (M_PI / 180.f);
}

constexpr float rad2deg(float rad) noexcept
{
    return rad * (180.f / M_PI);
}

struct Vector3 {
    float x;
    float y;
    float z;
};

constexpr Vector3 operator-(const Vector3& vec)
{
    return {-vec.x, -vec.y, -vec.z};
}

constexpr Vector3 operator+(const Vector3& a, const Vector3& b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

constexpr Vector3 operator-(const Vector3& a, const Vector3& b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

constexpr Vector3 operator*(const Vector3& a, const float& b)
{
    return {a.x * b, a.y * b, a.z * b};
}

constexpr Vector3 normalize(const Vector3& vec)
{
    float length = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

        return {vec.x / length, vec.y / length, vec.z / length};
}

constexpr float distance(const Vector3& a, const Vector3& b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;

    return sqrtf(dx * dx + dy * dy + dz * dz);
}

constexpr float dotproduct(const Vector3& a, const Vector3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

constexpr Vector3 crossproduct(const Vector3& a, const Vector3& b)
{
    return {
        a.y * b.z - a.z * b.y, 
        a.z * b.x - a.x * b.z, 
        a.x * b.y - a.y * b.x
    };
}

constexpr Vector3 rotate(const Vector3& vec, const Vector3& rotation)
{
    // Rotation around X-axis
    float sinX = sinf(rotation.x);
    float cosX = cosf(rotation.x);
    float y1 = cosX * vec.y - sinX * vec.z;
    float z1 = sinX * vec.y + cosX * vec.z;

    // Rotation around Y axis
    float sinY = sinf(rotation.y);
    float cosY = cosf(rotation.y);
    float x2 = cosY * vec.x + sinY * z1;
    float z2 = -sinY * vec.x + cosY * z1;

    // Rotation around Z axis
    float sinZ = sinf(rotation.z);
    float cosZ = cosf(rotation.z);
    float x3 = cosZ * x2 - sinZ * y1;
    float y3 = sinZ * x2 + cosZ * y1;

    return Vector3(x3, y3, z2);
}

constexpr Vector3 rotate(const Vector3& vec, const Vector3& axis, float angle)
{
    // v' = v * cos(θ) + (k × v) * sin(θ) + k * (k · v) * (1 - cos(θ))  (Rodrigues' rotation formula)
    Vector3 k = normalize(axis);

    float cosθ = cosf(angle);
    float sinθ = sinf(angle);

    Vector3 rotated = 
        vec * cosθ + crossproduct(k, vec) * sinθ + k * dotproduct(k, vec) * (1.f - cosθ);

    return rotated;
}

constexpr float arch(const Vector3& v1, const Vector3& v2)
{
    return acosf(
        dotproduct(v1, v2)
    );
}