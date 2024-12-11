#include "../Include/raylib.h"

#define NODEBUG
#include "util.h"
#include "model.h"

using namespace std;

vector<tile> tiles(6);//0 means nothing

void loadCellTexture(int targetWidth, int targetHeight)
{    
    tiles[5] = tile(); // Initialize a blank tile
    tiles[0] = tile(loadSpriteWithSize("images/blank.png", targetWidth, targetHeight),{0,0,0,0});
    tiles[1] = tile(loadSpriteWithSize("images/up.png", targetWidth, targetHeight),{1,1,0,1});
    tiles[2] = tiles[1].rotate(1);
    tiles[3] = tiles[1].rotate(2);
    tiles[4] = tiles[1].rotate(3);

    for (int i = 0; i < tiles.size()-1; i++)
    {
        if (!tiles[i].tex.id) {
            TraceLog(LOG_ERROR, "Failed to load texture for tile %d", i);
            exit(EXIT_FAILURE);
        }
        tiles[i].idx = 1<<(i);        
    }
    for (int i = 0; i < tiles.size()-1; i++)
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
                DrawTexture(tiles[__builtin_ctz(grid[index]->options)].tex,j*tex_width,i*tex_height,WHITE);
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
    /*for (int i = 0; i < tiles.size()-1; i++)
    {
        cout<<"tile options at "<<(int)tiles[i].idx<<endl;
        cout<<"up\n";
        for(auto &j : tiles[i].up)cout<<(int)j.idx<<" ";
        cout<<"\nright\n";
        for(auto &j : tiles[i].right)cout<<(int)j.idx<<" ";
        cout<<"\ndown\n";
        for(auto &j : tiles[i].down)cout<<(int)j.idx<<" ";
        cout<<"\nleft\n";
        for(auto &j : tiles[i].left)cout<<(int)j.idx<<" ";
        cout<<endl;
    }*/
    
    initializeGrid();
    logOptions();
    {
        int idx = randomRange(0, grid.size());
        grid[idx]->collapsed = true;
        grid[idx]->options = 1 << randomRange(1, tiles.size());
        logOptions();
        propagateConstraints(grid,tiles);
    }
    SetTargetFPS(100); // Set our game to run at 20 frames-per-second
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Logic
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
