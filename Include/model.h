#ifndef MODEL_H
#define MODEL_H

#include <cstdint>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include "tile.h"

using std::vector, std::shared_ptr, std::make_shared, std::bitset, std::endl, std::cerr, std::string;

struct cell
{
    bool collapsed;          // whether the cell is currently occupied or not
    bitset<NUM_OPS> options; // number of options this cell has (using bit enum for very small entropies)
};
class WFCMODEL
{
private:
    size_t num_rows, num_columns;
    size_t tile_options;

public:
    bool canApplyLogic = true;

private:
    void initializeGrid()
    {
        for (size_t i = 0; i < grid.size(); i++)
        {
            grid[i] = make_shared<cell>(); // Create shared pointer for each cell
            grid[i]->collapsed = false;
            grid[i]->options = ~0; // 1111 in binary (all 4 options available)
        }
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
                grid[index]->options = valid_options;
                grid[index]->collapsed = valid_options.count() == 1;
            }
        }
        logOptions();
    }

public:
    // 1D grid of shared pointers
    vector<shared_ptr<cell>> grid;
    WFCMODEL(size_t rows, size_t columns) : num_rows(rows), num_columns(columns), grid(rows * columns) {}

    void initializeModel(const vector<tile> &tiles)
    {
        canApplyLogic = true;
        initializeGrid(); // all shared pointers are freed automatically
        int idx = randomRange(0, grid.size());
        grid[idx]->collapsed = true;
        grid[idx]->options = 0;
        grid[idx]->options.set(randomRange(0, tiles.size() - 1));
        // logOptions();
        propagateConstraints(grid, tiles);
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
        {
            canApplyLogic = false;
            return;
        }
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
            initializeModel(tiles);
            return;
        }
        propagateConstraints(grid, tiles);
    }
};

void displayTileOptions(const vector<tile> &tiles)
{
    for (int i = 0; i < tiles.size(); i++)
    {
        std::cout << "tile options at " << (long long)tiles[i].idx << endl;
        std::cout << "up\n";
        for (const auto &j : tiles[i].up)
            std::cout << (long long)j.idx << " ";
        std::cout << "\nright\n";
        for (const auto &j : tiles[i].right)
            std::cout << (long long)j.idx << " ";
        std::cout << "\ndown\n";
        for (const auto &j : tiles[i].down)
            std::cout << (long long)j.idx << " ";
        std::cout << "\nleft\n";
        for (const auto &j : tiles[i].left)
            std::cout << (long long)j.idx << " ";
        std::cout << endl;
    }
}

#endif