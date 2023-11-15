#include <cmath>
#include <chrono>
#include <thread>

#include <RenderBuffer.hpp>
#include <Renderer.hpp>
#include <Input.hpp>

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

    while (true){
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        buffer.clear();

        renderer.renderScene();        
        buffer.draw();
        input.handleInput();

        std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS) - elapsed);
    
    }
}