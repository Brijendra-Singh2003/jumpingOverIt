#include <bits/stdc++.h>
#include <raylib.h>
using namespace std;

const int HEIGHT = 720;
const int WIDTH = 1280;
const int BAR_HEIGHT = 30;
const int BAR_WIDTH = 90;
const int GROUND_Y = 120;
const int GROUND_X = 0;

const Color white = {255, 255, 255, 255};
const Color black = {0, 100, 250, 255};

bool FALLING = false;
bool BLOCK_ABOVE;
bool BLOCK_BELOW;

uint16_t MAP[100] = {false};

int speedy = 0;
int Y;
int PLAYER_x = WIDTH / 2;
int PLAYER_y = 0;
int MAX_ALTITUDE = 0;

int main() {
    InitWindow(1280, 720, "Hello world");
    SetTargetFPS(60);

    Texture2D TEXTURE_I_MAN_L = LoadTexture("./i-man-l.png");
    Texture2D TEXTURE_I_MAN_R = LoadTexture("./i-man-r.png");
    Texture2D TEXTURE_BLOCKS = LoadTexture("./block.png");
    Texture2D TEXTURE_GROUND = LoadTexture("./bg.png");
    Texture2D *curr = &TEXTURE_I_MAN_L;

    int prev = 580;
    for (int i = 0; i < 90; i++) {
        prev += 180 - ((rand() * 5) % 365);
        if(prev > 1190)
            prev = (prev - 1190) % 95;
        if (prev < 0)
            prev = ((360 - prev) % 95) + 1100;
        MAP[i] = prev;
    }

    while (!WindowShouldClose()) {
        PLAYER_y += speedy;
        int8_t y = PLAYER_y % 90;
        Y = PLAYER_y / 90;
        BLOCK_ABOVE = PLAYER_x > MAP[Y] - 30 && PLAYER_x < MAP[Y] + 90 && y > 30;
        BLOCK_BELOW = PLAYER_x > MAP[Y - 1] - 30 && PLAYER_x < MAP[Y - 1] + 90;

        if (IsKeyDown(KEY_UP) && !FALLING) {
            speedy = 15;
            FALLING = true;
        }
        if (IsKeyDown(KEY_LEFT) && !(FALLING && (PLAYER_x > MAP[Y] - 30 && PLAYER_x < MAP[Y] + 95 && y > 30))) {
            PLAYER_x = PLAYER_x - 5;
            if (PLAYER_x < 0)
                PLAYER_x += 1245;
            curr = &TEXTURE_I_MAN_L;
        }
        if (IsKeyDown(KEY_RIGHT) && !(FALLING && (PLAYER_x > MAP[Y] - 35 && PLAYER_x < MAP[Y] + 90 && y > 30))) {
            PLAYER_x = (PLAYER_x + 5) % 1255;
            curr = &TEXTURE_I_MAN_R;
        }

        if (PLAYER_y < 0) {
            FALLING = false;
            speedy = 0;
            PLAYER_y = 0;
        }
        else if (FALLING) {
            speedy = (speedy <= -10) ? speedy : speedy - 1;

            if (speedy > 0 && BLOCK_ABOVE) {
                speedy = -10;
                PLAYER_y += speedy;
            }
            else if (speedy < 0 && BLOCK_ABOVE) {
                speedy = 0;
                PLAYER_y = Y * 90 + 90;
                FALLING = false;
                if(Y+1 > MAX_ALTITUDE) {
                    MAX_ALTITUDE = Y+1;
                }
            }
        }
        else if (PLAYER_y > 0 && !BLOCK_BELOW)
            FALLING = true;

        BeginDrawing();
            ClearBackground(black);
            DrawTexture(*curr, PLAYER_x, HEIGHT - GROUND_Y, white);
            DrawTexture(TEXTURE_GROUND, GROUND_X, HEIGHT - GROUND_Y + PLAYER_y + 4, white);

            for (int j = (Y > 0) ? Y - 1 : 0; j < 90 && j <= Y + 7; j++) {
                int x = MAP[j];
                int y = HEIGHT - GROUND_Y - 60 - j * 90 + PLAYER_y;
                DrawTexture(TEXTURE_BLOCKS, x, y, white);
                DrawTexture(TEXTURE_BLOCKS, x + 30, y, white);
                DrawTexture(TEXTURE_BLOCKS, x + 60, y, white);
                // DrawText((std::to_string(x) + ", " + std::to_string(j*90 + 60)).c_str(), x, y-20, 8, white);
            }

            // DrawText((std::to_string(PLAYER_x) + ", " + (std::to_string(y))).c_str(), PLAYER_x, HEIGHT - GROUND_Y - 20, 8, white);
            DrawText(("score: " + std::to_string(MAX_ALTITUDE)).c_str(), 10, 10, 48, white);
            if (MAX_ALTITUDE >= 90) {
                DrawText("you won", 100, 200, 128, white);
            }
        EndDrawing();
    }

    UnloadTexture(TEXTURE_GROUND);
    UnloadTexture(TEXTURE_BLOCKS);
    UnloadTexture(TEXTURE_I_MAN_L);
    UnloadTexture(TEXTURE_I_MAN_R);
    CloseWindow();

    return 0;
}
