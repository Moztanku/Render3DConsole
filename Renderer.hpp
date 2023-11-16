#pragma once

#include <RenderBuffer.hpp>
#include <thread>

#include <Vector3.hpp>

class Renderer {
    public:
        Renderer(RenderBuffer& renderBuffer): 
            renderBuffer(renderBuffer),
            vertFov((horizFov * renderBuffer.getHeight() / renderBuffer.getWidth()) * 2.0f)
            {
            };

        void renderScene() {
            const int width = renderBuffer.getWidth();
            const int height = renderBuffer.getHeight();

            const Vector3 base = rotate(
                Vector3{1.f,0.f,0.f},
                Vector3{0.f,1.f,0.f},
                cameraRotation.y
            );

            const float stepFovH = horizFov / width;
            const float stepFovV = vertFov / height;

            float maxZ = base.z;

            const Vector3 xAxis = rotate(
                Vector3{0.f,1.f,0.f},
                Vector3{0.f,1.f,0.f},
                cameraRotation.y
            );

            const Vector3 yAxis = rotate(
                Vector3{0.f,0.f,1.f},
                Vector3{0.f,1.f,0.f},
                cameraRotation.y
            );
            for (int y = 0; y < height; y++)
                for (int x = 0; x < width; x++)
                {
                    const float fovx = x*stepFovH + stepFovH/2 - horizFov/2;
                    const float fovy = y*stepFovV + stepFovV/2 - vertFov/2;

                    const Vector3 dir = rotate(
                        rotate(base, xAxis, fovx),
                        yAxis,
                        fovy);
                    auto result = raySphereIntersect(cameraPosition, dir, spherePosition, sphereRadius);

                    if(!result.hit)
                        continue;

                    const Vector3 notnormal = result.points.first - spherePosition;
                    const Vector3 normal = normalize(result.points.first - spherePosition);

                    const float arc = fabs(arch(-dir, normal));

                    const uint baseBrightness = 25u;
                    const uint normalBrightness = 20u * cosf(arc);

                    const float dotp = dotproduct(normal, -skyLightDirection);
                    const uint skyBrigtness = dotp > 0 ? skyLightBrightness * dotp : 0u;

                    const uint brightness = baseBrightness + normalBrightness + skyBrigtness;
                    renderBuffer.setPixel(x, y, brightness);
                }

            drawDebugInfo();
        }

        void drawDebugInfo()
        {
            const int y = renderBuffer.getHeight() - 1;
            const int width = renderBuffer.getWidth();

            std::string info = std::format(
                "pos: {} {} {}, rot: {} {} {}, dist: {}",
                static_cast<int>(cameraPosition.x),
                static_cast<int>(cameraPosition.y),
                static_cast<int>(cameraPosition.z),
                static_cast<int>(rad2deg(cameraRotation.x)),
                static_cast<int>(rad2deg(cameraRotation.y)),
                static_cast<int>(rad2deg(cameraRotation.z)),
                static_cast<int>(distance(cameraPosition, spherePosition))
                );

            for(int x = 0; x < width; x++)
            {
                if(x < info.size())
                    renderBuffer.setChar(x, y, info[x]);
                else
                    renderBuffer.setChar(x, y, ' ');
            }
        }

        void moveCameraForward(Vector3 delta)
        {
            const Vector3 move = rotate(
                delta,
                Vector3{0.f,1.f,0.f},
                cameraRotation.y
            );
            cameraPosition = cameraPosition + move;
        }

        void moveCamera(Vector3 delta)
        {
            cameraPosition = cameraPosition + delta;
        }

        void rotateCamera(Vector3 delta)
        {
            cameraRotation = cameraRotation + delta;
        }
    private:
        const int max_threads = std::thread::hardware_concurrency();
        int used_threads = 1;

        RenderBuffer& renderBuffer;

        Vector3 cameraPosition = {0.f, 0.f, 0.f};

        float horizFov = deg2rad(90.f);
        float vertFov;

        Vector3 cameraRotation = {0.f, 0.f, 0.f};

        // sphere
        Vector3 spherePosition = {1000.f, 0.f, 0.f};
        float sphereRadius = 200.f;

        // Sky light
        Vector3 skyLightDirection = normalize({0.5f, 0.2f, 0.1f});
        uint skyLightBrightness = 100u;

        struct IntersectionResult
        {
            bool hit;
            std::pair<Vector3, Vector3> points;
        };

        static constexpr IntersectionResult raySphereIntersect(const Vector3& origin, const Vector3& direction /* normalized */, const Vector3& spherePos, const float sphereRadius)
        {
            float dist = distance(origin, spherePos);

            float dotp = dotproduct(direction, spherePos - origin);

            if (dist < sphereRadius) return {false};
            if (dotp < 0) return {false};

            const Vector3 closestPoint = origin + (direction * dotp);
            const float d = distance(closestPoint, spherePos);

            if (d > sphereRadius) return {false};

            const float x = sqrtf(sphereRadius * sphereRadius - d * d);

            return {true, {closestPoint - direction * x, closestPoint + direction * x}};
        }
};