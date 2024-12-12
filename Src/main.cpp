/*
NOTE: Change the NUM_OPS in constants.h to the final size of tiles you will be using.

*/

#include "../Include/raylib.h"

#define NODEBUG
#include "util.h"
#define SCENE CIRCUIT
#define NUM_OPS SCENE
#include "model.h"

using namespace std;

vector<tile> tiles; // last slot is always empty (need to figure out why that was!)

void loadCellTexture(int targetWidth, int targetHeight)
{
    if (SCENE == DEFAULT)
    {
        tiles.resize(5);
        tiles[0] = tile(loadSpriteWithSize("images/default/blank.png", targetWidth, targetHeight), {0, 0, 0, 0});
        tiles[1] = tile(loadSpriteWithSize("images/default/up.png", targetWidth, targetHeight), {1, 1, 0, 1});
        tiles[2] = tiles[1].rotate(1);
        tiles[3] = tiles[1].rotate(2);
        tiles[4] = tiles[1].rotate(3);
    }
    else if (SCENE == CIRCUIT)
    {
        tiles.resize(11);
        tiles[0] = tile(loadSpriteWithSize("images/circuit/0.png", targetWidth, targetHeight), {0, 0, 0, 0});
        tiles[1] = tile(loadSpriteWithSize("images/circuit/1.png", targetWidth, targetHeight), {1, 1, 1, 1});
        tiles[2] = tile(loadSpriteWithSize("images/circuit/2.png", targetWidth, targetHeight), {1, 2, 1, 1});
        tiles[3] = tile(loadSpriteWithSize("images/circuit/3.png", targetWidth, targetHeight), {1, 3, 1, 3});
        tiles[4] = tile(loadSpriteWithSize("images/circuit/6.png", targetWidth, targetHeight), {1, 2, 1, 2});
        tiles[5] = tile(loadSpriteWithSize("images/circuit/7.png", targetWidth, targetHeight), {3, 2, 3, 2});
        tiles[6] = tile(loadSpriteWithSize("images/circuit/8.png", targetWidth, targetHeight), {3, 1, 2, 1});
        tiles[7] = tile(loadSpriteWithSize("images/circuit/9.png", targetWidth, targetHeight), {2, 2, 1, 2});
        tiles[8] = tile(loadSpriteWithSize("images/circuit/10.png", targetWidth, targetHeight), {2, 2, 2, 2});
        tiles[9] = tile(loadSpriteWithSize("images/circuit/11.png", targetWidth, targetHeight), {2, 2, 1, 1});
        tiles[10] = tile(loadSpriteWithSize("images/circuit/12.png", targetWidth, targetHeight), {1, 2, 1, 2});
        for (int i = 2; i < 11; i++)
        {
            for (int j = 1; j < 4; j++)
            {
                tiles.push_back(tiles[i].rotate(j));
            }
        }
        std::cout << tiles.size() << "\n";
    }
    for (int i = 0; i < tiles.size(); i++)
    {
        if (!tiles[i].tex.id)
        {
            TraceLog(LOG_ERROR, "Failed to load texture for tile %d", i);
            exit(EXIT_FAILURE);
        }
        tiles[i].idx = i;
    }
    for (int i = 0; i < tiles.size(); i++)
    {
        tiles[i].analyze(tiles);
    }
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
                DrawTexture(tiles[grid[index]->options._Find_first()].tex, j * tex_width, i * tex_height, WHITE);
            }
        }
    }
}

int main(void)
{
    srand(std::time(0));
    SetTraceLogLevel(LOG_ERROR);
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Wave Function Collapse - Pranjal");

    loadCellTexture(screenWidth / num_columns, screenHeight / num_rows);
    for (int i = 0; i < tiles.size(); i++)
    {
        cout << "tile options at " << (long long)tiles[i].idx << endl;
        cout << "up\n";
        for (auto &j : tiles[i].up)
            cout << (long long)j.idx << " ";
        cout << "\nright\n";
        for (auto &j : tiles[i].right)
            cout << (long long)j.idx << " ";
        cout << "\ndown\n";
        for (auto &j : tiles[i].down)
            cout << (long long)j.idx << " ";
        cout << "\nleft\n";
        for (auto &j : tiles[i].left)
            cout << (long long)j.idx << " ";
        cout << endl;
    }

    initializeGrid();
    logOptions();
    {
        int idx = randomRange(0, grid.size());
        grid[idx]->collapsed = true;
        grid[idx]->options = 0;
        grid[idx]->options.set(randomRange(0,tiles.size()-1));
        logOptions();
        propagateConstraints(grid, tiles);
    }
    LOG("here\n");
    SetTargetFPS(1000); // Set our game to run at 20 frames-per-second
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Logic
        if (applyLogic)
            logic(tiles);

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
