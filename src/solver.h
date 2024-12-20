#pragma once
#include <set>
#include <vector>
#include "board.h"
#include "move.h"

class Memory
{
public:
   void add_node(const Board& board);
   [[nodiscard]] bool is_visited (const Board& b) const;

private:
   std::set<std::size_t> visited_hashes{};
};

class Solver
{
public:
   void solve(Board& b);

private:
   [[nodiscard]] std::vector<Move> run_breadth_first_search(Board& b);
   [[nodiscard]] bool bfs(Board& board, std::vector<Move>& previous_moves, const Move& current_move);
   Memory memory_;
};
