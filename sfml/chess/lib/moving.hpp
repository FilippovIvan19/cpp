#define MOVING(name) void name::available_squares(board &chessboard, std::set<std::pair<int, int>> &squares)

MOVING(pawn)
{
    
    std::pair<int, int> square;

    int x0 = this->x_;
    int y0 = this->y_;

    int dy = 0;
    Color color = this->color_;
    if (color == WHITE)
        dy = -1;
    else
        dy = 1;

    
    if (!chessboard.squares_[x0][y0 + dy].piece_ptr_)
    {
        square = std::pair<int, int>(x0, y0 + dy);
        squares.insert(square);
    }
    
    for (int i = -1; i <= 1; i += 2)
        if (chessboard.squares_[x0 + i][y0 + dy].piece_ptr_ &&
            chessboard.squares_[x0 + i][y0 + dy].piece_ptr_->color_ != color)
        {
            square = std::pair<int, int>(x0 + i, y0 + dy);
            squares.insert(square);
        }
    
    /////////////////////взятие на перехвате
    //  ферзь

    // return squares;
}


#undef MOVING