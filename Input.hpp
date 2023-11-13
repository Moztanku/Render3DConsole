#pragma once

#include <Renderer.hpp>

#include <termios.h>

class Input
{
    public:
        Input(Renderer& renderer): renderer(renderer)
        {
        }
        ~Input()
        {
        }

        void handleInput()
        {
            char key = getKeyPress();

            switch(key)
            {
                case 'q':
                    renderer.rotateCamera({0.f, 0.1f, 0.f});
                    break;
                case 'e':
                    renderer.rotateCamera({0.f, -0.1f, 0.f});
                    break;
                case '[':
                    renderer.rotateCamera({0.1f, 0.f, 0.f});
                    break;
                case ']':
                    renderer.rotateCamera({-0.1f, 0.f, 0.f});
                    break;
                case ';':
                    renderer.moveCamera({0.f, 0.f, 0.1f});
                    break;
                case '\'':
                    renderer.moveCamera({0.f, 0.f, -0.1f});
                    break;
            }
        }
    private:
        Renderer& renderer;

        char getKeyPress() {
            struct termios oldt, newt;
            int ch;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            ch = getchar();
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return ch;
        }
};