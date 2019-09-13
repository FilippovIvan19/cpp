
#define MAKE_CLASS(name, const_name)                                                                                \
                                                                                                                    \
class name: public piece                                                                                           \
{                                                                                                                   \
public:                                                                                                             \
    name(sf::RenderWindow *window, Color color, int x, int y, sf::Texture &texture, board &chessboard);            \
   ~name() {};                                                                                                     \
    virtual std::set<std::pair<int, int>> &available_squares(board &chessboard, std::set<std::pair<int, int>> &squares);                      \
};                                                                                                                  \
                                                                                                                    \
name::name(sf::RenderWindow *window, Color color, int x, int y, sf::Texture &texture, board &chessboard):         \
    piece(window, color, x, y, texture, chessboard, const_name)                                                    \
    {}                                                                                                              


MAKE_CLASS(pawn, PAWN)
MAKE_CLASS(castle, CASTLE)
MAKE_CLASS(knight, KNIGHT)
MAKE_CLASS(bishop, BISHOP)
MAKE_CLASS(queen, QUEEN)
MAKE_CLASS(king, KING)


#undef MAKE_CLASS