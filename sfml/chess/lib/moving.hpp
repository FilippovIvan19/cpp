#define MOVING(name) void name::update_available_squares(\
    game_manager &manager, board &chessboard)

#define START \
    this->available_squares_.clear();   \
    int x0 = this->x_;                  \
    int y0 = this->y_;                  \
    Color color = this->color_;

#define IS_AVAILABLE(x, y) \
    0 <= (x) && (x) <= 7 &&                                     \
    0 <= (y) && (y) <= 7 &&                                     \
    chessboard.squares_[x][y].piece_ptr_ &&                     \
    chessboard.squares_[x][y].piece_ptr_->color_ != color &&    \
    chessboard.squares_[x][y].piece_ptr_->kind_ != KING

#define IS_HERE(name, x, y) \
    0 <= (x) && (x) <= 7 &&                                     \
    0 <= (y) && (y) <= 7 &&                                     \
    chessboard.squares_[x][y].piece_ptr_ &&                     \
    chessboard.squares_[x][y].piece_ptr_->color_ != color &&    \
    chessboard.squares_[x][y].piece_ptr_->kind_ == name

#define IS_EMPTY(x, y) \
    0 <= (x) && (x) <= 7 &&                                     \
    0 <= (y) && (y) <= 7 &&                                     \
    !chessboard.squares_[x][y].piece_ptr_

#define INSERT(x, y) this->available_squares_.insert(std::pair<int, int>(x, y))




void relocate(piece *first_piece, int x, int y, game_manager &manager, board &chessboard)
{
    piece *second_piece = chessboard.squares_[x][y].piece_ptr_;
    if (second_piece)
    {
        delete(second_piece);
        manager.game_objects_.erase(second_piece);
    }

    first_piece->relocate(x, y, chessboard);
}



bool make_shah(game_manager &manager, 
    board &chessboard, std::pair<int, int> square, 
    int x0, int y0)
{
    piece *first_piece = chessboard.squares_[x0][y0].piece_ptr_;
    piece *second_piece = chessboard.squares_[square.first][square.second].piece_ptr_;

    first_piece->relocate(square.first, square.second, chessboard);

    bool shah = first_piece->color_ == WHITE && manager.white_king_->checked(chessboard) ||
        first_piece->color_ == BLACK && manager.black_king_->checked(chessboard);

    first_piece->relocate(x0, y0, chessboard);
    chessboard.squares_[square.first][square.second].piece_ptr_ = second_piece;

    return shah;
}


void check_squares(game_manager &manager, 
    board &chessboard, std::set<std::pair<int, int>> &squares, 
    int x0, int y0)
{
    std::set<std::pair<int, int>> for_delete = std::set<std::pair<int, int>>();

    for (std::pair<int, int> cur_sqr: squares)
        if (make_shah(manager, chessboard, cur_sqr, x0, y0))
            for_delete.insert(cur_sqr);

    for (std::pair<int, int> cur_sqr: for_delete)
        squares.erase(cur_sqr);
}


MOVING(pawn)
{
    START

    int dy = (color == WHITE)? -1: 1;

    if((y0 - dy) % 7 == 0 && IS_EMPTY(x0, y0 + 2 * dy))
        INSERT(x0, y0 + 2 * dy);

    if (IS_EMPTY(x0, y0 + dy))
        INSERT(x0, y0 + dy);

    for (int i = -1; i <= 1; i += 2)
        if (IS_AVAILABLE(x0 + i, y0 + dy))
            INSERT(x0 + i, y0 + dy);
    
    /////////////////////взятие на перехвате
    // ферзь
    
    check_squares(manager, chessboard, this->available_squares_, x0, y0);
}

MOVING(castle)
{
    START

    for (int d = -1; d <= 1; d += 2)
    {
        int x = x0 + d;
        int y = y0 + d;

        while (IS_EMPTY(x0, y))
        {
            INSERT(x0, y);
            y += d;
        }
        while (IS_EMPTY(x, y0))
        {
            INSERT(x, y0);
            x += d;
        }

        if (IS_AVAILABLE(x0, y))
            INSERT(x0, y);

        if (IS_AVAILABLE(x, y0))
            INSERT(x, y0);
    }
    
    check_squares(manager, chessboard, this->available_squares_, x0, y0);

}

