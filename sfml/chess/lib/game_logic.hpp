void draw_all(sf::RenderWindow *window_ptr, game_manager &manager, board &chessboard)
{
    if (manager.white_king_->checked(chessboard) &&
        chessboard.squares_[manager.white_king_->x_][manager.white_king_->y_].frame_ != PURPLE_SQUARE)
        chessboard.squares_[manager.white_king_->x_][manager.white_king_->y_].change_frame(RED_SQUARE);
    if (manager.black_king_->checked(chessboard) &&
        chessboard.squares_[manager.black_king_->x_][manager.black_king_->y_].frame_ != PURPLE_SQUARE)
        chessboard.squares_[manager.black_king_->x_][manager.black_king_->y_].change_frame(RED_SQUARE);

    window_ptr->clear();
        chessboard.draw();
        manager.draw();
    window_ptr->display();

    chessboard.squares_[manager.white_king_->x_][manager.white_king_->y_].change_frame(NORMAL_SQUARE);
    chessboard.squares_[manager.black_king_->x_][manager.black_king_->y_].change_frame(NORMAL_SQUARE);
}


int is_game_over(Color cur_color, game_manager &manager, board &chessboard)
{
    for (piece *cur_piece: manager.game_objects_)
        if (cur_piece->color_ == cur_color && !cur_piece->available_squares_.empty())
            return 0;
    if (cur_color == WHITE)
    {
        if (manager.white_king_->checked(chessboard))
            return BLACK_WIN;
        else
            return STALEMATE;
    }
    else
    {
        if (manager.black_king_->checked(chessboard))
            return WHITE_WIN;
        else
            return STALEMATE;
    }
}


void draw_available_squares(sf::RenderWindow *window_ptr, game_manager &manager, board &chessboard,
    piece *chosen_piece)
{
    chessboard.squares_[chosen_piece->x_][chosen_piece->y_].change_frame(PURPLE_SQUARE);
    for (std::pair<int, int> square: chosen_piece->available_squares_)
        chessboard.squares_[square.first][square.second].change_frame(GREEN_SQUARE);

    draw_all(window_ptr, manager, chessboard);

    chessboard.squares_[chosen_piece->x_][chosen_piece->y_].change_frame(NORMAL_SQUARE);
    for (std::pair<int, int> square: chosen_piece->available_squares_)
        chessboard.squares_[square.first][square.second].change_frame(NORMAL_SQUARE);
}


int move(sf::RenderWindow *window_ptr, sf::Event &event, 
    game_manager &manager, board &chessboard, Color turn_color)
{
    int mouse_x = -1;
    int mouse_y = -1;
    std::pair<int, int> chosen_square = std::pair<int, int>(-1, -1);
    piece *first_piece = nullptr;
    piece *second_piece = nullptr;

    while (!first_piece || first_piece->color_ != turn_color)
    {
        chosen_square = wait_chosen_square(window_ptr, event);
        if (chosen_square == std::pair<int, int>(EXIT, EXIT))
            return EXIT;
        first_piece = chessboard.squares_[chosen_square.first][chosen_square.second].piece_ptr_;
    }
    
    draw_available_squares(window_ptr, manager, chessboard, first_piece);

    do
    {
        chosen_square = wait_chosen_square(window_ptr, event);
        if (chosen_square == std::pair<int, int>(EXIT, EXIT))
            return EXIT;
        second_piece = chessboard.squares_[chosen_square.first][chosen_square.second].piece_ptr_;
        if (second_piece && second_piece->color_ == turn_color)
        {
            first_piece = second_piece;
            draw_available_squares(window_ptr, manager, chessboard, first_piece);
        }
    }
    while (first_piece->available_squares_.find(chosen_square) == first_piece->available_squares_.end());

    relocate(first_piece, chosen_square.first, chosen_square.second, manager, chessboard, event);
    draw_all(window_ptr, manager, chessboard);
    return 0;
}


void end_game(sf::RenderWindow *window_ptr, game_manager &manager, board &chessboard, int game_over)
{
    sf::Font font;
    font.loadFromFile("textures/font.otf");
    char text_str[15] = "";

    switch (game_over)
        {
            case WHITE_WIN:
                window_ptr->setTitle("WHITE WIN");
                strcpy(text_str, "white win");
                break;

            case BLACK_WIN:
                window_ptr->setTitle("BLACK WIN");
                strcpy(text_str, "black win");
                break;

            case STALEMATE:
                window_ptr->setTitle("DRAW");
                strcpy(text_str, "draw");
                break;

            default:
                assert(0);
        }


    sf::Text text(text_str, font, 80);
    text.setOutlineThickness(5);
    text.setOutlineColor(sf::Color::White);
    text.setFillColor(sf::Color::Red);
    text.setPosition(200, 350);

    window_ptr->clear();
        chessboard.draw();
        manager.draw();
        window_ptr->draw(text);
    window_ptr->display();
}


void game_cycle(sf::RenderWindow *window_ptr, sf::Event &event, 
    game_manager &manager, board &chessboard)
{
    Color turn_color = WHITE;
    draw_all(window_ptr, manager, chessboard);

    while (window_ptr->isOpen())
    {
        while (window_ptr->pollEvent(event))
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window_ptr->close();
                return;
            }


        manager.update_available_squares(chessboard);
        int game_over = is_game_over(turn_color, manager, chessboard);
        if (!game_over)
            if (move(window_ptr, event, manager, chessboard, turn_color) == EXIT)
            {
                window_ptr->close();
                return;
            }
            else {}
        else
        {
            end_game(window_ptr, manager, chessboard, game_over);
            wait_chosen_square(window_ptr, event);
            window_ptr->close();
            return;
        }

        turn_color = (Color)(1 - (int)turn_color);
    }
}