#ifndef __maze_h__
#define __maze_h__

#include <stdint.h>
#include "constants.h"
#include "direction.h"
#include "room.h"

class Maze {

    typedef uint8_t RoomIndex; // Max amount of cells = 256

    static const uint8_t TEST_GOALS[3]; //= {0, 6, 42};

    Room rooms[NUM_ROWS * NUM_COLS];

    static inline RoomIndex get_index(const uint8_t row, const uint8_t col) {
        return row * NUM_COLS + col;
    }

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

    RoomIndex move(const RoomIndex room_i, const Direction dir);

    int get_neighbor(const RoomIndex room_i, Direction *neighbor);

    int dfs(RoomIndex room_i);

    void clear_visited(void);

    int get_rand_neighbor(RoomIndex *room_i);
    void print(RoomIndex robot_i);
    bool goal_achived();
    bool is_goal_room(RoomIndex room_i);
    // void correct_shortest(RoomIndex room_i, DirStack &moves);

public:
    inline int solve(const uint8_t start_row, const uint8_t start_col) {
        return dfs(get_index(start_row, start_col));
    }

    void build_from_file(const char *filename);

    void print_info();
};

#endif /* __maze_h__ */
