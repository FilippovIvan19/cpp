void draw_all(sf::RenderWindow &window, game_manager &manager, board &chessboard)
{
    if (manager.white_king_->checked(chessboard) &&
        chessboard.squares_[manager.white_king_->x_][manager.white_king_->y_].frame_ != PURPLE_SQUARE)
        chessboard.squares_[manager.white_king_->x_][manager.white_king_->y_].change_frame(RED_SQUARE);
    if (manager.black_king_->checked(chessboard) &&
        chessboard.squares_[manager.black_king_->x_][manager.black_king_->y_].frame_ != PURPLE_SQUARE)
        chessboard.squares_[manager.black_king_->x_][manager.black_king_->y_].change_frame(RED_SQUARE);

    window.clear();
        chessboard.draw();
        manager.draw();
    window.display();

    chessboard.squares_[manager.white_king_->x_][manager.white_king_->y_].change_frame(NORMAL_SQUARE);
    chessboard.squares_[manager.black_king_->x_][manager.black_king_->y_].change_frame(NORMAL_SQUARE);
}


int is_game_over(game_manager &manager, board &chessboard)
{
    if (manager.white_king_->checked(chessboard))
    {
        for (piece *cur_piece: manager.game_objects_)
            if (cur_piece->color_ == WHITE && !cur_piece->available_squares_.empty())
                return 0;
        return BLACK_WIN;
    }

    if (manager.black_king_->checked(chessboard))
    {
        for (piece *cur_piece: manager.game_objects_)
            if (cur_piece->color_ == BLACK && !cur_piece->available_squares_.empty())
                return 0;
        return WHITE_WIN;
    }
}


std::pair<int, int> wait_chosen_square(sf::RenderWindow &window, sf::Event &event, board &chessboard)
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


void draw_available_squares(sf::RenderWindow &window, game_manager &manager, board &chessboard,
    piece *chosen_piece)
{
    chessboard.squares_[chosen_piece->x_][chosen_piece->y_].change_frame(PURPLE_SQUARE);
    for (std::pair<int, int> square: chosen_piece->available_squares_)
        chessboard.squares_[square.first][square.second].change_frame(GREEN_SQUARE);

    draw_all(window, manager, chessboard);

    chessboard.squares_[chosen_piece->x_][chosen_piece->y_].change_frame(NORMAL_SQUARE);
    for (std::pair<int, int> square: chosen_piece->available_squares_)
        chessboard.squares_[square.first][square.second].change_frame(NORMAL_SQUARE);
}


int move(sf::RenderWindow &window, sf::Event &event, 
    game_manager &manager, board &chessboard, Color turn_color)
{
    int mouse_x = -1;
    int mouse_y = -1;
    std::pair<int, int> chosen_square = std::pair<int, int>(-1, -1);
    piece *first_piece = nullptr;
    piece *second_piece = nullptr;

    while (!first_piece || first_piece->color_ != turn_color)
    {
        chosen_square = wait_chosen_square(window, event, chessboard);
        if (chosen_square == std::pair<int, int>(EXIT, EXIT))
            return EXIT;
        first_piece = chessboard.squares_[chosen_square.first][chosen_square.second].piece_ptr_;
    }
    
    draw_available_squares(window, manager, chessboard, first_piece);

    do
    {
        chosen_square = wait_chosen_square(window, event, chessboard);
        if (chosen_square == std::pair<int, int>(EXIT, EXIT))
            return EXIT;
        second_piece = chessboard.squares_[chosen_square.first][chosen_square.second].piece_ptr_;
        if (second_piece && second_piece->color_ == turn_color)
        {
            first_piece = second_piece;
            draw_available_squares(window, manager, chessboard, first_piece);
        }
    }
    while (first_piece->available_squares_.find(chosen_square) == first_piece->available_squares_.end());

    relocate(first_piece, chosen_square.first, chosen_square.second, manager, chessboard);
    draw_all(window, manager, chessboard);
    return 0;
}


void end_game(sf::RenderWindow &window, game_manager &manager, board &chessboard, int game_over)
{
    sf::Font font;
    font.loadFromFile("textures/font.otf");
    char text_str[15] = "";

    switch (game_over)
        {
            case WHITE_WIN:
                window.setTitle("WHITE WIN");
                strcpy(text_str, "white win");
                break;

            case BLACK_WIN:
                window.setTitle("BLACK WIN");
                strcpy(text_str, "black win");
                break;

            default:
                assert(0);
        }


    sf::Text text(text_str, font, 80);
    text.setOutlineThickness(5);
    text.setOutlineColor(sf::Color::White);
    text.setFillColor(sf::Color::Red);
    text.setPosition(200, 350);

    window.clear();
        chessboard.draw();
        manager.draw();
        window.draw(text);
    window.display();
}


void game_cycle(sf::RenderWindow &window, sf::Event &event, 
    game_manager &manager, board &chessboard)
{
    Color turn_color = WHITE;
    draw_all(window, manager, chessboard);

    while (window.isOpen())
    {
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
                return;
            }


        manager.update_available_squares(chessboard);
        int game_over = is_game_over(manager, chessboard);
        if (!game_over)
            if (move(window, event, manager, chessboard, turn_color) == EXIT)
            {
                window.close();
                return;
            }
            else {}
        else
        {
            end_game(window, manager, chessboard, game_over);
            wait_chosen_square(window, event, chessboard);
            window.close();
            return;
        }

        turn_color = (Color)(1 - (int)turn_color);
    }
}