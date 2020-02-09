#include "lib/big_include.hpp"


void create_all_pieces(sf::RenderWindow *window_ptr, sf::Texture &pieces_texture, 
    board &chessboard, game_manager &manager)
{
    for (int i = 0; i < 8; ++i)
    {
        manager.add_obj(new pawn(window_ptr, BLACK, i, 1, pieces_texture, chessboard));
        manager.add_obj(new pawn(window_ptr, WHITE, i, 6, pieces_texture, chessboard));
    }

    manager.add_obj(new castle(window_ptr, BLACK, 0, 0, pieces_texture, chessboard));
    manager.add_obj(new castle(window_ptr, BLACK, 7, 0, pieces_texture, chessboard));
    manager.add_obj(new castle(window_ptr, WHITE, 0, 7, pieces_texture, chessboard));
    manager.add_obj(new castle(window_ptr, WHITE, 7, 7, pieces_texture, chessboard));

    manager.add_obj(new knight(window_ptr, BLACK, 1, 0, pieces_texture, chessboard));
    manager.add_obj(new knight(window_ptr, BLACK, 6, 0, pieces_texture, chessboard));
    manager.add_obj(new knight(window_ptr, WHITE, 1, 7, pieces_texture, chessboard));
    manager.add_obj(new knight(window_ptr, WHITE, 6, 7, pieces_texture, chessboard));

    manager.add_obj(new bishop(window_ptr, BLACK, 2, 0, pieces_texture, chessboard));
    manager.add_obj(new bishop(window_ptr, BLACK, 5, 0, pieces_texture, chessboard));
    manager.add_obj(new bishop(window_ptr, WHITE, 2, 7, pieces_texture, chessboard));
    manager.add_obj(new bishop(window_ptr, WHITE, 5, 7, pieces_texture, chessboard));

    manager.add_obj(new  queen(window_ptr, BLACK, 3, 0, pieces_texture, chessboard));
    manager.add_obj(new   king(window_ptr, BLACK, 4, 0, pieces_texture, chessboard));
    manager.add_obj(new  queen(window_ptr, WHITE, 3, 7, pieces_texture, chessboard));
    manager.add_obj(new   king(window_ptr, WHITE, 4, 7, pieces_texture, chessboard));
}


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

    create_all_pieces(&window, pieces_texture, chessboard, manager);


    sf::Event event;
    game_cycle(&window, event, manager, chessboard);

    return 0;
}
