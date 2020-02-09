#define MAKE_CLASS(name, const_name) \
                                                                                                                   \
class name: public piece                                                                                           \
{                                                                                                                  \
public:                                                                                                            \
    name(sf::RenderWindow *window_ptr, Color color, int x, int y, sf::Texture &texture, board &chessboard);            \
   ~name() {};                                                                                                     \
    virtual void update_available_squares(game_manager &manager, board &chessboard);                               \
};                                                                                                                 \
                                                                                                                   \
name::name(sf::RenderWindow *window_ptr, Color color, int x, int y, sf::Texture &texture, board &chessboard):          \
    piece(window_ptr, color, x, y, texture, chessboard, const_name)                                                    \
    {}                                                                                                              


MAKE_CLASS(pawn, PAWN)
MAKE_CLASS(castle, CASTLE)
MAKE_CLASS(knight, KNIGHT)
MAKE_CLASS(bishop, BISHOP)
MAKE_CLASS(queen, QUEEN)


class king: public piece
{
public:
    king(sf::RenderWindow *window_ptr, Color color, int x, int y, sf::Texture &texture, board &chessboard);
   ~king() {};
    virtual void update_available_squares(game_manager &manager, board &chessboard);
    bool checked(board &chessboard);
};

king::king(sf::RenderWindow *window_ptr, Color color, int x, int y, sf::Texture &texture, board &chessboard):
    piece(window_ptr, color, x, y, texture, chessboard, KING)
    {}  


#undef MAKE_CLASS