MOVING(knight)
{
    START

    for (int i = -1; i <= 1; i += 2)
    for (int j = -2; j <= 2; j += 4)
    {
        if (IS_EMPTY(x0 + i, y0 + j) || IS_AVAILABLE(x0 + i, y0 + j))
            INSERT(x0 + i, y0 + j);

        if (IS_EMPTY(x0 + j, y0 + i) || IS_AVAILABLE(x0 + j, y0 + i))
            INSERT(x0 + j, y0 + i);
    }
    
    check_squares(manager, chessboard, this->available_squares_, x0, y0);
}

MOVING(bishop)
{
    START

    for (int dx = -1; dx <= 1; dx += 2)
    for (int dy = -1; dy <= 1; dy += 2)
    {
        int x = x0 + dx;
        int y = y0 + dy;

        while (IS_EMPTY(x, y))
        {
            INSERT(x, y);
            x += dx;
            y += dy;
        }

        if (IS_AVAILABLE(x, y))
            INSERT(x, y);
    }
    
    check_squares(manager, chessboard, this->available_squares_, x0, y0);
}

MOVING(queen)
{
    START

    for (int d = -1; d <= 1; d += 2)
    {
        int x = x0 + d;
        int y = y0 + d;

        while (IS_EMPTY(x0, y))
        {
            INSERT(x0, y);
            y += d;
        }
        while (IS_EMPTY(x, y0))
        {
            INSERT(x, y0);
            x += d;
        }

        if (IS_AVAILABLE(x0, y))
            INSERT(x0, y);

        if (IS_AVAILABLE(x, y0))
            INSERT(x, y0);
    }



    for (int dx = -1; dx <= 1; dx += 2)
    for (int dy = -1; dy <= 1; dy += 2)
    {
        int x = x0 + dx;
        int y = y0 + dy;

        while (IS_EMPTY(x, y))
        {
            INSERT(x, y);
            x += dx;
            y += dy;
        }

        if (IS_AVAILABLE(x, y))
            INSERT(x, y);
    }
    
    check_squares(manager, chessboard, this->available_squares_, x0, y0);
}

MOVING(king)
{
    START

    for (int dx = -1; dx <= 1; dx += 1)
    for (int dy = -1; dy <= 1; dy += 1)
        if(dx || dy)
            if (IS_EMPTY(x0 + dx, y0 + dy) || IS_AVAILABLE(x0 + dx, y0 + dy))
                INSERT(x0 + dx, y0 + dy);

    //рокировка

    check_squares(manager, chessboard, this->available_squares_, x0, y0);
}


bool king::checked(board &chessboard)
{
    int x0 = this->x_;
    int y0 = this->y_;
    Color color = this->color_;


    for (int dx = -1; dx <= 1; dx += 2)
    for (int dy = -1; dy <= 1; dy += 2)
    {
        int x = x0 + dx;
        int y = y0 + dy;

        while (IS_EMPTY(x, y))
        {
            x += dx;
            y += dy;
        }

        if (IS_HERE(BISHOP, x, y) || IS_HERE(QUEEN, x, y))
            return true;
    }

    for (int d = -1; d <= 1; d += 2)
    {
        int x = x0 + d;
        int y = y0 + d;

        while (IS_EMPTY(x0, y))
            y += d;
        
        while (IS_EMPTY(x, y0))
            x += d;

        if (IS_HERE(QUEEN, x0, y) || IS_HERE(CASTLE, x0, y))
            return true;

        if (IS_HERE(QUEEN, x, y0) || IS_HERE(CASTLE, x, y0))
            return true;
    }

    for (int i = -1; i <= 1; i += 2)
    for (int j = -2; j <= 2; j += 4)
        if (IS_HERE(KNIGHT, x0 + i, y0 + j) || IS_HERE(KNIGHT, x0 + j, y0 + i))
            return true;

    int pawn_dy = (color == WHITE)? -1: 1;

    for (int i = -1; i <= 1; i += 2)
        if (IS_HERE(PAWN, x0 + i, y0 + pawn_dy))
            return true;

    for (int dx = -1; dx <= 1; dx += 1)
    for (int dy = -1; dy <= 1; dy += 1)
        if (dx || dy)
            if (IS_HERE(KING, x0 + dx, y0 + dy))
                return true;

    return false;
}


#undef MOVING
#undef START
#undef IS_EMPTY
#undef IS_AVAILABLE
#undef INSERT
#undef IS_HERE