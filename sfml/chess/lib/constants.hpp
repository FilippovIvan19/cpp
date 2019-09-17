const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 800;
const int SQUARE_SIZE  = 100;
const int PIC_H = 163;
const int PIC_W = 83;
const int NORMAL_SQUARE = 0;
const int GREEN_SQUARE = 1;
const int RED_SQUARE = 2;
const int PURPLE_SQUARE = 3;
const int EXIT = -666;

enum Color
{
    BLACK = 0,
    WHITE
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
