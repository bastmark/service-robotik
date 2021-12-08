#ifndef __maze_h__
#define __maze_h__

#include <stdint.h>
#include "constants.h"
#include "direction.h"
#include "room.h"

class Maze {

    typedef uint8_t RoomIndex; // Max amount of cells = 256
    DirStack all_moves;
    RoomIndex index;
    Direction dir, neighbor;

    Room rooms[NUM_ROWS * NUM_COLS];

    static inline int is_in_range(RoomIndex room_i){
        return (room_i / NUM_COLS >= 0 && room_i / NUM_COLS < NUM_ROWS) && (room_i % NUM_COLS >= 0 && room_i % NUM_COLS < NUM_COLS);
    }

    static inline RoomIndex get_neighbor_index(const RoomIndex room_i, const Direction dir){
        switch(dir){
            case Directions::NORTH:
                return room_i - NUM_COLS;
            case Directions::EAST:
                return room_i + 1;
            case Directions::SOUTH:
                return room_i + NUM_COLS;
            default: // West
                return room_i - 1;
        }
    }

    int get_neighbor(const RoomIndex room_i, Direction *neighbor);

    int get_rand_neighbor(RoomIndex *room_i);

    void print(RoomIndex robot_i);

public:

    // Takes 4 turns in the maze and backtracks out
    inline int test() {
        build_course_matrix();
        index = 45;
        int returned = 0;

        for(int i = 0; i < 4; i++){
            returned = get_turn();
            print_out("walking : ");
            print_out(returned);
            print_out('\n');
        }

        while (1){
            int returned = get_turn_out();
            print_out("exiting : ");
            print_out(returned);
            print_out('\n');

            if (returned == -1) break;
        }

        return 1;
    }

    int get_turn();

    int get_turn_out();

    void build_course_matrix();
};

#endif /* __maze_h__ */
