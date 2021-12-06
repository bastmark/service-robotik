#include <stdlib.h>
#include "maze.h"
#include "direction.h"

Maze maze;

int main() {
	srand(time(NULL));

	//const char filename[] = "course_matrix.txt";

	const char filename[] = "test_matrix.txt";
	maze.build_from_file(filename);
	maze.solve(2, 0);
	maze.print_info();
}
