# CMakeList.txt : CMake project for RushHour, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("RushHour")

# Add source to this project's executable.
add_executable (RushHour
"src/rush_hour.cpp"  "src/board.h" "src/board.cpp" "src/vehicle.h"  "src/solver.h"   "src/solver.cpp"  "src/move.h" "src/move.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RushHour PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
