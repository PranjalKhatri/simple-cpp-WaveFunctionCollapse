#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstddef>

#pragma region constants

#define OPSIZE uint64_t
//-----SCENE TYPES--------
#define DEFAULT 0
#define OPS_DEFAULT 5
#define NUM_OPS OPS_DEFAULT

#define CIRCUIT 1 
#define OPS_CIRCUIT 46

#define CIRCUIT_CTRAIN 2
#define OPS_CIRCUIT_CTRAIN 46

#define DEMO_TRACKS 3
#define OPS_DEMO_TRACKS 5

#define MOUNTAINS 4
#define OPS_MOUNTAINS 5

#define PIPES 5
#define OPS_PIPES 5

#define POLKA 6
#define OPS_POLKA 5

#define RAIL 7
#define OPS_RAIL 22

#define ROADS 8
#define OPS_ROADS 5

constexpr const size_t num_rows = 25;
constexpr const size_t num_columns = 25;
constexpr const size_t screenWidth = 800;
constexpr const size_t screenHeight = 800;
constexpr const float tex_width = (float)screenWidth / num_columns;
constexpr const float tex_height = (float)screenHeight / num_rows;

#pragma endregion

#endif