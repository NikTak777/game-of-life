#include <iostream>
#include <stdlib.h>
#include "windows.h"
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

class GameArea {
public:
    const unsigned int Hight;
    const unsigned int Width;
    bool* area;
    bool* new_area;
    int count;


    GameArea(const int hight, const int width):
        Hight(hight), Width(width) {
        area = new bool[Hight * Width]{};
        new_area = new bool[Hight * Width] {};
        count = Hight * Width;
    }

    void InitializeRandom(int percentage = 25) {
        srand(time(0));

        for (int i = 0; i < count; i++) {
            area[i] = (rand() % 100 < percentage);
        }
    }

    void InitializePattern(const std::string& name) {
        memset(area, 0, count);

        int centerX = Width / 2;
        int centerY = Hight / 2;

        if (name == "glider") {
            // Планер
            area[centerY * Width + centerX] = true;
            area[(centerY + 1) * Width + centerX + 1] = true;
            area[(centerY + 2) * Width + centerX - 1] = true;
            area[(centerY + 2) * Width + centerX] = true;
            area[(centerY + 2) * Width + centerX + 1] = true;
        }
        else if (name == "blinker") {
            // Осциллятор
            area[centerY * Width + centerX - 1] = true;
            area[centerY * Width + centerX] = true;
            area[centerY * Width + centerX + 1] = true;
        }
    }

    ~GameArea() {
        delete[] area;
        delete[] new_area;
    }

    void PrintArea() {
        for (int y = 0; y < Hight; y++) {
            for (int x = 0; x < Width; x++) {
                if (area[y * Width + x])
                    cout << "# ";
                else
                    cout << "  ";
            }
            std::cout << "\n";
        }
    }

    bool UpdateFrame() {
        if (memcmp(area, new_area, count) == 0) {
            return false;
        }

        memcpy(area, new_area, count);
        return true;
    }

};

extern "C" void next_step(int count, bool* area, int hight, int width, bool* new_area);


int main()
{
    GameArea Game = GameArea(25, 25);

    Game.InitializeRandom(25);

    //Game.InitializePattern("glider");

    unsigned int generation = 1;

    while (true) {
        Game.PrintArea();
        next_step(Game.count, Game.area, Game.Hight, Game.Width, Game.new_area);

        if (!Game.UpdateFrame()) {
            cout << "==================================================" << endl;
            cout << " Game over! Simulation stabilized at generation " << generation << "!" << endl;
            cout << "==================================================" << endl;
            break;
        }
        else {
            cout << "==================================================" << endl;
            cout << "   Generation: " << generation << "   |   Press Ctrl+C to exit" << endl;
            cout << "==================================================" << endl;
        }

        generation++;
        Sleep(500);
        system("cls");
    }
    system("pause");
}