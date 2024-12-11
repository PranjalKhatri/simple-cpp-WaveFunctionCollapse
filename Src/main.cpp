#include "../Include/raylib.h"

#define NODEBUG
#include "util.h"
#include "model.h"

using namespace std;

vector<tile> tiles(5);

void loadCellTexture(int targetWidth, int targetHeight)
{    
    tiles[0] = tile(loadSpriteWithSize("images/blank.png", targetWidth, targetHeight),{0,0,0,0});
    tiles[1] = tile(loadSpriteWithSize("images/up.png", targetWidth, targetHeight),{1,1,0,1});
    tiles[2] = tiles[1].rotate(1);
    tiles[3] = tiles[1].rotate(2);
    tiles[4] = tiles[1].rotate(3);
    for(auto &t : tiles)t.analyze(tiles);
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
                    DrawTexture(tiles[4].tex, j * tex_width, i * tex_height, WHITE);
                    break;
                case RIGHT:
                    DrawTexture(tiles[2].tex, j * tex_width, i * tex_height, WHITE);
                    break;
                case UP:
                    DrawTexture(tiles[1].tex, j * tex_width, i * tex_height, WHITE);
                    break;
                case DOWN:
                    DrawTexture(tiles[3].tex, j * tex_width, i * tex_height, WHITE);
                    break;
                case BLANK:
                    DrawTexture(tiles[0].tex, j * tex_width, i * tex_height, WHITE);
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
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
