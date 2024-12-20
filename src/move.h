#pragma once
#include "vehicle.h"

struct Move
{
   VehicleIdType vehicle_id_;
   int distance_;
   [[nodiscard]] Move operator-() const;
};
