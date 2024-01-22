#include <bits/stdc++.h>
#include <raylib.h>
using namespace std;

const int HEIGHT = 720;
const int WIDTH = 1280;
const int BAR_HEIGHT = 30;
const int BAR_WIDTH = 90;
const int GROUND_Y = 120;
const int GROUND_X = 0;
const int TERMINAL_VELOCITY = -10;

const Color white = {255, 255, 255, 255};
const Color black = {0, 100, 250, 255};

bool FALLING = false;
bool BLOCK_ABOVE;
bool BLOCK_BELOW;

int MAP[100] = {false};

int VELOCITY_Y = 0;
int Y;
int PLAYER_X = WIDTH / 2;
int PLAYER_Y = 0;
int CAMERA_X = PLAYER_X;
int CAMERA_Y = PLAYER_Y;
int SCORE = 0;

int main()
{
    InitWindow(1280, 720, "Hello world");
    SetTargetFPS(60);

    Texture2D TEXTURE_I_MAN_L = LoadTexture("./i-man-l.png");
    Texture2D TEXTURE_I_MAN_R = LoadTexture("./i-man-r.png");
    Texture2D TEXTURE_BLOCKS = LoadTexture("./block.png");
    Texture2D TEXTURE_GROUND = LoadTexture("./bg.png");
    Texture2D *curr = &TEXTURE_I_MAN_L;

    int prev = 580;
    for (int i = 0; i < 90; i++)
    {
        prev += 180 - ((rand() * 5) % 365);
        // if (prev > 1160)
        // {
        //     prev = 1160 - (prev - 1160);
        // }
        // else if (prev < 60)
        // {
        //     prev = 60 - prev;
        // }
        MAP[i] = prev;
    }

    while (!WindowShouldClose())
    {
        // if (CAMERA_X - PLAYER_X > 100)
        // {
        //     CAMERA_X -= 2;
        // }
        // if (PLAYER_X - CAMERA_X > 100)
        // {
        CAMERA_X += (PLAYER_X - CAMERA_X) / 16;
        CAMERA_Y += (PLAYER_Y - CAMERA_Y) / 16;
        // }
        // if (CAMERA_Y - PLAYER_Y > 100)
        // {
        //     CAMERA_Y -= 2;
        // }
        // if (PLAYER_Y - CAMERA_Y > 100)
        // {
        //     CAMERA_Y += 2;
        // }

        PLAYER_Y += VELOCITY_Y;
        int8_t y = PLAYER_Y % 90;
        Y = PLAYER_Y / 90;
        BLOCK_ABOVE = PLAYER_X > MAP[Y] - 30 && PLAYER_X < MAP[Y] + 90 && y > 30;
        BLOCK_BELOW = PLAYER_X > MAP[Y - 1] - 30 && PLAYER_X < MAP[Y - 1] + 90;

        if (IsKeyDown(KEY_UP) && !FALLING)
        {
            VELOCITY_Y = 15;
            FALLING = true;
        }
        if (IsKeyDown(KEY_LEFT) && !(FALLING && (PLAYER_X > MAP[Y] - 30 && PLAYER_X < MAP[Y] + 95 && y > 30)))
        {
            PLAYER_X = PLAYER_X - 5;
            // if (PLAYER_X < 0)
            //     PLAYER_X += 1245;
            curr = &TEXTURE_I_MAN_L;
        }
        if (IsKeyDown(KEY_RIGHT) && !(FALLING && (PLAYER_X > MAP[Y] - 35 && PLAYER_X < MAP[Y] + 90 && y > 30)))
        {
            PLAYER_X = PLAYER_X + 5;
            curr = &TEXTURE_I_MAN_R;
        }

        if (PLAYER_Y < 0)
        {
            FALLING = false;
            VELOCITY_Y = 0;
            PLAYER_Y = 0;
        }
        else if (FALLING)
        {
            if (VELOCITY_Y > TERMINAL_VELOCITY)
            {
                VELOCITY_Y--;
            }
            else
            {
                VELOCITY_Y++;
            }

            if (VELOCITY_Y > 0 && BLOCK_ABOVE)
            {
                VELOCITY_Y = -VELOCITY_Y;
                PLAYER_Y += VELOCITY_Y;
            }
            else if (VELOCITY_Y < 0 && BLOCK_ABOVE)
            {
                VELOCITY_Y = 0;
                PLAYER_Y = Y * 90 + 90;
                FALLING = false;
                if (Y + 1 > SCORE)
                {
                    SCORE = Y + 1;
                }
            }
        }
        else if (PLAYER_Y > 0 && !BLOCK_BELOW)
        {
            FALLING = true;
        }

        BeginDrawing();
        ClearBackground(black);
        DrawTexture(*curr, 625 + PLAYER_X - CAMERA_X, HEIGHT - GROUND_Y - 130 + CAMERA_Y - PLAYER_Y, white);
        DrawRectangle(GROUND_X, HEIGHT - GROUND_Y - 104 + CAMERA_Y + 4, 1280, 220, DARKGREEN);

        for (int j = (Y > 5) ? Y - 6 : 0; j < 90 && j <= Y + 7; j++)
        {
            int x = MAP[j] - CAMERA_X + 625;
            int y = HEIGHT - GROUND_Y - 190 - j * 90 + CAMERA_Y;
            DrawTexture(TEXTURE_BLOCKS, x, y, white);
            DrawTexture(TEXTURE_BLOCKS, x + 30, y, white);
            DrawTexture(TEXTURE_BLOCKS, x + 60, y, white);
            // DrawText((std::to_string(x) + ", " + std::to_string(j*90 + 60)).c_str(), x, y-20, 8, white);
        }

        // DrawText((std::to_string(PLAYER_X) + ", " + (std::to_string(y))).c_str(), PLAYER_X, HEIGHT - GROUND_Y - 20, 8, white);
        DrawText(("score: " + std::to_string(SCORE)).c_str(), 10, 10, 48, white);
        if (SCORE >= 90)
        {
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
