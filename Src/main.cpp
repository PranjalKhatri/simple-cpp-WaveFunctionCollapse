/*
NOTE: Change the NUM_OPS in constants.h to the final size of tiles you will be using.

*/

#include "../Include/raylib.h"

#define NODEBUG
#include "util.h"

//see constants.h for more options
#define SCENE ROADS
#define NUM_OPS OPS_ROADS

#include "model.h"
#include "cell_texture.h"
// #include "cell_texture_loader.h"
using namespace std;

vector<tile> tiles; // last slot is always empty (need to figure out why that was!)
WFCMODEL model(num_rows, num_columns);

void draw()
{
    for (size_t i = 0; i < num_rows; i++)
    {
        for (size_t j = 0; j < num_columns; j++)
        {
            size_t index = i * num_columns + j; // 1D index
            if (model.grid[index]->collapsed)
            {
                DrawTexture(tiles[model.grid[index]->options._Find_first()].tex, j * tex_width, i * tex_height, WHITE);
            }
        }
    }
}

int main(void)
{
    srand(std::time(0));
    SetTraceLogLevel(LOG_ERROR);

    InitWindow(screenWidth, screenHeight, "Wave Function Collapse - Pranjal");
    // CellTextureLoader texloader(screenWidth / num_columns, screenHeight / num_rows);
    // texloader.loadCellTexture(tiles);

    loadCellTexture(screenWidth / num_columns, screenHeight / num_rows, tiles);
    // displayTileOptions();

    model.initializeModel(tiles);

    SetTargetFPS(1000);
    // Main game loop
    while (!WindowShouldClose())
    {
        // Logic
        if (model.canApplyLogic)
            model.logic(tiles);
        if (GetKeyPressed() == KEY_S)
        {
            model.initializeModel(tiles);
        }
        // DRAW
        BeginDrawing();
        ClearBackground(RAYWHITE); // Clear the screen
        draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
