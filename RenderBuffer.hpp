#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <array>

class RenderBuffer
{
    public:
        RenderBuffer(uint width, uint height)
            : width(width), height(height), 
            bufferA(width * height, ' '),
            bufferB(width * height, ' '),
            frontBuffer(bufferA),
            backBuffer(bufferB)
        {}

        void clear(uint brightness = 0)
        {
            std::fill(backBuffer.begin(),
                backBuffer.end(),
                getChar(brightness));
        }

        void setPixel(uint x, uint y, uint brightness)
        {
            if(x >= width || y >= height) return;

            backBuffer[y * width + x] = getChar(brightness);
        }

        void draw()
        {
            clear_terminal();
            swapBuffers();
            std::cout << std::string(width + 2, 'x') << '\n';
            for (uint y = 0; y < height; y++){
                std::cout << 'x';
                for (uint x = 0; x < width; x++){
                    std::cout << frontBuffer[y * width + x];
                }
                std::cout << "x\n";
            }
            std::cout << std::string(width + 2, 'x') << std::endl;
        }

        const uint getWidth() const { return width; }
        const uint getHeight() const { return height; }

    private:
        using Buffer = std::vector<char>;


        constexpr static char getChar(uint brightness)
        {
            constexpr std::array<char, 10> characters = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

            if(brightness > 100) brightness = 100;

            return characters[(brightness * (characters.size() - 1)) / 100];
        }

        uint width;
        uint height;

        Buffer bufferA;
        Buffer bufferB;

        Buffer& frontBuffer;
        Buffer& backBuffer;


        void clear_terminal()
        {
            #if defined(_WIN32)
                system("cls");
            #elif defined(__linux__)
                std::cout << "\033[2J\033[1;1H";
            #endif
        }

        void swapBuffers()
        {
            std::swap(frontBuffer, backBuffer);
        }
};