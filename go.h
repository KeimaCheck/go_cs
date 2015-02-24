// Header file for go.cpp

#ifndef _GO_INCLUDED
#define _GO_INCLUDED



#include <iostream>
#include <vector>

#define BLACK 1
#define WHITE -1
#define EMPTY 0

typedef std::vector<int> int_vec;

namespace Go {
    // game state
    extern int_vec board;
    extern int board_size;
    extern int turn;
    extern int passes;

    void init(int);

    inline int index(int, int);
    inline void coords(int, int&, int&);

    // boolean board queries
    int out_of_bounds(int, int);
    int point_occupied(int);
    int same_color(int,int);
    int same_color(int,int, int_vec);
    int suicide(int, int);
    int is_captured(int);
    int is_captured(int, int_vec);
    int is_territory(int);
    int is_territory(int, int_vec);

    int_vec neighbors(int);
}

// inline functions must be defined in the header file

inline int Go::index(int row, int col){
    return row*board_size + col;
}

inline void Go::coords(int point, int& x, int& y){
    y = point % board_size;
    x = (int)point / board_size;
}


#endif // _GO_INCLUDED
