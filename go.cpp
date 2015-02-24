/*

This is an implementation for the behavior necessary for a program
to make the most basic decisions necessary to play Go.

General rules:
Board positions should be represented internally as array indices whenever possible
Coordinates should only be used when it is most convenient, and only passed as
    function parameters for checking if a point is on the board

*/

#include "go.h"

int Go::board_size;
int_vec Go::board;
int Go::turn;
int Go::passes;

// Must be called by the client before use
void Go::init(int gridsize){
    board_size = gridsize;
    board = int_vec(gridsize*gridsize, 0);
    turn = WHITE;
    passes = 0;
}


/* Returns 1 if the point specified by (row, col) is off the board
and a 0 otherwise
*/
int Go::out_of_bounds(int row, int col){
    return (row > board_size-1 || col > board_size-1 || row < 0 || col < 0);
}
/* Returns 1 if the point specified is occupied and 0 otherwise
*/
int Go::point_occupied(int point){
    int a = board[point];
    return (a*a);
}
/* Returns 1 if the point specified is occupied and 0 otherwise
*/
int Go::same_color(int point1, int point2){
    // TODO: error-check the inputs
    return (board[point1] == board[point2]);
}
int Go::same_color(int point1, int point2, int_vec board_t){
    // TODO: error-check the inputs
    return (board_t[point1] == board_t[point2]);
}
int Go::suicide(int point, int color){
    // TODO: error-check the inputs

    // clone the board
    int_vec bcopy = board;
    bcopy[point] = color;

    // see if the group containing point is captured
    if (is_captured(point, bcopy)) {
        return 1;
    }

    int_vec neighborhood = neighbors(point);

    int i;

    for (i=0; i< neighborhood.size(); i++){
        if (!same_color(point, neighborhood[i], bcopy)) {
            return (!is_captured(neighborhood[i], bcopy));
        }

    }

}

int Go::is_captured(int point){}

int Go::is_captured(int point, int_vec board_t){

}

int Go::is_territory(int point){}

int Go::is_territory(int point, int_vec board_t){

}

/*
Provides the functionality needed for graph-like search
operations on a go board without actually building a graph

int pos - the array index of the point in question
int board_size - given an nxn board, this is n

return - a vector of array indices corresponding to
    the point's neighboring points

*/

int_vec Go::neighbors(int point){
    // it's awful, but here I'm using x to represent the row
    int x,y;
    coords(point,x,y);

    // first check that the position is legal (sanity)
    if (point >= board_size * board_size) {
        std::cout << "Size mismatch in neigbhors().";
        // exit with failure
    }

    int_vec outs;

    int x_t, y_t;

    // Top neighbor
    x_t = x - 1;
    y_t = y;
    if (out_of_bounds(x_t,y_t) ){
        outs.push_back(index(x_t,y_t));
    }

    // Bottom neighbor
    x_t = x + 1;
    y_t = y;
    if (out_of_bounds(x_t,y_t)){
        outs.push_back(index(x_t,y_t));
    }

    // Left neighbor
    x_t = x;
    y_t = y - 1;
    if (out_of_bounds(x_t,y_t)){
        outs.push_back(index(x_t,y_t));
    }

    // Right neighbor
    x_t = x;
    y_t = y + 1;
    if (out_of_bounds(x_t,y_t)){
        outs.push_back(index(x_t,y_t));
    }

    return outs;
}

