#include "board.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>

Board Board::make_board_from_csv(const std::string& path)
{
   Board b{};
   std::ifstream file(path);
   if (!file.is_open())
   {
      std::cerr << "Could not open the file!\n";
      return b;
   }

   std::string line;

   for (std::size_t y{}; y < board_size; ++y)
   {
      std::getline(file, line);
      std::stringstream lineStream(line);
      std::string cell;

      for (std::size_t x{}; x < board_size; ++x)
      {
         std::getline(lineStream, cell, ',');
         b.grid_[x][y] = static_cast<VehicleIdType>(std::stoi(cell));
      }
   }

   file.close();

   b.update_vehicles_from_grid();
   return b;
}

void Board::print() const
{
   std::cout << "#############\n";

   for (int y{}; y < board_size; ++y)
   {
      for (int x{}; x < board_size; ++x)
      {
         if (x == 0) { std::cout << "#"; }
         else { std::cout << " "; }

         if (grid_[x][y] == no_vehicle) { std::cout << " "; }
         else { std::cout << static_cast<char>(grid_[x][y] + 48); }

         if (x == board_size - 1 && y != 2) { std::cout << "#"; }
      }
      std::cout << "\n";
   }
   std::cout << "#############\n";
}

void Board::apply_moves(const std::vector<Move>& next_moves, const std::vector<Move>& applied_moves)
{
   std::ranges::for_each(std::ranges::reverse_view(applied_moves), [this](const auto& m) { this->undo(m); });
   std::ranges::for_each(next_moves, [this](const auto& m) { this->apply(m); });

   /* TODO try more efficient? don't need to undo everything, only up to the point where "next_moves" and "applied_moves" diverge
   std::size_t branch_idx{};
   while (branch_idx < next_moves.size()
      && branch_idx < applied_moves.size()
      && next_moves[branch_idx] == applied_moves[branch_idx])
   {
      ++branch_idx;
   }

   for (std::size_t undo_idx{ applied_moves.size() - 1 };
      undo_idx >= branch_idx && undo_idx != std::numeric_limits<std::size_t>::max();
      --undo_idx)
   {
      apply(-applied_moves[undo_idx]);
   }

   for (std::size_t apply_idx{ branch_idx }; apply_idx < next_moves.size(); ++apply_idx)
   {
      apply(next_moves[apply_idx]);
   }
   */
}

void Board::apply(const Move& move)
{
   if (vehicles_[move.vehicle_id_].is_vertical_)
   {
      vehicles_[move.vehicle_id_].y_coordinate_ += move.distance_;
   }
   else
   {
      vehicles_[move.vehicle_id_].x_coordinate_ += move.distance_;
   }
   update_grid_from_vehicles();
}

void Board::undo(const Move& move)
{
   apply(-move);
}


