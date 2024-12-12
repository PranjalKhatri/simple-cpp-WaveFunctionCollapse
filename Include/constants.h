#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstddef>

#pragma region constants
#define OPSIZE uint64_t
#define DEFAULT 5
#define CIRCUIT 38
#define NUM_OPS DEFAULT
constexpr const size_t num_rows = 50;
constexpr const size_t num_columns = 50;
constexpr const size_t screenWidth = 800;
constexpr const size_t screenHeight = 800;
constexpr const float tex_width = (float)screenWidth / num_columns;
constexpr const float tex_height = (float)screenHeight / num_rows;

/*
#define UP 1
#define RIGHT 2
#define DOWN 4
#define LEFT 8
#define BLANK 16
const int rules[5][4] = {
    // UP
    {
        {RIGHT | LEFT | DOWN},
        {LEFT | UP | DOWN},
        {BLANK | DOWN},
        {RIGHT | UP | DOWN}},
    // RIGHT
    {
        {RIGHT | DOWN | LEFT},
        {LEFT | UP | DOWN},
        {RIGHT | UP | LEFT},
        {BLANK | LEFT}},
    // DOWN
    {
        {BLANK | UP},
        {LEFT | UP | DOWN},
        {RIGHT | LEFT | UP},
        {RIGHT | UP | DOWN}},
    // LEFT
    {
        {RIGHT | DOWN | LEFT},
        {BLANK | RIGHT},
        {LEFT | RIGHT | UP},
        {UP | DOWN | RIGHT}},
    // BLANK
    {
        {BLANK | UP},
        {BLANK | RIGHT},
        {BLANK | DOWN},
        {BLANK | LEFT}}};
*/
#pragma endregion

#endif