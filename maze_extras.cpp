#include "maze.h"

void Maze::build_course_matrix(){
	// NUM_ROWS * NUM_COLS long
	uint8_t maze_description[] = 
		{0,1,1,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,0,0,0,0,1,1,
		1,0,1,0,1,1,0,0,1,0,1,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,1,
		1,0,1,0,0,1,1,0,1,0,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,0,0,1,
		1,0,1,0,1,0,1,0,1,1,0,0,0,1,0,1,1,1,0,1,1,1,0,1,0,0,1,1,
		1,0,1,0,1,0,1,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,1,1,1,0,1,1,
		1,1,0,0,1,1,1,1,0,1,1,1,1,0,0,1,0,1,1,0,1,0,0,1,1,0,1,0,
		0,1,0,0,1,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,1,0,1,1,0,0,1};
	const uint16_t num_rooms = NUM_ROWS * NUM_COLS;
	for (RoomIndex i = 0; i < num_rooms; i++) {
		for(Direction j = 0; j < 4; j++){
			if (maze_description[i * 4 + j]){
				rooms[i].mark_open(j);
			}
		}
  }

	return;
}


// for visualizing the matrix (turn on by redefining print_state in maze.cpp)
void Maze::print(RoomIndex robot_i) {
	print_out('\n');
	//top
	for (uint8_t i = 0; i < NUM_COLS; i++) {
		print_out(rooms[i].is_open(Directions::NORTH) ? ".   " : ".___");
	}
	print_out(".\n");

	for (uint8_t j = 0; j < NUM_ROWS; j++) {
		//middle and bottom

		for (uint8_t i = 0; i < NUM_COLS; i++) {
			RoomIndex room_i = j * NUM_COLS + i;

			print_out(rooms[room_i].is_open(Directions::WEST) ? "  " : "| ");

			if (room_i == robot_i) {
				print_out('o');
			} else if (rooms[room_i].is_visited()) {
				print_out('.');
			} else {
				print_out(' ');
			}
			print_out(' ');

			if (i == NUM_COLS - 1 && !rooms[room_i].is_open(Directions::EAST)) {
				print_out('|');
			}
		}
		print_out('\n');

		for (uint8_t i = 0; i < NUM_COLS; i++) {
			RoomIndex room_i = j * NUM_COLS + i;

			print_out(rooms[room_i].is_open(Directions::WEST) ? '.' : '|');
			print_out(rooms[room_i].is_open(Directions::SOUTH) ? "   " : "___");

			if (i == NUM_COLS - 1 && !rooms[room_i].is_open(Directions::EAST)) {
				print_out('|');
			}
		}
		print_out('\n');
	}
}
