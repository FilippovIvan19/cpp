#include "lib/big_include.hpp"


void create_all_pieces(sf::RenderWindow &window, sf::Texture &pieces_texture, 
    board &chessboard, game_manager &manager)
{
    for (int i = 0; i < 8; ++i)
    {
        manager.add_obj(new pawn(&window, BLACK, i, 1, pieces_texture, chessboard));
        manager.add_obj(new pawn(&window, WHITE, i, 6, pieces_texture, chessboard));
    }

    manager.add_obj(new castle(&window, BLACK, 0, 0, pieces_texture, chessboard));
    manager.add_obj(new castle(&window, BLACK, 7, 0, pieces_texture, chessboard));
    manager.add_obj(new castle(&window, WHITE, 0, 7, pieces_texture, chessboard));
    manager.add_obj(new castle(&window, WHITE, 7, 7, pieces_texture, chessboard));

    manager.add_obj(new knight(&window, BLACK, 1, 0, pieces_texture, chessboard));
    manager.add_obj(new knight(&window, BLACK, 6, 0, pieces_texture, chessboard));
    manager.add_obj(new knight(&window, WHITE, 1, 7, pieces_texture, chessboard));
    manager.add_obj(new knight(&window, WHITE, 6, 7, pieces_texture, chessboard));

    manager.add_obj(new bishop(&window, BLACK, 2, 0, pieces_texture, chessboard));
    manager.add_obj(new bishop(&window, BLACK, 5, 0, pieces_texture, chessboard));
    manager.add_obj(new bishop(&window, WHITE, 2, 7, pieces_texture, chessboard));
    manager.add_obj(new bishop(&window, WHITE, 5, 7, pieces_texture, chessboard));

    manager.add_obj(new  queen(&window, BLACK, 3, 0, pieces_texture, chessboard));
    manager.add_obj(new   king(&window, BLACK, 4, 0, pieces_texture, chessboard));
    manager.add_obj(new  queen(&window, WHITE, 3, 7, pieces_texture, chessboard));
    manager.add_obj(new   king(&window, WHITE, 4, 7, pieces_texture, chessboard));
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

    create_all_pieces(window, pieces_texture, chessboard, manager);


    sf::Event event;
    game_cycle(window, event, manager, chessboard);

    manager.clear();
    return 0;
}
