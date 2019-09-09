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


piece *find_piece(game_manager &manager, int x, int y)
{
    for (piece *obj: manager.game_objects_)
        if(obj->x_ == x && obj->y_ == y)
            return obj;
    return nullptr;
}


int move(sf::RenderWindow &window, sf::Event &event, 
    game_manager &manager, board &chessboard, Color turn_color)
{
    int mouse_x = -1;
    int mouse_y = -1;
    square *chosen_square = nullptr;
    piece *chosen_piece = nullptr;
    piece *attacked_piece = nullptr;
    
    printf("1\n");

    while (window.waitEvent(event))
    {
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
            return -1;
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            mouse_x = event.mouseButton.x / SQUARE_SIZE;
            mouse_y = event.mouseButton.y / SQUARE_SIZE;

            chosen_piece = find_piece(manager, mouse_x, mouse_y);
            if (chosen_piece && chosen_piece->color_ == turn_color)
                break;
        }
    }
    
    printf("2\n");

    chosen_square = &chessboard.squares_[mouse_x][mouse_y];
    chosen_square->change_frame(PURPLE_SQUARE);
    draw_all(window, manager, chessboard);


    while (window.waitEvent(event))
    {
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
            return -1;
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            mouse_x = event.mouseButton.x / SQUARE_SIZE;
            mouse_y = event.mouseButton.y / SQUARE_SIZE;

            attacked_piece = find_piece(manager, mouse_x, mouse_y);
            if (!attacked_piece || attacked_piece->color_ != turn_color)
                break;
        }
    }
    
    printf("3\n");

    if (attacked_piece)
        manager.game_objects_.erase(attacked_piece);

    chosen_piece->relocate(mouse_x, mouse_y);

    chosen_square->change_frame(NORMAL_SQUARE);
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

        if (move(window, event, manager, chessboard, turn_color) == -1)
        {
            window.close();
            return;
        }

        turn_color = (Color)(1 - (int)turn_color);
    }
}