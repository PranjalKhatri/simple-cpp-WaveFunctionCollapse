#ifndef CELL_TEXTURE_H
#define CELL_TEXTURE_H
#include "util.h"
#include "tile.h"
void loadCellTexture(int targetWidth, int targetHeight, std::vector<tile> &tiles)
{
#if SCENE == DEFAULT
    {
        tiles.resize(5);
        std::string base_path = "images/default";
        tiles[0] = tile(loadSpriteWithSize((base_path + "/blank.png").c_str(), targetWidth, targetHeight), {"AAA", "AAA", "AAA", "AAA"});
        tiles[1] = tile(loadSpriteWithSize((base_path + "/up.png").c_str(), targetWidth, targetHeight), {"ABA", "ABA", "AAA", "ABA"});
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
            std::std::string file_path = std::std::string(base_path) + "/" + std::to_std::string(i) + ".png";
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

#elif SCENE == DEMO_TRACKS
    {
        tiles.resize(5);
        std::string base_path = "images/demo-tracks";
        tiles[0] = tile(loadSpriteWithSize((base_path + "/blank.png").c_str(), targetWidth, targetHeight), {"AAA", "AAA", "AAA", "AAA"});
        tiles[1] = tile(loadSpriteWithSize((base_path + "/up.png").c_str(), targetWidth, targetHeight), {"ABA", "ABA", "AAA", "ABA"});
        tiles[2] = tiles[1].rotate(1);
        tiles[3] = tiles[1].rotate(2);
        tiles[4] = tiles[1].rotate(3);
    }
#elif SCENE == MOUNTAINS
    {
        tiles.resize(5);
        std::string base_path = "images/mountains";
        tiles[0] = tile(loadSpriteWithSize((base_path + "/blank.png").c_str(), targetWidth, targetHeight), {"AAA", "AAA", "AAA", "AAA"});
        tiles[1] = tile(loadSpriteWithSize((base_path + "/up.png").c_str(), targetWidth, targetHeight), {"ABA", "ABA", "AAA", "ABA"});
        tiles[2] = tiles[1].rotate(1);
        tiles[3] = tiles[1].rotate(2);
        tiles[4] = tiles[1].rotate(3);
    }
#elif SCENE == PIPES
    {
        tiles.resize(5);
        std::string base_path = "images/pipes";
        tiles[0] = tile(loadSpriteWithSize((base_path + "/blank.png").c_str(), targetWidth, targetHeight), {"AAA", "AAA", "AAA", "AAA"});
        tiles[1] = tile(loadSpriteWithSize((base_path + "/up.png").c_str(), targetWidth, targetHeight), {"ABA", "ABA", "AAA", "ABA"});
        tiles[2] = tiles[1].rotate(1);
        tiles[3] = tiles[1].rotate(2);
        tiles[4] = tiles[1].rotate(3);
    }
#elif SCENE == POLKA
    {
        tiles.resize(5);
        std::string base_path = "images/polka";
        tiles[0] = tile(loadSpriteWithSize((base_path + "/blank.png").c_str(), targetWidth, targetHeight), {"AAA", "AAA", "AAA", "AAA"});
        tiles[1] = tile(loadSpriteWithSize((base_path + "/up.png").c_str(), targetWidth, targetHeight), {"ABA", "ABA", "AAA", "ABA"});
        tiles[2] = tiles[1].rotate(1);
        tiles[3] = tiles[1].rotate(2);
        tiles[4] = tiles[1].rotate(3);
    }
#elif SCENE == RAIL
    {
        tiles.resize(7);
        std::string base_path = "images/rail/tile";
        const char *patterns[] = {
            "AAA AAA AAA AAA",
            "ABA ABA ABA AAA",
            "BAA AAB AAA AAA",
            "BAA AAA AAB AAA",
            "ABA ABA AAA AAA",
            "ABA AAA ABA AAA",
            "ABA ABA ABA ABA"};

        for (int i = 0; i < 7; ++i)
        {
            std::std::string file_path = std::std::string(base_path) + std::to_std::string(i) + ".png";
            tiles[i] = tile(loadSpriteWithSize(file_path.c_str(), targetWidth, targetHeight),
                            split(patterns[i], ' '));
        }
        for (int i = 1; i < 6; i++)
        {
            for (int j = 1; j < 4; j++)
            {
                tiles.push_back(tiles[i].rotate(j));
            }
        }
        std::cout << tiles.size() << "\n";
    }
#elif SCENE == ROADS
    {
        tiles.resize(5);
        std::string base_path = "images/roads";
        tiles[0] = tile(loadSpriteWithSize((base_path + "/blank.png").c_str(), targetWidth, targetHeight), {"AAA", "AAA", "AAA", "AAA"});
        tiles[1] = tile(loadSpriteWithSize((base_path + "/up.png").c_str(), targetWidth, targetHeight), {"ABA", "ABA", "AAA", "ABA"});
        tiles[2] = tiles[1].rotate(1);
        tiles[3] = tiles[1].rotate(2);
        tiles[4] = tiles[1].rotate(3);
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

#endif