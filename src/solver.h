#pragma once
#include <queue>
#include <set>
#include <vector>
#include "board.h"
#include "move.h"

class Solver
{
public:
   bool solve(Board& board);

   std::size_t num_solver_steps{};
   std::size_t max_size_deque{};

   std::vector<Move> applied_moves{};

private:
   void mark_board_as_visited(const Board& board);
   std::set<std::size_t> visited_hashes{};
   [[nodiscard]] bool is_board_visited(const Board& b) const;
   std::deque<std::vector<Move>> considered_moves_;
};
