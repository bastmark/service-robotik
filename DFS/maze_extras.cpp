#include "maze.h"
//#include "robot.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

void Maze::build_from_file(const char *filename){
	std::ifstream input(filename, ::std::ios::binary);
	char open;

	const uint16_t num_rooms = NUM_ROWS * NUM_COLS;
    for (RoomIndex i = 0; i < num_rooms; i++) {
		for(Direction j = 0; j < 4; j++){
			input.get(open);
			if (open == '1'){
				rooms[i].mark_open(j);
			}
		}
		// Scan new line
		input.get(open);
    }

	return;
}

void Maze::print(RoomIndex robot_i) {
	static const char arrows[] = {'^', '>', 'v', '<'};
	print_out('\n');
	//top
	for (uint8_t i = 0; i < NUM_COLS; i++) {
		print_out(rooms[get_index(0, i)].is_open(Directions::NORTH) ? ".   " : ".___");
	}
	print_out(".\n");

	for (uint8_t j = 0; j < NUM_ROWS; j++) {
		//middle and bottom

		for (uint8_t i = 0; i < NUM_COLS; i++) {
			RoomIndex room_i = get_index(j, i);

			print_out(rooms[room_i].is_open(Directions::WEST) ? "  " : "| ");

			if (is_goal_room(room_i)) {
				print_out('x');
			} else if (room_i == robot_i) {
				print_out('o');
			} else if (rooms[room_i].is_solution()) {
				print_out(arrows[rooms[room_i].get_next()]);
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
			RoomIndex room_i = get_index(j, i);

			print_out(rooms[room_i].is_open(Directions::WEST) ? '.' : '|');
			print_out(rooms[room_i].is_open(Directions::SOUTH) ? "   " : "___");

			if (i == NUM_COLS - 1 && !rooms[room_i].is_open(Directions::EAST)) {
				print_out('|');
			}
		}
		print_out('\n');
	}
}

void Maze::print_info() {
	print_out("maze size: ");
	print_out(sizeof(Maze) / 2);
	print_out("\nstack size: ");
	print_out(sizeof(uint32_t) * (((NUM_ROWS * NUM_COLS) / 16) + 1));

	print_out("\nroom size: ");
	print_out(sizeof(Room));

	print_out('\n');
}
