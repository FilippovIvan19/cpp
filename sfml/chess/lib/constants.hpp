const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 800;
const int SQUARE_SIZE  = 100;
const int EXIT = -666;
const int WHITE_WIN = 1;
const int BLACK_WIN = 2;
const int STALEMATE = 3;

const int PIC_H = 200;
const int PIC_W = 200;

const int NORMAL_SQUARE = 0;
const int GREEN_SQUARE = 1;
const int RED_SQUARE = 2;
const int PURPLE_SQUARE = 3;

enum Color
{
    WHITE = 0,
    BLACK
};

enum Kind
{
    PAWN = 0,
    CASTLE,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};
