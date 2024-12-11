#ifndef MODEL_H
#define MODEL_H

#include <cstdint>
#include <vector>
#include <memory>
#include "constants.h"
#include "util.h"
#include <iostream>
#include <algorithm>

using std::vector, std::shared_ptr, std::make_shared;

struct cell
{
    bool collapsed;  // whether the cell is currently occupied or not
    uint8_t options; // number of options this cell has (using bit enum for very small entropies)
};
struct tile
{
    uint8_t idx;
    vector<uint8_t> edges;
    Texture2D tex;
    vector<tile> up, down, left, right;
    tile() = default;

    tile(Texture2D img, vector<uint8_t> edges) : edges(edges), tex(img) {}

    tile(const tile &other) : edges(other.edges), idx(other.idx)
    {
        Image img = LoadImageFromTexture(other.tex);
        tex = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    tile &operator=(const tile &other)
    {
        if (this != &other)
        {
            UnloadTexture(tex);
            edges = other.edges;
            idx = other.idx;
            Image img = LoadImageFromTexture(other.tex);
            tex = LoadTextureFromImage(img);
            UnloadImage(img);
        }
        return *this;
    }

    tile(tile &&other) noexcept : edges(std::move(other.edges)), idx(other.idx), tex(other.tex)
    {
        other.tex.id = 0; // Nullify the texture ID to prevent double unloading
    }

    tile &operator=(tile &&other) noexcept
    {
        if (this != &other)
        {
            UnloadTexture(tex);
            edges = std::move(other.edges);
            tex = other.tex;
            idx = other.idx;
            other.tex.id = 0;
        }
        return *this;
    }

    tile rotate(size_t num)
    {
        num %= 4;

        vector<uint8_t> rotated = edges;
        std::rotate(rotated.rbegin(), rotated.rbegin() + num, rotated.rend());

        Image img = LoadImageFromTexture(this->tex);

        for (size_t i = 0; i < num; ++i)
        {
            ImageRotateCW(&img);
        }

        Texture2D rotatedTexture = LoadTextureFromImage(img);
        UnloadImage(img);

        return tile(rotatedTexture, rotated);
    }

    void analyze(const vector<tile> &tiles)
    {
        // skips the 0th index
        for_each(tiles.begin(), tiles.end()-1, [this](const tile &t)
                 {
                    // if(this->idx == 2){printvector(this->edges);printvector<uint8_t>(t.edges);}
                    // up
                     if (t.edges[2] == this->edges[0])
                     {
                        // if(this->idx == 1)
                        //     LOG("matched up with down of "<<(int)t.idx<<" with "<<"\n");
                        this->up.push_back(t);
                     }
                     // right
                     if (t.edges[3] == this->edges[1])
                     {
                         this->right.push_back(t);
                     }
                     // down
                     if (t.edges[0] == this->edges[2])
                     {
                         this->down.push_back(t);
                     }
                     // left
                     if (t.edges[1] == this->edges[3])
                     {
                         this->left.push_back(t);
                     } });
    }

    ~tile()
    {
        // std::cout<<"tile Destroyed! "<<tex.id<<"\n";
        if (tex.id != 0)
        {
            UnloadTexture(tex);
        }
    }
};

// 1D grid of shared pointers
vector<shared_ptr<cell>> grid(num_rows *num_columns);

void initializeGrid()
{
    for (size_t i = 0; i < grid.size(); i++)
    {
        grid[i] = make_shared<cell>(); // Create shared pointer for each cell
        grid[i]->collapsed = false;
        grid[i]->options = 0x1F; // 1111 in binary (all 4 options available)
    }
}

void logOptions()
{
    for (size_t i = 0; i < num_rows; ++i)
    {
        for (size_t j = 0; j < num_columns; ++j)
        {
            size_t index = i * num_columns + j;     // Calculate the 1D index from 2D coordinates
            LOG((int)grid[index]->options << "\t"); // Log the number of options
        }
        LOG("" << std::endl); // Newline at the end of each row
    }
    LOG("\n");
}
// TODO:use a BFS like approach in future
void propagateConstraints(vector<shared_ptr<cell>> &grid, const vector<tile> &tiles)
{
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_columns; j++)
        {
            int index = i * num_columns + j;
            LOG("valid option calulation for "<<index<<"\n");
            if (grid[index]->collapsed)
                continue;

            uint8_t valid_options = ~0;

            // LOOK UP
            if (i > 0)
            {
                uint8_t up_options{};
                auto up = grid[index - num_columns];
                for (uint8_t option = up->options; option; option &= (option - 1))
                {
                    int tileIndex = __builtin_ctz(option);
                    for (auto &t : tiles[tileIndex].down)
                    {
                        up_options |= t.idx;
                    }
                }
                valid_options &= up_options;
            }
            LOG("valid options after up "<<(int)valid_options<<" ");
            // LOOK DOWN
            if (i < num_rows - 1)
            {
                uint8_t down_options{};
                auto down = grid[index + num_columns];
                for (uint8_t option = down->options; option; option &= (option - 1))
                {
                    int tileIndex = __builtin_ctz(option);
                    for (auto &t : tiles[tileIndex].up)
                    {
                        down_options |=t.idx;
                    }
                }
                valid_options &= down_options;
            }
            LOG("valid options after down "<<(int)valid_options<<" ");

            // LOOK LEFT
            if (j > 0)
            {
                uint8_t left_options{};
                auto left = grid[index - 1];
                for (uint8_t option = left->options; option; option &= (option - 1))
                {
                    int tileIndex = __builtin_ctz(option);
                    for (auto &t : tiles[tileIndex].right)
                    {
                        left_options |= t.idx;
                    }
                }
                valid_options &= left_options;
            }

            LOG("valid options after left "<<(int)valid_options<<" ");
            // LOOK RIGHT
            if (j < num_columns - 1)
            {
                uint8_t right_options{};
                auto right = grid[index + 1];
                for (uint8_t option = right->options; option; option &= (option - 1))
                {
                    int tileIndex = __builtin_ctz(option);
                    for (auto &t : tiles[tileIndex].left)
                    {
                        right_options |= t.idx;
                    }
                }
                valid_options &= right_options;
            }
            LOG("valid options after right "<<(int)valid_options<<"\n");

            // std::cout << "valid options after all " << (int)valid_options << "\n";
            grid[index]->options = valid_options;
            grid[index]->collapsed = __builtin_popcount(valid_options) == 1;
        }
    }
    logOptions();
}

