#ifndef TILE_H
#define TILE_H

#include "util.h"
#include <algorithm>

//@brief : compares a with b reversed
bool edge_compare(const std::string &a, const std::string &b)
{
    return (a.size() == b.size()) && std::equal(a.begin(), a.end(), b.rbegin());
}

struct tile
{
    OPSIZE idx;
    std::vector<std::string> edges;
    Texture2D tex;
    std::vector<tile> up, down, left, right;
    tile() = default;

    tile(Texture2D img, std::vector<std::string> edges) : edges(edges), tex(img) {}

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

        std::vector<std::string> rotated = edges;
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

    void analyze(const std::vector<tile> &tiles)
    {
        // skips the 0th index
        for_each(tiles.begin(), tiles.end(), [this](const tile &t)
                 {
                    // if(this->idx == 2){printstd::vector(this->edges);printstd::vector<uint8_t>(t.edges);}
                    // up
                     if (edge_compare(t.edges[2],this->edges[0]))
                     {
                        // if(this->idx == 1)
                        //     LOG("matched up with down of "<<(int)t.idx<<" with "<<"\n");
                        this->up.push_back(t);
                     }
                     // right
                     if (edge_compare(t.edges[3] , this->edges[1]))
                     {
                         this->right.push_back(t);
                     }
                     // down
                     if (edge_compare(t.edges[0] , this->edges[2]))
                     {
                         this->down.push_back(t);
                     }
                     // left
                     if (edge_compare(t.edges[1] , this->edges[3]))
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

#endif