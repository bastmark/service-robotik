#include "constants.h"
#include "direction.h"



DirStack::DirStack(void) {
    last_int = 0;
    last_dir = 0;
}

#ifdef VERBOSE_STACK
static const char *dirs[] = {" N ", " E ", " S ", " W "};

#define print_push \
    print_out("\nstack PUSH:"); \
    print_out(dirs[elt]); \
    print();

#define print_pop \
    print_out("\nstack POP:"); \
    print_out(dirs[(elts[last_int] >> last_dir) & MASK]); \
    print();

#else /* VERBOSE_STACK */
//neutralize the printouts
#define print_push
#define print_pop

#endif /* VERBOSE_STACK */

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

    print_push
}

Direction DirStack::pop(void) {
    //decrement the indices
    if (last_dir == 0) { //reached the beginning of an integer
        last_dir = MAX_LAST_DIR;
        last_int--;
    }
    last_dir -= 2;

    print_pop

    //get the bits
    return (elts[last_int] >> last_dir) & MASK;
}

#ifdef VERBOSE_STACK
void DirStack::print(void) {
    uint8_t elt_i, dir_i;

    Direction dir;

    print_out("\nMax last dir:");
    print_out((int) MAX_LAST_DIR);

    print_out("\nLast int:");
    print_out((int) last_int);

    print_out("\nLast dir:");
    print_out((int) last_dir);

    print_out("\nStack size:");
    print_out(STACK_SIZE);

    print_out('\n');

    for (elt_i = 0; elt_i < last_int; elt_i++) {
        for (dir_i = 0; dir_i < MAX_LAST_DIR; dir_i += 2) {
            dir = (elts[elt_i] >> dir_i) & MASK;
            print_out(dirs[dir]);
        }
    }
    for (dir_i = 0; dir_i < last_dir; dir_i += 2) {
        dir = (elts[elt_i] >> dir_i) & MASK;
        print_out(dirs[dir]);
    }
    print_out('\n');

}
#endif /* VERBOSE_STACK */
