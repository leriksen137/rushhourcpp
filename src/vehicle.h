#pragma once
#include <cstddef>
#include <cstdint>

using VehicleIdType = std::int8_t;
static constexpr VehicleIdType no_vehicle{ -1 };
static constexpr std::size_t num_vehicles{ 16 };

struct Vehicle
{
   VehicleIdType id_;
   int length_;
   int x_coordinate_;
   int y_coordinate_;
   bool is_vertical_;
};
