#include "maze.h"


// Different visualisation
#define print_state(room_i) print(room_i); usleep(1000000)
// #define print_state(room_i) print(room_i)
// #define print_state(room_i)

// Goal maze indicies for testing
const uint8_t Maze::TEST_GOALS[] = {0, 6, 11};
static DirStack all_moves;

Maze::RoomIndex Maze::move(const RoomIndex room_i, const Direction dir) {
    static Direction last_dir;

    if (dir != last_dir){
        print_out("turn");
    }

    last_dir = dir;
    
    return get_neighbor_index(room_i, dir);
}

int Maze::get_neighbor(const RoomIndex room_i, Direction *neighbor) {
    RoomIndex next_room_i;
    for (Direction dir = 0; dir < 4; dir++) {
        next_room_i = get_neighbor_index(room_i, dir);

        if (rooms[room_i].is_open(dir)) {
            if (is_in_range(next_room_i)
                    && !rooms[next_room_i].is_visited()) {
                
                *neighbor = dir;
                return 1;
            }
        }
    }
    return 0;
}

// void Maze::correct_shortest(RoomIndex room_i, DirStack &moves){
//     DirStack corrected;

//     while (moves.is_nonempty()){
//         moves.pop();
//     }
// }

int Maze::dfs_get_turn() {
    Direction dir, neighbor;

    rooms[START].mark_visited();


    
}

int Maze::dfs_get_turn_out(){

}

int Maze::dfs(RoomIndex room_i) {
    DirStack moves;

    Direction dir, neighbor;

    while (1) {
        rooms[room_i].mark_visited();
        // correct_shortest(room_i, moves);
        
        // Goal achived: backtrack out
        if (goal_achived()) {
            while (moves.is_nonempty()) {
                dir = moves.pop();
                room_i = move(room_i, Directions::opposite(dir));
                rooms[room_i].set_next(dir);
                rooms[room_i].mark_solution();

                print_state(room_i);
            }
            return 1;
        }
        

        //backtrack until there are unvisited neighbors
        while (!get_neighbor(room_i, &neighbor)) {
            if (moves.is_nonempty()) {
                room_i = move(room_i, Directions::opposite(moves.pop()));
                print_state(room_i);
            } else {
                //can't backtrack anymore, so no solution was found
                return 0;
            }
        }

        //found an unvisited neighbor
        moves.push(neighbor); //store the move on the move stack
        room_i = move(room_i, neighbor);
        print_state(room_i);
    }
}

void Maze::clear_visited(void) {
    const uint16_t num_rooms = NUM_ROWS * NUM_COLS;
    for (uint16_t i = 0; i < num_rooms; i++) {
        rooms[i].mark_unvisited();
    }
}

// For testing is goal achived 
bool Maze::goal_achived(){
    for (uint16_t i = 0; i < 3; i++){
        if (!rooms[TEST_GOALS[i]].is_visited()){
            return false;
        }
    }
    return true;
}

// For printing
bool Maze::is_goal_room(RoomIndex room_i){
    for (uint16_t i = 0; i < 3; i++){
        if (TEST_GOALS[i] == room_i){
            return true;
        }
    }
    return false;
}

