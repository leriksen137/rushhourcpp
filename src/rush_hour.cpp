#include "board.h"
#include "solver.h"

#include <iostream>

int main()
{
   std::cout << "RUSH HOUR\n";
   Board b{ Board::make_board_from_csv("C:\\Users\\leriks\\source\\repos\\RushHour\\levels\\test.csv") };
   Solver s{};
   b.print();
   s.solve(b);
   b.print();
}
