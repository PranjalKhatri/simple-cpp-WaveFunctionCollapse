#ifndef UTIL_H
#define UTIL_H

#include <random>
#include <ctime>
#include <string>
#include <iostream>
#include "raylib.h"
#include "constants.h"
#include <vector>

#ifdef NODEBUG
#define LOG(x)
#else
#define LOG(x) std::cout << x
#endif

int randomRange(int min, int max)
{
    return min + rand() % (max - min + 1); // Generate a random number within the specified range
}

int pickRandom(const std::vector<int> &vec)
{
    if (vec.empty())
    {
        throw std::invalid_argument("The vector is empty!");
    }
    int randomIndex = rand() % vec.size();
    return vec[randomIndex];
}

Texture2D loadSprite(const char *path)
{
    Texture2D sprite = LoadTexture(path);
    if (sprite.id == 0)
    {
        std::cerr << "Failed to load texture: " << path << std::endl;
        exit(1);
    }
    return sprite;
}

template <typename T>
void printvector(const std::vector<T> &arr)
{
    for (auto &i : arr)
        LOG((int)i << " ");
    LOG("\n");
}
// Helper function to load and resize a sprite to a specific size (width and height)
Texture2D loadSpriteWithSize(const char *fileName, int targetWidth, int targetHeight)
{
    // Load the texture
    Texture2D texture = LoadTexture(fileName);

    // Convert texture to image
    Image image = LoadImageFromTexture(texture);

    // Resize the image to the target width and height
    ImageResize(&image, targetWidth, targetHeight);

    // Load the resized image into a new texture
    Texture2D resizedTexture = LoadTextureFromImage(image);

    // Unload the original texture and image
    UnloadTexture(texture);
    UnloadImage(image);

    return resizedTexture;
}

std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0, end;

    while ((end = str.find(delimiter, start)) != std::string::npos)
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
    }

    tokens.push_back(str.substr(start)); // Add the last token
    return tokens;
}

#endif