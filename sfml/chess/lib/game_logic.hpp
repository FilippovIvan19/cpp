void draw_all(sf::RenderWindow &window, game_manager &manager, board &chessboard)
{
    window.clear();
        chessboard.draw();
        manager.draw();
    window.display();
}


bool game_over(game_manager &manager)
{
    return false;//////////////////////////////
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


std::set<std::pair<int, int>> draw_available_squares(sf::RenderWindow &window, game_manager &manager, board &chessboard,
    piece *chosen_piece, std::set<std::pair<int, int>> &available_squares)
{
    chosen_piece->available_squares(chessboard, available_squares);

    chessboard.squares_[chosen_piece->x_][chosen_piece->y_].change_frame(PURPLE_SQUARE);
    for (std::pair<int, int> square: available_squares)
        chessboard.squares_[square.first][square.second].change_frame(GREEN_SQUARE);

    draw_all(window, manager, chessboard);

    chessboard.squares_[chosen_piece->x_][chosen_piece->y_].change_frame(NORMAL_SQUARE);
    for (std::pair<int, int> square: available_squares)
        chessboard.squares_[square.first][square.second].change_frame(NORMAL_SQUARE);

    return available_squares;
}


int move(sf::RenderWindow &window, sf::Event &event, 
    game_manager &manager, board &chessboard, Color turn_color)
{
    int mouse_x = -1;
    int mouse_y = -1;
    std::pair<int, int> chosen_square = std::pair<int, int>(-1, -1);
    piece *first_piece = nullptr;
    piece *second_piece = nullptr;
    std::set<std::pair<int, int>> available_squares;

    while (!first_piece || first_piece->color_ != turn_color)
    {
        chosen_square = wait_chosen_square(window, event, chessboard);
        if (chosen_square == std::pair<int, int>(EXIT, EXIT))
            return EXIT;
        first_piece = chessboard.squares_[chosen_square.first][chosen_square.second].piece_ptr_;
    }
    
    available_squares = draw_available_squares(window, manager, chessboard, first_piece, available_squares);

    do
    {
        chosen_square = wait_chosen_square(window, event, chessboard);
        if (chosen_square == std::pair<int, int>(EXIT, EXIT))
            return EXIT;
        second_piece = chessboard.squares_[chosen_square.first][chosen_square.second].piece_ptr_;
        if (second_piece && second_piece->color_ == turn_color)
        {
            first_piece = second_piece;
            available_squares = draw_available_squares(window, manager, chessboard, first_piece, available_squares);
        }
    }
    while (available_squares.find(chosen_square) == available_squares.end());

    if (second_piece)
    {
        delete(second_piece);
        manager.game_objects_.erase(second_piece);
    }

    first_piece->relocate(chosen_square.first, chosen_square.second, chessboard);

    draw_all(window, manager, chessboard);

    return 0;
}


void game_cycle(sf::RenderWindow &window, sf::Event &event, 
    game_manager &manager, board &chessboard)
{
    Color turn_color = WHITE;
    draw_all(window, manager, chessboard);

    while (window.isOpen() && !game_over(manager))
    {
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
                return;
            }

        if (move(window, event, manager, chessboard, turn_color) == EXIT)
        {
            window.close();
            return;
        }

        turn_color = (Color)(1 - (int)turn_color);
    }
}