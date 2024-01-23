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
uint8_t R = 0;
uint8_t B = 255;
uint8_t G = B / 2;
const Color background = {0, 228, 48, 255};

bool FALLING = false;
bool HOLD = false;
bool BLOCK_ABOVE;
bool BLOCK_BELOW;

vector<int> MAP;

int VELOCITY_Y = 0;
int Y;
int PLAYER_X = WIDTH / 2;
int PLAYER_Y = 0;
int CAMERA_X = PLAYER_X;
int CAMERA_Y = PLAYER_Y;
int SCORE = 0;
int CH = 0;

int main()
{
    InitWindow(1280, 720, "Jumping Over It");
    InitAudioDevice();
    SetTargetFPS(60);

    Texture2D TEXTURE_I_MAN_L = LoadTexture("assets/i-man-l.png");
    Texture2D TEXTURE_I_MAN_R = LoadTexture("assets/i-man-r.png");
    Texture2D TEXTURE_BLOCKS = LoadTexture("assets/block.png");
    Texture2D TEXTURE_GROUND = LoadTexture("assets/bg.png");
    Sound COMPLETE_FX = LoadSound("resources/mario-end-level.wav");
    Texture2D *curr = &TEXTURE_I_MAN_L;

    int prev = 580;
    for (int i = 0; i < 10; i++)
    {
        const int t = rand() % 301;
        prev += 150 - t;
        MAP.push_back(prev);
    }
    // for (int i = 20; i < 40; i++)
    // {
    //     const int t = rand() % 401;
    //     prev += 200 - t;
    //     MAP.push_back(prev);
    // }
    // char c = 1;
    // int t = MAP[2];

    while (!WindowShouldClose())
    {
        // MAP[4] += c;
        // MAP[2] += c;
        // if (MAP[2] > t + 135 || MAP[2] < t - 135)
        // {
        //     c = -c;
        // }
        // cout << "HOLD: " << HOLD << '\n';

        CAMERA_X += (PLAYER_X - CAMERA_X) / 16;
        CAMERA_Y += (PLAYER_Y - CAMERA_Y) / 16;

        PLAYER_Y += VELOCITY_Y;
        int8_t y = PLAYER_Y % 90;
        Y = PLAYER_Y / 90;
        BLOCK_ABOVE = PLAYER_X > MAP[Y] - 30 && PLAYER_X < MAP[Y] + 90 && y > 30;
        BLOCK_BELOW = PLAYER_X > MAP[Y - 1] - 30 && PLAYER_X < MAP[Y - 1] + 90;

        if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_UP))
        {
            if (FALLING)
            {
                PLAYER_Y++;
            }
            else if (!HOLD)
            {
                VELOCITY_Y = 15;
                FALLING = true;
            }
            HOLD = true;
        }
        else
        {
            HOLD = false;
        }
        if (IsKeyDown(KEY_LEFT) && !(FALLING && (PLAYER_X > MAP[Y] - 30 && PLAYER_X < MAP[Y] + 95 && y > 30)))
        {
            PLAYER_X = PLAYER_X - 5;
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

            CH = Y + 1;

            if (VELOCITY_Y > 0 && BLOCK_ABOVE)
            {
                VELOCITY_Y = TERMINAL_VELOCITY;
                PLAYER_Y += VELOCITY_Y;
            }
            else if (VELOCITY_Y < 0 && BLOCK_ABOVE)
            {
                VELOCITY_Y = 0;
                PLAYER_Y = Y * 90 + 90;
                FALLING = false;
                B = 255 - CH;
                G = B / 2;
                if (CH > SCORE)
                {
                    SCORE = CH;
                    const int t = rand() % 21;
                    prev = (SCORE % 100 < 50) ? prev + 200 - t * t : prev - 200 + t * t;
                    MAP.push_back(prev);
                    if (SCORE == 90)
                    {
                        PlaySound(COMPLETE_FX);
                    }
                }
            }
        }
        else if (PLAYER_Y > 0 && !BLOCK_BELOW)
        {
            FALLING = true;
        }

        BeginDrawing();
        ClearBackground(Color{R, G, B, 255});
        DrawTexture(*curr, 625 + PLAYER_X - CAMERA_X, HEIGHT - GROUND_Y - 130 + CAMERA_Y - PLAYER_Y, white);
        DrawTexture(TEXTURE_GROUND, -30 - (CAMERA_X % 30), HEIGHT - GROUND_Y - 100 + CAMERA_Y, background);

        for (int j = (Y > 3) ? Y - 4 : 0; j <= Y + 7; j++)
        {
            int x = MAP[j] - CAMERA_X + 625;
            int y = HEIGHT - GROUND_Y - 190 - j * 90 + CAMERA_Y;
            DrawTexture(TEXTURE_BLOCKS, x, y, white);
            // DrawText((std::to_string(x) + ", " + std::to_string(j*90 + 60)).c_str(), x, y-20, 8, white);
        }

        // DrawText((std::to_string(PLAYER_X) + ", " + (std::to_string(y))).c_str(), PLAYER_X, HEIGHT - GROUND_Y - 20, 8, white);
        DrawText(("Score: " + std::to_string(SCORE)).c_str(), 10, 10, 48, white);
        DrawText(("Current Height: " + std::to_string(CH)).c_str(), 10, 60, 24, white);
        if (SCORE == 80)
        {
            DrawText("Never Gonna Let You Down", 100, 200, 48, white);
            // MAP[Y] = INT_MIN;
        }
        if (SCORE == 40)
        {
            DrawText("Never Gonna Give You Up", 100, 200, 48, white);
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
