#include <iostream>
#include <stdlib.h>
#include "windows.h"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <map>
#include <conio.h>

extern "C" void next_step(int count, bool* area, int hight, int width, bool* new_area);
extern "C" void next_tor_step(int count, bool* area, int hight, int width, bool* new_area);

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

        memset(history, 0, Width * sizeof(int));
        current_percent = 0;
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
                    cout << "* ";
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

class Game {
public:
    GameArea Area;
    int FPS;
    unsigned int generation;
    bool is_paused = false;
    int current_mode;
    map<int, string> mode_names;

    Game(const int hight, const int width, int fps = 2) :
    FPS(fps), Area(hight, width) {
        current_mode = 2;
        mode_names[1] = "Classic (Borders)";
        mode_names[2] = "Torus (Seamless)";
    }

    void StartSimulation() {
        Area.InitializeRandom(25);
        generation = 1;
        //Area.InitializePattern("glider");

        GameLoop();
    }

    void GameLoop() {
        while (true) {
            if (_kbhit()) {
                char key = _getch();

                if (key == 27 || key == 'm' || key == 'M') {
                    system("cls");
                    cout << "==================================================" << endl;
                    cout << "                MENU / PAUSE                      " << endl;
                    cout << "==================================================" << endl;
                    cout << " 1. Continue simulation" << endl;
                    cout << " 2. Restart with new random field" << endl;
                    cout << " 3. Toggle Game Mode (Current: " << mode_names[current_mode] << ")" << endl;
                    cout << " 4. Exit game" << endl;
                    cout << "==================================================" << endl;
                    cout << "Choose option (1-4): ";

                    char choice = _getch();

                    if (choice == '2') {
                        Area.InitializeRandom(25);
                        generation = 1;
                        system("cls");
                        continue;
                    }
                    else if (choice == '3') {
                        current_mode = (current_mode == 1) ? 2 : 1;
                        system("cls");
                        continue;
                    }
                    else if (choice == '4') {
                        break;
                    }
                    system("cls");
                }
                else if (key == 'w' || key == 'W') {
                    FPS++;
                    if (FPS > 10) FPS = 10;
                }
                else if (key == 's' || key == 'S') {
                    if (FPS > 1) {
                        FPS--;
                    }
                }
            }


            Area.UpdateGraph();

            cout << "============================================================================================================" << endl;
            cout << "   Generation: " << generation << "   |   Alive: " << Area.current_percent << "%   |   FPS: " << FPS
                << "   |   Mode: " << mode_names[current_mode] << "   |   Press Esc to Menu" << endl;
            cout << "============================================================================================================" << endl;

            Area.PrintArea();

            if (current_mode == 1) {
                next_step(Area.count, Area.area, Area.Hight, Area.Width, Area.new_area);
            }
            else {
                next_tor_step(Area.count, Area.area, Area.Hight, Area.Width, Area.new_area);
            }

            if (!Area.UpdateFrame()) {
                cout << "============================================================================================================" << endl;
                cout << " Game over! Simulation stabilized at generation " << generation << "!" << endl;
                cout << "============================================================================================================" << endl;
                break;
            }

            generation++;
            Sleep(1000/FPS);
            system("cls");
        }
        system("pause");
    }


};

int main()
{
    Game NewGame = Game(25, 25, 2);
    NewGame.StartSimulation();
    return 0;
}