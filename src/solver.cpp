#include "solver.h"

#include <iostream>

void Memory::add_node(const Board& board)
{
   visited_hashes.insert(hash_value(board));
}

bool Memory::is_visited(const Board& board) const
{
   return visited_hashes.contains(hash_value(board));
}

void Solver::solve(Board& b)
{
   for (const auto& move : run_breadth_first_search(b))
   {
      std::cout << "move car " << static_cast<int>(move.vehicle_id_) << " by " << move.distance_ << "\n";
   }
}

std::vector<Move> Solver::run_breadth_first_search(Board& b)
{
   std::vector<Move> winning_moves{};

   [[maybe_unused]] const bool success = bfs(b, winning_moves, { 0, 0 });

   return winning_moves;
}

bool Solver::bfs(Board& board, std::vector<Move>& previous_moves, const Move& current_move)
{
   board.apply(current_move);
   previous_moves.push_back(current_move);

   if (board.is_solved())
   {
      return true;
   }

   if (!memory_.is_visited(board))
   {
      memory_.add_node(board);

      for (const auto& move : board.get_moves())
      {
         if (bfs(board, previous_moves, move))
         {
            return true;
         }
      }
   }

   previous_moves.pop_back();
   board.apply(-current_move);
   return false;
}
