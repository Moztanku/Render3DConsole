#include <cmath>
#include <chrono>
#include <thread>

#include <RenderBuffer.hpp>
#include <Renderer.hpp>
#include <Input.hpp>
#include <format>

#include <ncurses.h>

int main(int argc, char** argv)
{
    const int WIDTH =
        argc > 1 ? std::stoi(argv[1]) : 30;
    const int HEIGHT =
        argc > 2 ? std::stoi(argv[2]) : 20;
    const int FPS =
        argc > 3 ? std::stoi(argv[3]) : 60;

    RenderBuffer buffer(WIDTH, HEIGHT);
    Renderer renderer(buffer);
    Input input(renderer);
        std::chrono::high_resolution_clock::time_point start;
    while (true){

        buffer.clear();

        start = std::chrono::high_resolution_clock::now();
        renderer.renderScene();        
        std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
        buffer.draw();
        std::cout << std::format("frame time {}ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());

        input.handleInput();

        // std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS) - elapsed);
    
    }
}