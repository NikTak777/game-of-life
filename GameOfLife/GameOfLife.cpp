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
    int* history;
    int current_percent;

    GameArea(const int hight, const int width) :
        Hight(hight), Width(width) {
        area = new bool[Hight * Width] {};
        new_area = new bool[Hight * Width] {};
        count = Hight * Width;

        history = new int[Width] {};
        current_percent = 0;
    }

    ~GameArea() {
        delete[] area;
        delete[] new_area;
        delete[] history;
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
            area[centerY * Width + centerX] = true;
            area[(centerY + 1) * Width + centerX + 1] = true;
            area[(centerY + 2) * Width + centerX - 1] = true;
            area[(centerY + 2) * Width + centerX] = true;
            area[(centerY + 2) * Width + centerX + 1] = true;
        }
        else if (name == "blinker") {
            area[centerY * Width + centerX - 1] = true;
            area[centerY * Width + centerX] = true;
            area[centerY * Width + centerX + 1] = true;
        }
    }

    void UpdateGraph() {
        int alive = 0;
        for (int i = 0; i < count; i++) {
            if (area[i]) alive++;
        }

        current_percent = (alive * 100) / count;
        int bar_height = (current_percent * (Hight - 1)) / 50;

        if (bar_height > Hight - 1) {
            bar_height = Hight - 1;
        }

        for (int i = 0; i < Width - 1; i++) {
            history[i] = history[i + 1];
        }

        history[Width - 1] = bar_height;
    }

    void PrintArea() {
        for (int y = 0; y < Hight; y++) {

            for (int x = 0; x < Width; x++) {
                if (area[y * Width + x]) cout << "# ";
                else cout << "  ";
            }

            if (y == 0) {
                cout << "50% ";
            }
            else if (y == Hight - 1) {
                cout << " 0% ";
            }
            else {
                cout << " |  ";
            }

            int current_level = Hight - 1 - y;

            for (int x = 0; x < Width; x++) {
                if (history[x] >= current_level && history[x] > 0) {
                    cout << "* ";
                }
                else if (current_level == 0) {
                    cout << "_ ";
                }
                else {
                    cout << "  ";
                }
            }
            cout << "\n";
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
        Game.UpdateGraph();

        cout << "============================================================================================================" << endl;
        cout << "   Generation: " << generation << "   |   Alive: " << Game.current_percent << "%   |   Press Ctrl+C to exit" << endl;
        cout << "============================================================================================================" << endl;

        Game.PrintArea();
        next_step(Game.count, Game.area, Game.Hight, Game.Width, Game.new_area);

        if (!Game.UpdateFrame()) {
            cout << "============================================================================================================" << endl;
            cout << " Game over! Simulation stabilized at generation " << generation << "!" << endl;
            cout << "============================================================================================================" << endl;
            break;
        }

        generation++;
        Sleep(500);
        system("cls");
    }
    system("pause");
}