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


std::pair<int, int> wait_chosen_square(sf::RenderWindow &window, sf::Event &event)
{
    while (window.waitEvent(event))
    {
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
            return std::pair<int, int> (EXIT, EXIT);
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            int mouse_x = event.mouseButton.x / SQUARE_SIZE;
            int mouse_y = event.mouseButton.y / SQUARE_SIZE;

            return std::pair<int, int> (mouse_x, mouse_y);
        }
    }
}


void fill_manager_for_choise(sf::RenderWindow *window_ptr, sf::Texture &pieces_texture, 
    game_manager &manager_for_choise, board &empty_chessboard)
{
    manager_for_choise.add_obj(new castle(window_ptr, WHITE, 0, 0, pieces_texture, empty_chessboard));
    manager_for_choise.add_obj(new knight(window_ptr, WHITE, 1, 0, pieces_texture, empty_chessboard));
    manager_for_choise.add_obj(new bishop(window_ptr, WHITE, 2, 0, pieces_texture, empty_chessboard));
    manager_for_choise.add_obj(new  queen(window_ptr, WHITE, 3, 0, pieces_texture, empty_chessboard));

    manager_for_choise.add_obj(new castle(window_ptr, BLACK, 0, 7, pieces_texture, empty_chessboard));
    manager_for_choise.add_obj(new knight(window_ptr, BLACK, 1, 7, pieces_texture, empty_chessboard));
    manager_for_choise.add_obj(new bishop(window_ptr, BLACK, 2, 7, pieces_texture, empty_chessboard));
    manager_for_choise.add_obj(new  queen(window_ptr, BLACK, 3, 7, pieces_texture, empty_chessboard));
}


int pawn_reborn(piece *cur_pawn, game_manager &manager, board &chessboard, sf::Event &event)
{
    Color color = cur_pawn->color_;

    sf::RenderWindow *window_ptr = cur_pawn->window_ptr_;

    static board empty_chessboard(window_ptr, (sf::Texture&)*chessboard.squares_[0][0].sprite_.getTexture());
    static game_manager manager_for_choise;
    if (manager_for_choise.game_objects_.empty())
        fill_manager_for_choise(window_ptr, (sf::Texture&)*cur_pawn->sprite_.getTexture(),
            manager_for_choise, empty_chessboard);    

    window_ptr->clear();
        chessboard.draw();
        for (piece *obj: manager_for_choise.game_objects_)
            if (obj->color_ == color)
                obj->draw();
    window_ptr->display();

    std::pair<int, int> chosen_square;
    piece *chosen_piece = nullptr;

    do
    {
        chosen_square = wait_chosen_square(*window_ptr, event);
        if (chosen_square == std::pair<int, int>(EXIT, EXIT))
            return EXIT;

        chosen_piece = empty_chessboard.squares_[chosen_square.first][chosen_square.second].piece_ptr_;
    }
    while (!chosen_piece || chosen_piece->color_ != color);

    piece *new_piece = nullptr;


    switch (chosen_square.first)
    {
        case 0:
            new_piece = new castle(window_ptr, color, (int)cur_pawn->x_, (int)cur_pawn->y_, (sf::Texture&)*cur_pawn->sprite_.getTexture(), chessboard);
            break;
        case 1:
            new_piece = new knight(window_ptr, color, (int)cur_pawn->x_, (int)cur_pawn->y_, (sf::Texture&)*cur_pawn->sprite_.getTexture(), chessboard);
            break;
        case 2:
            new_piece = new bishop(window_ptr, color, (int)cur_pawn->x_, (int)cur_pawn->y_, (sf::Texture&)*cur_pawn->sprite_.getTexture(), chessboard);
            break;
        case 3:
            new_piece = new  queen(window_ptr, color, (int)cur_pawn->x_, (int)cur_pawn->y_, (sf::Texture&)*cur_pawn->sprite_.getTexture(), chessboard);
            break;
        default:
            assert(0);
    }

    manager.add_obj(new_piece);


    manager.game_objects_.erase(cur_pawn);
    delete(cur_pawn);
}


