#pragma once
#include <cstddef>

// G++ requirement to compare a size_t with -1 without throwing warnings
namespace ID
{
    const static size_t invalid_id = -1;
};