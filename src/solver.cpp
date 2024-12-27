#include "solver.h"

#include <iostream>

void Solver::mark_board_as_visited(const Board& board)
{
   visited_hashes.insert(hash_value(board));
}

bool Solver::is_board_visited(const Board& board) const
{
   return visited_hashes.contains(hash_value(board));
}

bool Solver::solve(Board& board)
{
   for (const auto& m : board.get_moves())
   {
      std::vector initial_move{ m };
      considered_moves_.push_back(initial_move);
   }

   while (!considered_moves_.empty())
   {
      max_size_deque = std::max(considered_moves_.size(), max_size_deque);

      const std::vector next_moves{ considered_moves_.front() };
      considered_moves_.pop_front();

      board.apply_moves(next_moves, applied_moves);
      applied_moves = next_moves;

      if (is_board_visited(board))
      {
         continue;
      }
      mark_board_as_visited(board);

      ++num_solver_steps;
#ifdef _DEBUG
      if (num_solver_steps % 100 == 0 && num_solver_steps != 0)
      {
         std::cout << "num steps: " << num_solver_steps << "\n";
         std::cout << "current deque size: " << considered_moves_.size() << "\n";
         std::cout << "visited positions: " << visited_hashes.size() << "\n";
         std::cout << "current board:\n";
         board.print();
      }
#endif

      if (board.is_solved())
      {
         return true;
      }

      for (const auto& move : board.get_moves())
      {
         std::vector new_moves{ applied_moves };
         new_moves.push_back(move);
         considered_moves_.push_back(new_moves);
      }

   }

   return false;
}
