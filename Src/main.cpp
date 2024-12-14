/*
NOTE: Change the NUM_OPS in constants.h to the final size of tiles you will be using.

*/

#include "../Include/raylib.h"

#define NODEBUG
#include "util.h"

#define SCENE CIRCUIT_CTRAIN
#define NUM_OPS OPS_CIRCUIT_CTRAIN

#include "model.h"
// #include "cell_texture_loader.h"
using namespace std;

vector<tile> tiles;                    // last slot is always empty (need to figure out why that was!)
WFCMODEL model(num_rows, num_columns); 

 void loadCellTexture(int targetWidth, int targetHeight)
 {
 #if SCENE == DEFAULT
     {
         tiles.resize(5);
         tiles[0] = tile(loadSpriteWithSize("images/default/blank.png", targetWidth, targetHeight), {"AAA", "AAA", "AAA", "AAA"});
         tiles[1] = tile(loadSpriteWithSize("images/default/up.png", targetWidth, targetHeight), {"ABA", "ABA", "AAA", "ABA"});
         tiles[2] = tiles[1].rotate(1);
         tiles[3] = tiles[1].rotate(2);
         tiles[4] = tiles[1].rotate(3);
     }
 #elif SCENE == CIRCUIT
     {
         tiles.resize(13);
         tiles[0] = tile(loadSpriteWithSize("images/circuit/0.png", targetWidth, targetHeight), {"AAA", "AAA", "AAA", "AAA"});
         tiles[1] = tile(loadSpriteWithSize("images/circuit/1.png", targetWidth, targetHeight), {"BBB", "BBB", "BBB", "BBB"});
         tiles[2] = tile(loadSpriteWithSize("images/circuit/2.png", targetWidth, targetHeight), {"BBB", "BCB", "BBB", "BBB"});
         tiles[3] = tile(loadSpriteWithSize("images/circuit/3.png", targetWidth, targetHeight), {"BBB", "BDB", "BBB", "BDB"});
         tiles[4] = tile(loadSpriteWithSize("images/circuit/4.png", targetWidth, targetHeight), {"ABB", "BCB", "BBA", "AAA"});
         tiles[5] = tile(loadSpriteWithSize("images/circuit/5.png", targetWidth, targetHeight), {"ABB", "BBB", "BBB", "BBA"});
         tiles[6] = tile(loadSpriteWithSize("images/circuit/6.png", targetWidth, targetHeight), {"BBB", "BCB", "BBB", "BCB"});
         tiles[7] = tile(loadSpriteWithSize("images/circuit/7.png", targetWidth, targetHeight), {"BDB", "BCB", "BDB", "BCB"});
         tiles[8] = tile(loadSpriteWithSize("images/circuit/8.png", targetWidth, targetHeight), {"BDB", "BBB", "BCB", "BBB"});
         tiles[9] = tile(loadSpriteWithSize("images/circuit/9.png", targetWidth, targetHeight), {"BCB", "BCB", "BBB", "BCB"});
         tiles[10] = tile(loadSpriteWithSize("images/circuit/10.png", targetWidth, targetHeight), {"BCB", "BCB", "BCB", "BCB"});
         tiles[11] = tile(loadSpriteWithSize("images/circuit/11.png", targetWidth, targetHeight), {"BCB", "BCB", "BBB", "BBB"});
         tiles[12] = tile(loadSpriteWithSize("images/circuit/12.png", targetWidth, targetHeight), {"BBB", "BCB", "BBB", "BCB"});
         for (int i = 2; i < 13; i++)
         {
             for (int j = 1; j < 4; j++)
             {
                 tiles.push_back(tiles[i].rotate(j));
             }
         }
         std::cout << tiles.size() << "\n";
     }
 #elif SCENE == CIRCUIT_CTRAIN
     {
         tiles.resize(13);
         const char *base_path = "images/circuit-coding-train";
         const char *patterns[] = {
             "AAA AAA AAA AAA",
             "BBB BBB BBB BBB",
             "BBB BCB BBB BBB",
             "BBB BDB BBB BDB",
             "ABB BCB BBA AAA",
             "ABB BBB BBB BBA",
             "BBB BCB BBB BCB",
             "BDB BCB BDB BCB",
             "BDB BBB BCB BBB",
             "BCB BCB BBB BCB",
             "BCB BCB BCB BCB",
             "BCB BCB BBB BBB",
             "BBB BCB BBB BCB"};

         for (int i = 0; i < 13; ++i)
         {
             std::string file_path = std::string(base_path) + "/" + std::to_string(i) + ".png";
             tiles[i] = tile(loadSpriteWithSize(file_path.c_str(), targetWidth, targetHeight),
                             split(patterns[i], ' '));
         }
         for (int i = 2; i < 13; i++)
         {
             for (int j = 1; j < 4; j++)
             {
                 tiles.push_back(tiles[i].rotate(j));
             }
         }
         std::cout << tiles.size() << "\n";
     }
 #endif

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

    loadCellTexture(screenWidth / num_columns, screenHeight / num_rows);
    // displayTileOptions();

    model.initializeModel(tiles);

    SetTargetFPS(1000);
    // Main game loop
    while (!WindowShouldClose())
    {
        // Logic
        if (model.applyLogic)
            model.logic(tiles);

        // DRAW
        BeginDrawing();
        ClearBackground(RAYWHITE); // Clear the screen
        draw();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
