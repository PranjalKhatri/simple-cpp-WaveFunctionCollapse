#ifndef UTIL_H
#define UTIL_H

#include <random>
#include <ctime>
#include <iostream>
#include"raylib.h"
#ifdef NODEBUG
#define LOG(x)
#else
#define LOG(x) std::cout << x << std::endl
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

#endif