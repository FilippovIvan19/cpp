#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 800;
const int SQUARE_SIZE  = 100;
const int PIC_H = 163;
const int PIC_W = 83;
const int NORMAL_SQUARE = 0;
const int GREEN_SQUARE = 1;
const int RED_SQUARE = 2;
const int PURPLE_SQUARE = 3;



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



#include "lib/common_elements.hpp"
#include "lib/board.hpp"
#include "lib/pieces.hpp"
#include "lib/game_manager.hpp"
#include "lib/game_logic.hpp"



int main(int argc, char const *argv[])
{    
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "CHESS");

    sf::Texture board_texture;
    board_texture.loadFromFile("textures/board.png");
    board_texture.setSmooth(true);
    board chessboard = board(&window, board_texture);

    sf::Texture pieces_texture;
    pieces_texture.loadFromFile("textures/pieces.png");
    pieces_texture.setSmooth(true);
    

    game_manager manager;
    piece *cur_piece;

    for (int i = 0; i < 8; ++i)
    {
        cur_piece = new pawn(&window, BLACK, i, 1, pieces_texture);
        manager.add_obj(cur_piece);

        cur_piece = new pawn(&window, WHITE, i, 6, pieces_texture);
        manager.add_obj(cur_piece);
    }

    sf::Event event;
    game_cycle(window, event, manager, chessboard);

    manager.clear();
    return 0;
}