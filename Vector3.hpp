#pragma once

#include <cmath>
#include <format>
#include <iostream>

float deg2rad(float deg)
{
    return deg * (M_PI / 180.f);
}

float rad2deg(float rad)
{
    return rad * (180.f / M_PI);
}

struct Vector3 {
    float x;
    float y;
    float z;

    friend constexpr Vector3 normalize(const Vector3& vec)
    {
        float length = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

        return {vec.x / length, vec.y / length, vec.z / length};
    }

    friend constexpr Vector3 rotate(const Vector3& vec, const Vector3& rotation) {
        // Rotation around X-axis
        float sinX = sin(rotation.x);
        float cosX = cos(rotation.x);
        float y1 = cosX * vec.y - sinX * vec.z;
        float z1 = sinX * vec.y + cosX * vec.z;

        // Rotation around Y axis
        float sinY = sin(rotation.y);
        float cosY = cos(rotation.y);
        float x2 = cosY * vec.x + sinY * z1;
        float z2 = -sinY * vec.x + cosY * z1;

        // Rotation around Z axis
        float sinZ = sin(rotation.z);
        float cosZ = cos(rotation.z);
        float x3 = cosZ * x2 - sinZ * y1;
        float y3 = sinZ * x2 + cosZ * y1;

        return Vector3(x3, y3, z2);
    }

    friend constexpr Vector3 rotate(const Vector3& vec, const Vector3& axis, float angle) {
        return {0.f,0.f,0.f};
    }

    friend constexpr float distance(const Vector3& a, const Vector3& b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        float dz = a.z - b.z;

        return sqrtf(dx * dx + dy * dy + dz * dz);
    }

    friend constexpr float dotproduct(const Vector3& a, const Vector3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    friend constexpr Vector3 operator+(const Vector3& a, const Vector3& b)
    {
        return {a.x + b.x, a.y + b.y, a.z + b.z};
    }

    friend constexpr Vector3 operator-(const Vector3& a, const Vector3& b)
    {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }

    friend constexpr Vector3 operator*(const Vector3& a, const float& b)
    {
        return {a.x * b, a.y * b, a.z * b};
    }

    static bool test()
    {
        bool success = true;

        auto compare = [](float a, float b){
            float diff = a - b;
            if(diff < 0.00001f && diff > -0.00001f)
                return true;
            else
                return false;
        };

        {
            Vector3 vec = {1.f, 1.f, 1.f};

            float dist = distance(normalize(vec), {0.f, 0.f, 0.f});
            if(!compare(dist, 1.f))
            {
                std::cerr << "Vector3::normalize failed\n";
                success = false;
            }
        }
        {
            Vector3 vec = {1.f, 1.f, 1.f};

            auto check = [&success, &compare](const Vector3& actual, const Vector3& expected){
                if(!compare(actual.x, expected.x) || !compare(actual.y, expected.y) || !compare(actual.z, expected.z))
                {
                    std::cerr << std::format("Vector3::rotate failed, expected: {} {} {}, actual: {} {} {}\n", expected.x, expected.y, expected.z, actual.x, actual.y, actual.z);
                    success = false;
                }
            };

            check(rotate(vec, {0.f,0.f,0.f}), vec);
            check(rotate(vec, {deg2rad(90.f),0.f,0.f}), {1.f, -1.f, 1.f});
            check(rotate(vec, {0.f,deg2rad(90.f),0.f}), {1.f, 1.f, -1.f});
            check(rotate(vec, {0.f,0.f,deg2rad(90.f)}), {-1.f, 1.f, 1.f});

            check(rotate(vec, {0.f,deg2rad(180.f),0.f}), {-1.f, 1.f, -1.f});

            Vector3 normalized = normalize(vec);
            for(int x = 0.f; x < 360.f; x+=10.f)
                for(int y = 0.f; y < 360.f; y+=10.f)
                    for(int z = 0.f; z < 360.f; z+=10.f)
                    {
                        auto dist = distance(rotate(normalized, {deg2rad(x), deg2rad(y), deg2rad(z)}), {0.f, 0.f, 0.f});
                        if(!compare(dist,1.f))
                        {
                            std::cerr << std::format("Vector3::rotate failed, expected: {}, actual: {}\n", 1.f, dist);
                            success = false;
                        }
                    }
        }

        return success;
    }
};