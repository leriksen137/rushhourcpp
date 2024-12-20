#pragma once
#include <array>
#include <string>
#include <vector>
#include <functional>
#include <map>

#include "move.h"
#include "vehicle.h"

static constexpr auto board_size{ 6 };
using BoardGridStateType = std::array<std::array<VehicleIdType, board_size>, board_size>;
using BoardVehicleStateType = std::map<VehicleIdType, Vehicle>;

class Board
{
public:
   void apply(const Move& move);
   void print() const;
   [[nodiscard]] static Board make_board_from_csv(const std::string& path);
   [[nodiscard]] std::vector<Move> get_moves();
   [[nodiscard]] bool is_solved() const;
   [[nodiscard]] friend std::size_t hash_value(const Board& board);

private:
   [[nodiscard]] static bool is_position_valid(int x, int y);
   BoardGridStateType grid_{};
   BoardVehicleStateType vehicles_{};
   void update_grid_from_vehicles();
   void update_vehicles_from_grid();
};
