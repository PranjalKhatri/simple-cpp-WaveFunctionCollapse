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
    vector<uint8_t> edges;
    Texture2D tex;
    vector<tile> up, down, left, right;
    tile() = default;

    tile(Texture2D img, vector<uint8_t> edges) : edges(edges), tex(img) {}

    tile(const tile &other) : edges(other.edges)
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
            Image img = LoadImageFromTexture(other.tex);
            tex = LoadTextureFromImage(img);
            UnloadImage(img);
        }
        return *this;
    }

    tile(tile &&other) noexcept : edges(std::move(other.edges)), tex(other.tex)
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

    void analyze(vector<tile> &tiles)
    {
        for_each(tiles.begin(), tiles.end(), [this](const tile& t)
                 {
                     // up
                     if (t.edges[2] == this->edges[0])
                     {
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
                     }
                 });
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
        LOG(""); // Newline at the end of each row
    }
}

void propagateConstraints(size_t index, vector<shared_ptr<cell>> &grid)
{
    LOG("Propagating for " << (int)grid[index]->options);
    size_t row = index / num_columns;
    size_t col = index % num_columns;

    // Check each direction (LEFT, RIGHT, UP, DOWN)
    if (col > 0) // Left
    {
        size_t leftIndex = index - 1;
        if (!grid[leftIndex]->collapsed)
        {
            LOG("Left is updated;\nbefore " << (int)grid[leftIndex]->options);
            grid[leftIndex]->options &= rules[__builtin_ffs(grid[index]->options) - 1][3]; // Apply LEFT direction rules
            LOG("after " << (int)grid[leftIndex]->options);
        }
    }

    if (col < num_columns - 1) // Right
    {
        size_t rightIndex = index + 1;
        if (!grid[rightIndex]->collapsed)
        {
            LOG("Left is updated;\nbefore " << (int)grid[rightIndex]->options);
            grid[rightIndex]->options &= rules[__builtin_ffs(grid[index]->options) - 1][1]; // Apply RIGHT direction rules
            LOG("after " << (int)grid[rightIndex]->options);
        }
    }

    if (row > 0) // Up
    {
        size_t upIndex = index - num_columns;
        if (!grid[upIndex]->collapsed)
        {
            LOG("Left is updated;\nbefore " << (int)grid[upIndex]->options);
            grid[upIndex]->options &= rules[__builtin_ffs(grid[index]->options) - 1][0]; // Apply UP direction rules
            LOG("after " << (int)grid[upIndex]->options);
        }
    }

    if (row < num_rows - 1) // Down
    {
        size_t downIndex = index + num_columns;
        if (!grid[downIndex]->collapsed)
        {
            LOG("Left is updated;\nbefore " << (int)grid[downIndex]->options);
            grid[downIndex]->options &= rules[__builtin_ffs(grid[index]->options) - 1][2]; // Apply DOWN direction rules
            LOG("after " << (int)grid[downIndex]->options);
        }
    }
    logOptions();
    LOG("");
}

void logic()
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
    propagateConstraints(grid_cpy[stopIndex].second, grid);
}

#endif