#include "move.h"

Move Move::operator-() const
{
   return Move{ vehicle_id_, -distance_ };
}
