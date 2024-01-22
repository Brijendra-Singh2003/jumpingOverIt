#include <bits/stdc++.h>
#include <raylib.h>
using namespace std;

int main()
{
    InitWindow(1280, 720, "my Window");
    SetTargetFPS(60);

    int pos_x = 100;
    int pos_y = 100;

    int speed_y = 0;

    while (!WindowShouldClose())
    {
        pos_y += speed_y;

        if (IsKeyDown(KEY_W))
        {
            pos_y -= 5;
        }
        if (IsKeyDown(KEY_S))
        {
            pos_y += 5;
        }
        if (IsKeyDown(KEY_A))
        {
            pos_x -= 5;
        }
        if (IsKeyDown(KEY_D))
        {
            pos_x += 5;
        }
        if (IsKeyDown(KEY_SPACE))
        {
            speed_y = -15;
        }

        if (pos_y > 720 - 50)
        {
            speed_y = 0;
            pos_y = 720 - 50;
        }
        else
        {
            speed_y++;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(pos_x, pos_y, 100, 50, WHITE);
        EndDrawing();
    }

    CloseWindow();
}