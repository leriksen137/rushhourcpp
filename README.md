# RushHourCpp

A C++ implementation of a solver for the [Rush Hour puzzle game](https://en.wikipedia.org/wiki/Rush_Hour_(puzzle)).

The current implementation finds the shortest path to the solution using [BFS](https://en.wikipedia.org/wiki/Breadth-first_search). The graph is not explicitly generated, insted the check whether a node (board position) has already been visited is performed by storing a hash of the board content in a set.
