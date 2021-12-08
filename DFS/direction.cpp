#include "constants.h"
#include "direction.h"


DirStack::DirStack(void) {
    last_int = 0;
    last_dir = 0;
}

void DirStack::push(const Direction dir) {
    const Element elt = dir;

    //set the bits to 0
    elts[last_int] &= ~(MASK << last_dir);

    //set the bits to the direction given
    elts[last_int] |= (elt << last_dir);

    //increment the indices
    last_dir += 2;

    if (last_dir == MAX_LAST_DIR) { //reached the end of an integer
        last_dir = 0; //reset the within-integer counter
        last_int++; //increment the integer counter
    }

}

Direction DirStack::pop(void) {
    //decrement the indices
    if (last_dir == 0) { //reached the beginning of an integer
        last_dir = MAX_LAST_DIR;
        last_int--;
    }
    last_dir -= 2;

    //get the bits
    return (elts[last_int] >> last_dir) & MASK;
}