#include <cmath>
#include <chrono>
#include <thread>

#include <RenderBuffer.hpp>
#include <Renderer.hpp>
#include <Input.hpp>

#include <ncurses.h>

int main(int argc, char** argv)
{
    if(!Vector3::test())
        throw std::runtime_error("Vector3 test failed");

    const int WIDTH =
        argc > 1 ? std::stoi(argv[1]) : 30;
    const int HEIGHT =
        argc > 2 ? std::stoi(argv[2]) : 20;
    const int FPS =
        argc > 3 ? std::stoi(argv[3]) : 60;

    RenderBuffer buffer(WIDTH, HEIGHT);
    Renderer renderer(buffer);
    Input input(renderer);

    while (true){
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        buffer.clear();
        input.handleInput();

        renderer.renderScene();
        buffer.draw();

        std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS) - elapsed);
    
    }
}

// int main(int argc, char** argv)
// {
//     const int WIDTH = 
//         argc > 1 ? std::stoi(argv[1]) : 30;
//     const int HEIGHT =
//         argc > 2 ? std::stoi(argv[2]) : 20;
//     RenderBuffer buffer(WIDTH, HEIGHT);

//     auto draw_circle = [&buffer](int centerX, int centerY, int radius){
//         int a_x = centerX - radius; int a_y = centerY - radius;
//         int b_x = centerX + radius; int b_y = centerY + radius;

//         for (int x = a_x; x <= b_x; x++)
//             for (int y = a_y; y <= b_y; y++)
//                 if (std::pow(x - centerX, 2) + std::pow(y - centerY, 2) <= std::pow(radius, 2))
//                     buffer.setPixel(x, y, 100);
//     };

//     auto mychar = L'░';

//     const int FPS = 60;

//     int cx = 10;
//     int cy = 10;
//     int r = 3;

//     auto move_circle = [&cx, &cy, &r, &WIDTH, &HEIGHT](float dx, float dy){
//         static float carryX = 0;
//         static float carryY = 0;

//         carryX += dx - std::floor(dx); carryY += dy - std::floor(dy);
//         cx += dx + carryX; cy += dy + carryY;
//         carryX -= std::floor(carryX); carryY -= std::floor(carryY);

//         if (cx < 0 - r) cx = WIDTH + r;
//         if (cx > WIDTH + r) cx = -r;
//         if (cy < 0 - r) cy = HEIGHT + r;
//         if (cy > HEIGHT + r) cy = -r;
//     };
    
//     while (true){
//         std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
//         move_circle(0.1, 0.05);

//         buffer.clear();
//         draw_circle(cx, cy, r);
//         buffer.draw();

//         std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
//         std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS) - elapsed);
//     }


//     return 0;
// }