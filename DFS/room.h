#ifndef __room_h__
#define __room_h__

#include <stdint.h>

class Room {
    static const uint8_t VISITED_MASK = (1 << 4),   //00010000 Mask for the visited bit
                         OPEN_MASK = 0xF;           //00001111 Mask for the OPEN direction bits

    //| 1 1 1 | 1 | 1 1 1 1 |
    //  UNUSED  V   W S E N
public:
    uint8_t data;
    Room(void) {
        data = 0;
    }

    inline int is_visited(void) {
        return !!(data & VISITED_MASK);
    }
    inline void mark_visited(void) {
        data |= VISITED_MASK;
    }
    inline int is_open(const Direction dir) {
        return !!(data & (1 << dir));
    }
    inline void mark_open(const Direction dir) {
        data |= (1 << dir);
    }
    inline void set_open(const Direction dir, int open) {
        data = open ? data | (1 << dir) : data & ~(1 << dir);
    }
    inline int is_junction(void){
        uint8_t bits = data & OPEN_MASK;
        return (bits != 10) && (bits != 5);
    }
};

#endif /* __room_h__ */
