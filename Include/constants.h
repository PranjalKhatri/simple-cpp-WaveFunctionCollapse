#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstddef>

#pragma region constants

#define OPSIZE uint64_t
//-----SCENE TYPES--------
#define DEFAULT 0
#define OPS_DEFAULT 5

#define CIRCUIT 1 
#define OPS_CIRCUIT 46

#define CIRCUIT_CTRAIN 2
#define OPS_CIRCUIT_CTRAIN 46
#define NUM_OPS OPS_DEFAULT


constexpr const size_t num_rows = 40;
constexpr const size_t num_columns = 40;
constexpr const size_t screenWidth = 800;
constexpr const size_t screenHeight = 800;
constexpr const float tex_width = (float)screenWidth / num_columns;
constexpr const float tex_height = (float)screenHeight / num_rows;

#pragma endregion

#endif