void relocate(piece *first_piece, int x, int y, game_manager &manager, board &chessboard, sf::Event &event)
{
    if (first_piece->kind_ == KING && abs(x - first_piece->x_) == 2)
        switch (x)
        {
            case 2:
                chessboard.squares_[0][y].piece_ptr_->relocate(3, y, chessboard);
                chessboard.squares_[3][y].piece_ptr_->was_relocated_ = true;
                break;
            case 6:
                chessboard.squares_[7][y].piece_ptr_->relocate(5, y, chessboard);
                chessboard.squares_[5][y].piece_ptr_->was_relocated_ = true;
                break;
            default:
                assert(0);
        }

    if (first_piece->kind_ == PAWN && abs(x - first_piece->x_) == 1 && !chessboard.squares_[x][y].piece_ptr_)
        switch (y)
        {
            case 2:
                delete(chessboard.squares_[x][3].piece_ptr_);
                manager.game_objects_.erase(chessboard.squares_[x][3].piece_ptr_);
                chessboard.squares_[x][3].piece_ptr_ = nullptr;
                break;
            case 6:
                delete(chessboard.squares_[x][5].piece_ptr_);
                manager.game_objects_.erase(chessboard.squares_[x][5].piece_ptr_);
                chessboard.squares_[x][5].piece_ptr_ = nullptr;
                break;
            default:
                assert(0);
        }

    if (first_piece->kind_ == PAWN && abs(y - first_piece->y_) == 2)
        manager.jumped_pawn_ = first_piece;
    else
        manager.jumped_pawn_ = nullptr;

    piece *second_piece = chessboard.squares_[x][y].piece_ptr_;
    if (second_piece)
    {
        delete(second_piece);
        manager.game_objects_.erase(second_piece);
    }

    first_piece->relocate(x, y, chessboard);
    first_piece->was_relocated_ = true;

    if (first_piece->kind_ == PAWN && y % 7 == 0)
        pawn_reborn(first_piece, manager, chessboard, event);
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

    if((y0 - dy) % 7 == 0 && IS_EMPTY(x0, y0 + dy) && IS_EMPTY(x0, y0 + 2 * dy))
        INSERT(x0, y0 + 2 * dy);

    if (IS_EMPTY(x0, y0 + dy))
        INSERT(x0, y0 + dy);

    for (int i = -1; i <= 1; i += 2)
        if (IS_AVAILABLE(x0 + i, y0 + dy))
            INSERT(x0 + i, y0 + dy);
        else if (IS_EMPTY(x0 + i, y0 + dy) && manager.jumped_pawn_ &&
            manager.jumped_pawn_->x_ == x0 + i &&
            manager.jumped_pawn_->y_ == y0)
            INSERT(x0 + i, y0 + dy);

    
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


    check_squares(manager, chessboard, this->available_squares_, x0, y0);

    if (!this->was_relocated_ && !this->checked(chessboard))
    {
        if (IS_EMPTY(x0 - 1, y0) && IS_EMPTY(x0 - 2, y0) && IS_EMPTY(x0 - 3, y0) && 
            chessboard.squares_[0][y0].piece_ptr_ && !chessboard.squares_[0][y0].piece_ptr_->was_relocated_ &&
            this->available_squares_.find(std::pair<int, int>(x0 - 1, y0)) != this->available_squares_.end())
            INSERT(x0 - 2, y0);

        if (IS_EMPTY(x0 + 1, y0) && IS_EMPTY(x0 + 2, y0) && 
            chessboard.squares_[7][y0].piece_ptr_ && !chessboard.squares_[7][y0].piece_ptr_->was_relocated_ &&
            this->available_squares_.find(std::pair<int, int>(x0 + 1, y0)) != this->available_squares_.end())
            INSERT(x0 + 2, y0);
    }

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