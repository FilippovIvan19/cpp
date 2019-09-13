#define MOVING(name) std::set<std::pair<int, int>> &name::available_squares(board &chessboard, std::set<std::pair<int, int>> &squares)

#define START \
    squares.clear();                \
    std::pair<int, int> square;     \
    int x0 = this->x_;              \
    int y0 = this->y_;              \
    Color color = this->color_;

#define IS_AVAILABLE(x, y) \
    0 <= (x) && (x) <= 7 &&                                     \
    0 <= (y) && (y) <= 7 &&                                     \
    chessboard.squares_[x][y].piece_ptr_ &&                     \
    chessboard.squares_[x][y].piece_ptr_->color_ != color &&    \
    chessboard.squares_[x][y].piece_ptr_->kind_ != KING

#define IS_EMPTY(x, y) \
    0 <= (x) && (x) <= 7 &&                                     \
    0 <= (y) && (y) <= 7 &&                                     \
    !chessboard.squares_[x][y].piece_ptr_

#define INSERT(x, y) squares.insert(std::pair<int, int>(x, y))


MOVING(pawn)
{
    START

    int dy = 0;
    if (color == WHITE)
        dy = -1;
    else
        dy = 1;

    if((y0 - dy) % 7 == 0 && IS_EMPTY(x0, y0 + 2 * dy))
        INSERT(x0, y0 + 2 * dy);

    if (IS_EMPTY(x0, y0 + dy))
        INSERT(x0, y0 + dy);

    for (int i = -1; i <= 1; i += 2)
        if (IS_AVAILABLE(x0 + i, y0 + dy))
            INSERT(x0 + i, y0 + dy);
    
    /////////////////////взятие на перехвате
    // ферзь

    return squares;
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

}

MOVING(king)
{
    START

    for (int d = -1; d <= 1; d += 2)
    {
        if (IS_AVAILABLE(x0, y0 + d) || IS_EMPTY(x0, y0 + d))
            INSERT(x0, y0 + d);

        if (IS_AVAILABLE(x0 + d, y0) || IS_EMPTY(x0 + d, y0))
            INSERT(x0 + d, y0);
    }

    for (int dx = -1; dx <= 1; dx += 2)
    for (int dy = -1; dy <= 1; dy += 2)
        if (IS_EMPTY(x0 + dx, y0 + dy) || IS_AVAILABLE(x0 + dx, y0 + dy))
            INSERT(x0 + dx, y0 + dy);

    //рокировка
    //шах
}


#undef MOVING
#undef START
#undef IS_EMPTY
#undef IS_AVAILABLE
#undef INSERT