#ifndef MODEL_H
#define MODEL_H

#include <cstdint>
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <bitset>
#include "util.h"

bool applyLogic = true;
using std::vector, std::shared_ptr, std::make_shared, std::bitset, std::endl, std::cerr;

struct cell
{
    bool collapsed;          // whether the cell is currently occupied or not
    bitset<NUM_OPS> options; // number of options this cell has (using bit enum for very small entropies)
};
struct tile
{
    OPSIZE idx;
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
        for_each(tiles.begin(), tiles.end(), [this](const tile &t)
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
        grid[i]->options = ~0; // 1111 in binary (all 4 options available)
    }
}

void logOptions()
{
    for (size_t i = 0; i < num_rows; ++i)
    {
        for (size_t j = 0; j < num_columns; ++j)
        {
            size_t index = i * num_columns + j;            // Calculate the 1D index from 2D coordinates
            LOG(grid[index]->options.to_string() << "\t"); // Log the number of options
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
            // LOG("valid option calulation for "<<index<<"\n");
            if (grid[index]->collapsed)
                continue;

            bitset<NUM_OPS> valid_options = ~0;
            if (i > 0)
            {
                std::bitset<NUM_OPS> up_options; // Initialize all bits to 0
                auto up = grid[index - num_columns];
                for (size_t tileIndex = up->options._Find_first();
                     tileIndex < NUM_OPS;
                     tileIndex = up->options._Find_next(tileIndex))
                {
                    // std::cerr << "accessing tile index " << tileIndex << "\n";
                    for (auto &t : tiles[tileIndex].down)
                    {
                        up_options.set(t.idx); // Set the corresponding valid options
                    }
                }
                valid_options &= up_options; // Intersect with current valid options
            }
            // LOG("valid options after up " << valid_options.count() << " ");

            // LOOK DOWN
            if (i < num_rows - 1)
            {
                std::bitset<NUM_OPS> down_options; // Initialize all bits to 0
                auto down = grid[index + num_columns];

                for (size_t tileIndex = down->options._Find_first();
                     tileIndex < NUM_OPS;
                     tileIndex = down->options._Find_next(tileIndex))
                {
                    // std::cerr << "accessing tile index " << tileIndex << "\n";
                    for (auto &t : tiles[tileIndex].up)
                    {
                        down_options.set(t.idx); // Set the corresponding valid options
                    }
                }
                valid_options &= down_options; // Intersect with current valid options
            }
            // LOG("valid options after down " << valid_options.count() << " ");

            // LOOK LEFT
            if (j > 0)
            {
                std::bitset<NUM_OPS> left_options; // Initialize all bits to 0
                auto left = grid[index - 1];

                for (size_t tileIndex = left->options._Find_first();
                     tileIndex < NUM_OPS;
                     tileIndex = left->options._Find_next(tileIndex))
                {
                    // std::cerr << "accessing tile index " << tileIndex << "\n";
                    for (auto &t : tiles[tileIndex].right)
                    {
                        left_options.set(t.idx); // Set the corresponding valid options
                    }
                }
                valid_options &= left_options; // Intersect with current valid options
            }
            // LOG("valid options after left " << valid_options.count() << " ");

            // LOOK RIGHT
            if (j < num_columns - 1)
            {
                std::bitset<NUM_OPS> right_options; // Initialize all bits to 0
                auto right = grid[index + 1];

                for (size_t tileIndex = right->options._Find_first();
                     tileIndex < NUM_OPS;
                     tileIndex = right->options._Find_next(tileIndex))
                {
                    // std::cerr << "accessing tile index " << tileIndex << "\n";
                    for (auto &t : tiles[tileIndex].left)
                    {
                        right_options.set(t.idx); // Set the corresponding valid options
                    }
                }
                valid_options &= right_options; // Intersect with current valid options
            }
            // LOG("valid options after right " << valid_options.count() << " ");

            /*
                        // LOOK UP
                        // if (i > 0)
                        // {
                        //     bitset<NUM_OPS> up_options{};
                        //     auto up = grid[index - num_columns];

                        //     for (OPSIZE option = up->options; option; option &= (option - 1))
                        //     {
                        //         int tileIndex = __builtin_ctz(option);
                        //         for (auto &t : tiles[tileIndex].down)
                        //         {
                        //             up_options |= t.idx;
                        //         }
                        //     }
                        //     valid_options &= up_options;
                        // }
                        // // LOG("valid options after up "<<(int)valid_options<<" ");
                        // // LOOK DOWN
                        // if (i < num_rows - 1)
                        // {
                        //     OPSIZE down_options{};
                        //     auto down = grid[index + num_columns];
                        //     for (OPSIZE option = down->options; option; option &= (option - 1))
                        //     {
                        //         int tileIndex = __builtin_ctz(option);
                        //         for (auto &t : tiles[tileIndex].up)
                        //         {
                        //             down_options |= t.idx;
                        //         }
                        //     }
                        //     valid_options &= down_options;
                        // }
                        // // LOG("valid options after down "<<(int)valid_options<<" ");

                        // // LOOK LEFT
                        // if (j > 0)
                        // {
                        //     OPSIZE left_options{};
                        //     auto left = grid[index - 1];
                        //     for (OPSIZE option = left->options; option; option &= (option - 1))
                        //     {
                        //         int tileIndex = __builtin_ctz(option);
                        //         for (auto &t : tiles[tileIndex].right)
                        //         {
                        //             left_options |= t.idx;
                        //         }
                        //     }
                        //     valid_options &= left_options;
                        // }

                        // // LOG("valid options after left "<<(int)valid_options<<" ");
                        // // LOOK RIGHT
                        // if (j < num_columns - 1)
                        // {
                        //     OPSIZE right_options{};
                        //     auto right = grid[index + 1];
                        //     for (OPSIZE option = right->options; option; option &= (option - 1))
                        //     {
                        //         int tileIndex = __builtin_ctz(option);
                        //         for (auto &t : tiles[tileIndex].left)
                        //         {
                        //             right_options |= t.idx;
                        //         }
                        //     }
                        //     valid_options &= right_options;
                        // }
                        // LOG("valid options after right "<<(int)valid_options<<"\n");
            */
            // std::cout << "valid options after all " << (int)valid_options << "\n";
            grid[index]->options = valid_options;
            grid[index]->collapsed = valid_options.count() == 1;
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
              { return a.first->options.count() < b.first->options.count(); });

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

    std::bitset<NUM_OPS> options = grid_cpy[stopIndex].first->options;
    std::vector<int> indices;

    for (size_t i = options._Find_first(); i < NUM_OPS; i = options._Find_next(i))
    {
        indices.push_back(static_cast<int>(i)); // Store the index of the set bit
    }

    grid_cpy[stopIndex].first->collapsed = true;
    try
    {
        grid_cpy[stopIndex].first->options = 0;
        grid_cpy[stopIndex].first->options.set(pickRandom(indices));
    }
    catch (...)
    {
        applyLogic = false;
        return;
    }
    propagateConstraints(grid, tiles);
}

#endif