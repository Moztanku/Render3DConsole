#pragma once

#include <RenderBuffer.hpp>
#include <thread>

#include <Vector3.hpp>

class Renderer {
    public:
        Renderer(RenderBuffer& renderBuffer): 
            RenderBuffer(renderBuffer), 
            directionVectors(renderBuffer.getWidth() * renderBuffer.getHeight(), {0.f, 0.f, 0.f}),
            vertFov((horizFov * renderBuffer.getHeight() / renderBuffer.getWidth()) * 2.f)
            {
                calculateDirectionVectors(directionVectors);
            };

        void renderScene() {
            calculateDirectionVectors(directionVectors);

            const int width = RenderBuffer.getWidth();
            const int height = RenderBuffer.getHeight();

            for(int y = 0; y < height; y++)
                for(int x = 0; x< width; x++)
                {
                    const Vector3& direction = directionVectors[y * width + x];

                    if(raySphereIntersect(cameraPosition, direction, spherePosition, sphereRadius))
                        RenderBuffer.setPixel(x, y, 100);
                    else
                        RenderBuffer.setPixel(x, y, 0); 
                }
        }

        void moveCameraForward(Vector3 delta)
        {
            cameraPosition = cameraPosition + rotate(delta, cameraRotation);
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

        RenderBuffer& RenderBuffer;

        Vector3 cameraPosition = {0.f, 0.f, 0.f};

        float horizFov = deg2rad(90.f);
        float vertFov;

        Vector3 cameraRotation = {0.f, 0.f, 0.f};

        // Normalized direction vector
        std::vector<Vector3> directionVectors;

        // sphere
        Vector3 spherePosition = {400.f, 400.f, 400.f};
        float sphereRadius = 100.f;

        bool raySphereIntersect(const Vector3& origin, const Vector3& direction /* normalized */, const Vector3& spherePos, const float sphereRadius)
        {
            float dist = distance(origin, spherePos);
            float dotp = dotproduct(direction, spherePos - origin);

            if (dist < sphereRadius) return true;
            if (dotp < 0) return false;

            Vector3 closestPoint = origin + (direction * dotp);

            return distance(closestPoint, spherePos) < sphereRadius;
        }

        void calculateDirectionVectors(std::vector<Vector3>& vectors)
        {
            const Vector3 base = rotate(normalize(Vector3{1.f, 1.f, 1.f}), cameraRotation);

            const int width = RenderBuffer.getWidth();
            const int height = RenderBuffer.getHeight();

            const float stepFovH = horizFov / width;
            const float stepFovV = vertFov / height;

            for (int y = 0; y < height; y++)
                for (int x = 0; x < width; x++){
                    Vector3 vec = base;

                    const float rotX = stepFovH * x - horizFov / 2.f;
                    const float rotY = stepFovV * y - vertFov / 2.f;

                    vec = rotate(vec, {rotX, rotY, 0.f});
                    vectors[y * width + x] = vec;
                }
        }
};