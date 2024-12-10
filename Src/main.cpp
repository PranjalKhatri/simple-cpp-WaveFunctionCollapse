#include "../Include/raylib.h"

#define NODEBUG
#include "util.h"
#include "model.h"

using namespace std;

Texture2D tex_left, tex_right, tex_up, tex_down, tex_blank;

void loadCellTexture(int targetWidth, int targetHeight)
{
    tex_left = loadSpriteWithSize("images/left.png", targetWidth, targetHeight);
    tex_right = loadSpriteWithSize("images/right.png", targetWidth, targetHeight);
    tex_up = loadSpriteWithSize("images/up.png", targetWidth, targetHeight);
    tex_down = loadSpriteWithSize("images/down.png", targetWidth, targetHeight);
    tex_blank = loadSpriteWithSize("images/blank.png", targetWidth, targetHeight);
}

void draw()
{
    for (size_t i = 0; i < num_rows; i++)
    {
        for (size_t j = 0; j < num_columns; j++)
        {
            size_t index = i * num_columns + j; // 1D index
            if (grid[index]->collapsed)
            {
                switch (grid[index]->options)
                {
                case LEFT:
                    DrawTexture(tex_left, j * tex_width, i * tex_height, WHITE);
                    break;
                case RIGHT:
                    DrawTexture(tex_right, j * tex_width, i * tex_height, WHITE);
                    break;
                case UP:
                    DrawTexture(tex_up, j * tex_width, i * tex_height, WHITE);
                    break;
                case DOWN:
                    DrawTexture(tex_down, j * tex_width, i * tex_height, WHITE);
                    break;
                case BLANK:
                    DrawTexture(tex_blank, j * tex_width, i * tex_height, WHITE);
                    break;
                }
            }
        }
    }
}

int main(void)
{
    srand(std::time(0));
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Wave Function Collapse - Pranjal");

    loadCellTexture(screenWidth / num_columns, screenHeight / num_rows);

    initializeGrid();
    logOptions();
    {
        int idx = randomRange(0, grid.size());
        grid[idx]->collapsed = true;
        grid[idx]->options = 1 << randomRange(0, 3);
        propagateConstraints(idx, grid);
    }
    SetTargetFPS(60); // Set our game to run at 20 frames-per-second
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Logic
        logic();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE); // Clear the screen
        draw();
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(tex_left);
    UnloadTexture(tex_right);
    UnloadTexture(tex_up);
    UnloadTexture(tex_down);
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
