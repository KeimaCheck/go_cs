

#include "go.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <string>
#include <cctype>
#include <vector>
#include <boost/regex.hpp>

#define GRID_SIZE 9
#define STRETCH 3

#define ALPHABET_LENGTH 25

// status markers
#define WRAP_UP 0
#define WAIT_FOR_INPUT 1
#define VALIDATE_MOVE 2
#define UPDATE 3
#define DRAW 4
#define PASS 5

typedef std::vector<int> int_vec;

char symbol(int);
int alphanum_to_xy(std::string, int&, int&);

void draw_board();

int status;


/* a board position in valid GTP syntax will match the regex play_format,
and the other two are used to convert that representation into one understood
by the program
*/
boost::regex play_format("([a-z])([0-9])");

int main(){

    Go::init(GRID_SIZE);

    int row, col;

    int i, j, x, y;

    std::string play;

    status = WAIT_FOR_INPUT;
    while (status != WRAP_UP){

        if (status == WAIT_FOR_INPUT){
            // TODO: this needs a series of refactors

            // obtain move
            std::cout << "Enter your move in alphanumeric form.\n(I.e., to play the 1-1 point, enter \'a1\')\nTo pass, enter \'pass\'.\n\n";
            std::cin >> play;

            // attempt to convert the move to row/col representation
            if (alphanum_to_xy(play, row, col)){
                // do nothing
            } else {
                std::cout << "Improperly formatted input.\n\n";
                // redundant sanity check
                status = WAIT_FOR_INPUT;
            }
        }
        else if (status == VALIDATE_MOVE) {
            // check legality of move

            if (Go::out_of_bounds(row, col)) {
                std :: cout << "The specified position is off the board.\n\n";
                status = WAIT_FOR_INPUT;
            } else if (Go::point_occupied(Go::index(row,col))){
                std::cout << "The specified point is already occupied.\n\n";
                status = WAIT_FOR_INPUT;
            }

            else {                            // to be deprecated for the branch below
                status = UPDATE;
            }

            /* TODO
             else if (suicide) {
                std::cout << "The specified play commits suicide.\n\n";
                status = WAIT_FOR_INPUT;
            } else {
                status = UPDATE;
            }
            */
        }
        else if (status == UPDATE){
            // update game status after a play
            // update the board array and the turn marker
            Go::board[Go::index(row,col)] = Go::turn;
            Go::turn = -1 * Go::turn;
            Go::passes = 0;
            status = DRAW;
        } else if (status == DRAW){
            draw_board();
            status = WAIT_FOR_INPUT;
        }else if (status == PASS) {
            // update game status after a pass
            Go::turn = Go::turn *-1;
            Go::passes++;
            if (Go::passes == 2) {
                status = WRAP_UP;
            } else {
                // no need to redraw the board
                status = WAIT_FOR_INPUT;
            }
        }
    }
    std::cout << "Game ended by agreement.";
    return 1;
}

/* converts the 1, 0, or -1 used to mark black, empty, and white points on the goban
to x, ., or o as appropriate
*/
char symbol(int marker){
    if (marker == BLACK){
        return 'x';
    } else if (marker == EMPTY){
        return '.';
    } else if (marker == WHITE) {
        return 'o';
    } else {
        std::cerr << "Board data corrupt or invalid---accepts values of 1,0,-1";
        exit(EXIT_FAILURE);
    }
}

/* The standard way to represent a go move is alphanumerically.
This function takes the alphanumeric representation and stores the appropriate
row-column data using the given pointers.
Returns 1 if submitted play is valid and 0 otherwise
*/
int alphanum_to_xy(std::string a_n, int &row, int &col){
    // catch passes
    if (a_n == "pass"){
        status = PASS;
        return 1;
    }

    // match basic format
    boost::smatch m;
    if (!boost::regex_match(a_n, m, play_format)){
        return 0;
    }

    // parse alpha char (the column)

    std::string alpha = std::string(m[1].first, m[1].second);

    const char col_c = alpha.at(0);
    int col_i1 = (int)col_c - (int)'a';
    int col_i2 = (int)col_c - (int)'A';

    // magic
    if (col_i1 >=0 & col_i1 <= ALPHABET_LENGTH){
        col = col_i1;
    } else if (col_i2 >=0 & col_i1 <= ALPHABET_LENGTH) {
        col = col_i2;
    } else{
        return 0;
    }

    // parse numeric chars (the row)

    std::string nummy = m.str(2);
    std::string row_s = nummy;
    int row_i = std::atoi(row_s.c_str());
    row_i--;
    row = row_i;

    status = VALIDATE_MOVE;

    return 1;
}

void draw_board(){
    int x, y;
    for(y=0; y < GRID_SIZE; y++){

        for(x = 0; x < GRID_SIZE; x++){
            std::cout << ' ';
            // conver the integer in the board to an ascii character
            std::cout << symbol(Go::board[Go::index(y,x)]);
        }
        std::cout << "\n";
    }
}