std::vector<Move> Board::get_moves()
{
   std::vector<Move> valid_moves{};
   for (VehicleIdType vehicle_id{}; vehicle_id < static_cast<VehicleIdType>(num_vehicles); ++vehicle_id)
   {
      if (vehicles_.contains(vehicle_id))
      {
         const Vehicle& vehicle{ vehicles_[vehicle_id] };
         if (vehicle.is_vertical_)
         {
            int max_tested_positive_displacement{ 1 };
            while (is_position_valid(vehicle.x_coordinate_, vehicle.y_coordinate_ + vehicle.length_ - 1 + max_tested_positive_displacement)
               && grid_[vehicle.x_coordinate_][vehicle.y_coordinate_ + vehicle.length_ - 1 + max_tested_positive_displacement] == no_vehicle)
            {
               ++max_tested_positive_displacement;
            }

            int max_tested_negative_displacement{ -1 };
            while (is_position_valid(vehicle.x_coordinate_, vehicle.y_coordinate_ + max_tested_negative_displacement)
               && grid_[vehicle.x_coordinate_][vehicle.y_coordinate_ + max_tested_negative_displacement] == no_vehicle)
            {
               --max_tested_negative_displacement;
            }

            for (int displacement{ 1 }; displacement < max_tested_positive_displacement; ++displacement)
            {
               valid_moves.push_back({ vehicle_id, displacement });
            }
            for (int displacement{ -1 }; displacement > max_tested_negative_displacement; --displacement)
            {
               valid_moves.push_back({ vehicle_id, displacement });
            }
         }
         else
         {
            int max_tested_positive_displacement{ 1 };
            while (is_position_valid(vehicle.x_coordinate_ + vehicle.length_ - 1 + max_tested_positive_displacement, vehicle.y_coordinate_)
               && grid_[vehicle.x_coordinate_ + vehicle.length_ - 1 + max_tested_positive_displacement][vehicle.y_coordinate_] == no_vehicle)
            {
               ++max_tested_positive_displacement;
            }

            int max_tested_negative_displacement{ -1 };
            while (is_position_valid(vehicle.x_coordinate_ + max_tested_negative_displacement, vehicle.y_coordinate_)
               && grid_[vehicle.x_coordinate_ + max_tested_negative_displacement][vehicle.y_coordinate_] == no_vehicle)
            {
               --max_tested_negative_displacement;
            }

            for (int displacement{ 1 }; displacement < max_tested_positive_displacement; ++displacement)
            {
               valid_moves.push_back({ vehicle_id, displacement });
            }
            for (int displacement{ -1 }; displacement > max_tested_negative_displacement; --displacement)
            {
               valid_moves.push_back({ vehicle_id, displacement });
            }
         }
      }
   }
   std::ranges::sort(valid_moves, [](const Move& a, const Move& b) { return std::abs(a.distance_) > std::abs(b.distance_); });
   return valid_moves;
}

bool Board::is_solved() const
{
   return grid_[5][2] == 0;
}

bool Board::is_position_valid(int x, int y)
{
   return x >= 0 && y >= 0 && x < board_size && y < board_size;
}

// TODO inefficient
void Board::update_grid_from_vehicles()
{
   // wipe everything
   grid_.fill({ no_vehicle, no_vehicle, no_vehicle, no_vehicle, no_vehicle, no_vehicle }); // TODO fails if board size changes

   // fill grid from vehicles
   for (VehicleIdType vehicle_id{}; vehicle_id < static_cast<VehicleIdType>(num_vehicles); ++vehicle_id)
   {
      if (vehicles_.contains(vehicle_id))
      {
         if (vehicles_[vehicle_id].is_vertical_)
         {
            for (int l{}; l < vehicles_[vehicle_id].length_; ++l)
            {
               grid_[vehicles_[vehicle_id].x_coordinate_][vehicles_[vehicle_id].y_coordinate_ + l] = vehicle_id;
            }
         }
         else
         {
            for (int l{}; l < vehicles_[vehicle_id].length_; ++l)
            {
               grid_[vehicles_[vehicle_id].x_coordinate_ + l][vehicles_[vehicle_id].y_coordinate_] = vehicle_id;
            }
         }
      }
   }
}


void Board::update_vehicles_from_grid()
{
   for (int x{}; x < board_size; ++x)
   {
      for (int y{}; y < board_size; ++y)
      {
         if (grid_[x][y] != no_vehicle)
         {
            VehicleIdType vehicle_id{ grid_[x][y] };
            if (vehicles_.contains(vehicle_id))
            {
               vehicles_[vehicle_id].length_++;
               if (x > vehicles_[vehicle_id].x_coordinate_)
               {
                  vehicles_[vehicle_id].is_vertical_ = false;
               }
            }
            else
            {
               vehicles_.insert({ vehicle_id, {vehicle_id, 1, x, y, true} });
            }
         }
      }
   }
}

// TODO this function is not tested, it probably works? who knows
std::size_t hash_value(const Board& board)
{
   std::hash<VehicleIdType> hasher{};
   std::size_t hash_value{};
   for (const auto& row : board.grid_) {
      for (uint8_t cell : row) {
         hash_value ^= hasher(cell) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
      }
   }
   return hash_value;
}
