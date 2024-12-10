#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstddef>

#pragma region constants

constexpr const size_t num_rows = 20;
constexpr const size_t num_columns = 20;
constexpr const size_t screenWidth = 400;
constexpr const size_t screenHeight = 400;
constexpr const float tex_width = (float)screenWidth / num_columns;
constexpr const float tex_height = (float)screenHeight / num_rows;

#define LEFT 1
#define RIGHT 2
#define UP 4
#define DOWN 8
#define BLANK 16

const int rules[5][4] = {
    // LEFT
    {
        {UP | DOWN | RIGHT},
        {BLANK | RIGHT},
        {RIGHT | DOWN | LEFT},
        {LEFT | RIGHT | UP}},
    // RIGHT
    {
        {BLANK | LEFT},
        {LEFT | UP | DOWN},
        {RIGHT | DOWN | LEFT},
        {RIGHT | UP | LEFT}},
    // UP
    {
        {RIGHT | UP | DOWN},
        {LEFT | UP | DOWN},
        {RIGHT | LEFT | DOWN},
        {BLANK | DOWN}},
    // DOWN
    {
        {RIGHT | UP | DOWN},
        {LEFT | UP | DOWN},
        {BLANK | UP},
        {RIGHT | LEFT | UP}},
    // BLANK
    {
        {BLANK | LEFT},
        {BLANK | RIGHT},
        {BLANK | UP},
        {BLANK | DOWN}}};

#pragma endregion

#endif