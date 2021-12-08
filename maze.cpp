#include "maze.h"

// Different visualisation
// #define print_state(room_i) print(room_i);
#define print_state(room_i);

Maze::Maze(RoomIndex start){
  Maze::index = start;
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

int Maze::get_turn() {
    rooms[index].mark_visited();

    if(!get_neighbor(index, &neighbor)){
        if(all_moves.is_nonempty()){
            dir = Directions::opposite(all_moves.pop());
            index = get_neighbor_index(index, dir);
            print_state(index);
            return rooms[index].is_junction() ? dir : get_turn();
        } else {
            return -1;
        }
    } else {
        all_moves.push(neighbor);
        index = get_neighbor_index(index, neighbor);
        print_state(index);
        return rooms[index].is_junction() ? neighbor : get_turn();
    } 
}

int Maze::get_turn_out(){
    if (all_moves.is_nonempty()){
        dir = Directions::opposite(all_moves.pop());
        index = get_neighbor_index(index, dir);
        print_state(index);
        return rooms[index].is_junction() ? dir : get_turn_out();
    }
    return -1;
}
