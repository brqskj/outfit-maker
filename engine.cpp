
#include "raylib.h"
#include <filesystem>
#include <string>
#include <vector>
#include <functional>


unsigned short int number = 0;
const float padding_ratio = 0.05f;      // 1/20 dello schermo verticale

struct Arrows {

    float side;
    Vector2 right;
    Vector2 left;
    const float horiz_ratio = 0.5f;

    void input(std::function<void(short int)> changeTexture) {
        Vector2 mouse = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, {right.x, right.y, side, side})) changeTexture(1);
            else if (CheckCollisionPointRec(mouse, {left.x, left.y, side, side})) changeTexture(-1);
        }
    }

    void draw(Texture2D arrowTex) {
        DrawTexturePro(arrowTex, { 0, 0, (float)arrowTex.width, (float)arrowTex.height }, { right.x, right.y, side, side }, { 0, 0 }, 0, WHITE);
        DrawTexturePro(arrowTex, { 0, 0, (float)arrowTex.width, (float)arrowTex.height }, { left.x + side, left.y + side, side, side }, { 0, 0 }, 180, WHITE);
    }
};

struct Slot {

    Texture2D texture;
    Rectangle space;
    std::vector<std::string> images;
    int selected = 0;
    Arrows arrows;

    Slot(std::string folder_name) {

        number++;


        /* ----- LOADING ASSETS ----- */
        for (auto &file : std::filesystem::directory_iterator(folder_name)) {
            images.push_back(file.path().string());
        }

        changeTexture(0);


        /* ----- DRAWING SPACE ----- */
        float padding = GetScreenHeight() * padding_ratio;
        float height = (GetScreenHeight() - (padding * 4)) / 3;
        float width = texture.width * height / texture.height;

        space = {
            (GetScreenWidth() - width) * 0.5f,
            (padding * number) + (height * (number - 1)),
            width,
            height
        };


        /* ----- ARROWS ----- */
        arrows.side = 50.f;

        float center = space.y + (space.height * 0.5f);

        arrows.right = {
            space.x + space.width + ((GetScreenWidth() - space.x - space.width) * arrows.horiz_ratio) - arrows.side,
            center - (arrows.side * 0.5f)
        };
        arrows.left = {
            space.x - ((GetScreenWidth() - space.x - space.width) * arrows.horiz_ratio),
            arrows.right.y
        };        
    }

    ~Slot() { UnloadTexture(texture); }

    void changeTexture(short int input) {

        selected += input;

        if (selected < 0) selected = images.size() - 1;
        else if (selected == images.size()) selected = 0;

        UnloadTexture(texture);
        texture = LoadTexture(images[selected].c_str());
    }

    void draw(Texture2D arrowTex) {
        DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height }, space, { 0, 0 }, 0, WHITE);
        arrows.draw(arrowTex);
    }
};


int main() {

    InitWindow(600, 1000, "stylo");
    // SetWindowState(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(30);

    Slot shirt("shirts");
    Slot pants("pants");
    Slot shoes("shoes");

    Texture2D arrowTex = LoadTexture("arrow.png");

    while (!WindowShouldClose()) {

        BeginDrawing();
            ClearBackground(BLACK);
            shirt.draw(arrowTex);
            pants.draw(arrowTex);
            shoes.draw(arrowTex);
        EndDrawing();

        shirt.arrows.input([&shirt](short int i){ shirt.changeTexture(i); });
        pants.arrows.input([&pants](short int i){ pants.changeTexture(i); });
        shoes.arrows.input([&shoes](short int i){ shoes.changeTexture(i); });
    }

    CloseWindow();
    return 0;
}
