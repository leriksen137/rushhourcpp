#include <algorithm>

#include "board.h"
#include "solver.h"

#include <iostream>
#include <ranges>

void run(const std::string& path)
{
   Board b{ Board::make_board_from_csv(path) };
   Solver s{};
   b.print();
   if (s.solve(b))
   {
      std::cout << "success!\n";
      std::cout << "steps applied = " << s.num_solver_steps << "\n";
      std::cout << "max size deque = " << s.max_size_deque << "\n";
      std::cout << "moves:\n";
      std::ranges::for_each(std::ranges::reverse_view(s.applied_moves), [&](const auto& m) { b.undo(m); });
      std::ranges::for_each(s.applied_moves, [&](const auto& m)
         {
            b.apply(m);
            b.print();
         });
   }
   else
   {
      std::cout << "something went wrong lol\n";
   }
   std::cout << "--------------\n";
   std::cout << "--------------\n";
   std::cout << "--------------\n";
}

int main()
{
   std::cout << "RUSH HOUR\n---------\n";

   run("C:\\Users\\leriks\\source\\repos\\RushHour\\levels\\lvl0.csv"); // TODO make path relative. only works on my computer right now.
   run("C:\\Users\\leriks\\source\\repos\\RushHour\\levels\\lvl1.csv");
   run("C:\\Users\\leriks\\source\\repos\\RushHour\\levels\\lvl2.csv");
   run("C:\\Users\\leriks\\source\\repos\\RushHour\\levels\\lvl3.csv");
}