void logic(const vector<tile> &tiles)
{
    std::vector<std::pair<std::shared_ptr<cell>, size_t>> grid_cpy;

    for (size_t i = 0; i < grid.size(); ++i)
    {
        grid_cpy.push_back({grid[i], i});
    }

    std::sort(grid_cpy.begin(), grid_cpy.end(), [](const std::pair<std::shared_ptr<cell>, size_t> &a, const std::pair<std::shared_ptr<cell>, size_t> &b)
              { return __builtin_popcount(a.first->options) < __builtin_popcount(b.first->options); });

    size_t stopIndex = -1;
    for (size_t i = 0; i < grid_cpy.size(); ++i)
    {
        if (!grid_cpy[i].first->collapsed)
        {
            stopIndex = i;
            break;
        }
    }

    if (stopIndex == -1)
        return;

    size_t num = grid_cpy[stopIndex].first->options;
    vector<int> powersOf2;

    for (int i = 0; num > 0; ++i)
    {
        if (num & 1)
        {
            powersOf2.push_back(static_cast<int>(1 << i));
        }
        num >>= 1;
    }

    grid_cpy[stopIndex].first->collapsed = true;
    grid_cpy[stopIndex].first->options = pickRandom(powersOf2);
    propagateConstraints(grid, tiles);
}

#endif