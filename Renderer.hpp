#pragma once

#include <RenderBuffer.hpp>
#include <thread>

#include <Vector3.hpp>

class Renderer {
    public:
        Renderer(RenderBuffer& renderBuffer): 
            RenderBuffer(renderBuffer),
            vertFov((horizFov * renderBuffer.getHeight() / renderBuffer.getWidth()) * 1.5f)
            {
            };

        void renderScene() {
            const int width = RenderBuffer.getWidth();
            const int height = RenderBuffer.getHeight();

            const Vector3 base = rotate(normalize(Vector3{1.f, 1.f, 1.f}), cameraRotation);

            const float stepFovH = horizFov / width;
            const float stepFovV = vertFov / height;

            float maxZ = base.z;

            for (int x = 0; x < width; x++)
                for (int y = 0; y < height; y++)
                {
                    const float fovx = x*stepFovH + stepFovH/2 - horizFov/2;
                    const float fovy = y*stepFovV + stepFovV/2 - vertFov/2;

                    const Vector3 dir = rotate(base, {fovy, fovx, 0.f});
                    auto result = raySphereIntersect(cameraPosition, dir, spherePosition, sphereRadius);

                    // std::cout << std::format(
                    //     "{},{} dir = {{{},{},{}}} \n",
                    //     x,y, dir.x, dir.y, dir.z
                    //     );

                    // if(dir.z > maxZ){
                    //     // std::cout << std::format("new maxZ = {}, {} {}\n", dir.z, x, y);
                    //     maxZ = dir.z;
                    // }

                    // if(x == width/2){
                    //     std::cout << std::format(
                    //         "{},{} dir = {{{},{},{}}} \n",
                    //         x,y, dir.x, dir.y, dir.z
                    //     );
                    // }

                    if(result)
                        RenderBuffer.setPixel(x, y, 100);
                    else
                        RenderBuffer.setPixel(x, y, 0);
                }

            drawDebugInfo();

            // std::cout << std::format("base = {{{},{},{}}}\n", base.x, base.y, base.z);
            // std::cout << std::format("maxZ = {}\n", maxZ);
            // exit(1);
        }

        void drawDebugInfo()
        {
            const int y = RenderBuffer.getHeight() - 1;
            const int width = RenderBuffer.getWidth();

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
                    RenderBuffer.setChar(x, y, info[x]);
                else
                    RenderBuffer.setChar(x, y, ' ');
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

        static bool test()
        {
            bool success = true;

            Vector3 spherePos = {100.f,100.f,100.f};
            float sphereRadius = 10.f;

            auto check = [&spherePos, &sphereRadius, &success](const Vector3& origin, const Vector3& direction, const bool expected)
            {
                bool result = raySphereIntersect(origin, direction, spherePos, sphereRadius);

                if(expected != result)
                {
                    std::cerr << std::format("Renderer::raySphereIntersect failed, expected: {}, actual: {}\n", expected, result);
                    success = false;
                }
            };
            Vector3 base = normalize(Vector3{1.f,1.f,1.f});

            check({0.f,0.f,0.f}, base, true);
            check({0.f,0.f,0.f}, rotate(base, {0.f,0.f,0.f}), true);
            check({0.f,0.f,0.f}, rotate(base, {0.f,0.075f,0.f}), false);
            check({0.f,0.f,0.f}, rotate(base, {0.f,-0.075f,0.f}), false);
            

            return success;
        }
    private:
        const int max_threads = std::thread::hardware_concurrency();
        int used_threads = 1;

        RenderBuffer& RenderBuffer;

        Vector3 cameraPosition = {0.f, 0.f, 0.f};

        float horizFov = deg2rad(90.f);
        float vertFov;

        Vector3 cameraRotation = {0.f, 0.f, 0.f};

        // sphere
        Vector3 spherePosition = {1000.f, 1000.f, 1000.f};
        float sphereRadius = 200.f;

        static constexpr bool raySphereIntersect(const Vector3& origin, const Vector3& direction /* normalized */, const Vector3& spherePos, const float sphereRadius)
        {
            float dist = distance(origin, spherePos);

            float dotp = dotproduct(direction, spherePos - origin);

            if (dist < sphereRadius) return true;
            if (dotp < 0) return false;

            Vector3 closestPoint = origin + (direction * dotp);

            return distance(closestPoint, spherePos) < sphereRadius;
        }
};