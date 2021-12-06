#ifndef __direction_h__
#define __direction_h__

#include <stdint.h>
#include "constants.h"


typedef uint8_t Direction;

class Directions {
public:
    static const Direction NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3;

    static inline Direction opposite(Direction dir) {
        return dir < 2 ? dir + 2 : dir - 2;
    }

    static inline Direction left(Direction dir) {
        return dir == 0 ? 3 : dir - 1;
    }

    static inline Direction right(Direction dir) {
        return dir == 3 ? 0 : dir + 1;
    }
};

class DirStack {
    typedef uint32_t Element;

    static const uint8_t STACK_SIZE = ((NUM_ROWS * NUM_COLS) / 16) + 1;
    static const Element MASK = 0x3; //3 = 00000011, for masking individual directions
    static const uint8_t MAX_LAST_DIR = (sizeof(Element)) * 8; //number of bits in Element

    Element elts[STACK_SIZE]; //each direction only needs 2 bits, so 16 directions can fit inside one int
    uint8_t last_int, last_dir;

public:
    DirStack(void);

    void push(const Direction elt);

    Direction pop(void);

#ifdef VERBOSE_STACK
    void print(void);
#endif

    inline int is_empty(void) {
        return (last_int == 0) && (last_dir == 0);
    }

    inline int is_nonempty(void) {
        return (last_int > 0) || (last_dir > 0);
    }
};



#endif /* __direction_h__ */
