#pragma once
#include <vector>

#include "vehicle.h"

struct Move
{
   VehicleIdType vehicle_id_;
   int distance_;
   [[nodiscard]] Move operator-() const;
   auto operator<=>(std::vector<Move>::const_reference move) const = default;
};
