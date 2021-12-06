#ifndef __room_h__
#define __room_h__

#include <stdint.h>

class Room {
    //solution bit: is this room on the solution path

    static const uint8_t V = (1 << 4),          //00010000 Mask for the visited bit
                         S = (1 << 5),          //00100000 Mask for the solution bit
                         NEXT_SHIFT = 6,
                         NEXT_MASK = 0xC0;      //11000000 Mask for the NEXT direction bits

    //| 1 1  | 1 | 1 | 1 1 1 1 |
    //  NEXT   S   V   W S E N
    uint8_t data;

    // Contains the distance from start room
    uint8_t distance;
public:
    Room(void) {
        data = 0;
        distance = 0;
    }

    inline int is_visited(void) {
        return !!(data & V);
    }
    inline void mark_visited(void) {
        data |= V;
    }
    inline void mark_unvisited(void) {
        data &= ~V;
    }
    inline int is_solution(void) {
        return !!(data & S);
    }
    inline void mark_solution(void) {
        data |= S;
    }
    inline void mark_not_solution(void) {
        data &= ~S;
    }
    inline int is_open(const Direction dir) {
        return !!(data & (1 << dir));
    }
    inline void mark_open(const Direction dir) {
        data |= (1 << dir);
    }
    inline void mark_closed(const Direction dir) {
        data &= ~(1 << dir);
    }
    inline void set_open(const Direction dir, int open) {
        data = open ? data | (1 << dir) : data & ~(1 << dir);
    }
    inline Direction get_next(void) {
        return (data & NEXT_MASK) >> NEXT_SHIFT;
    }
    inline void set_next(const Direction next) {
        data = (data & ~NEXT_MASK) | (next << NEXT_SHIFT);
    }
};

#endif /* __room_h__ */
