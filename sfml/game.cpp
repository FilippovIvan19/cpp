#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <unistd.h>
#include <assert.h>
#include <string.h>

//can be changed
const int DEATH_DOT_COUNT = 40;
const float SPEED_ADDITION = 0.3;
const float ANGLE_ADDITION = 0.4;
const float DOT_SPRITE_SIZE = 27;
const float DOT_PICTURE_SIZE = 600;
const float BIKE_SPRITE_HEIGHT = 150;
const float BIKE_PICTURE_HEIGHT = 650;
const float BIKE_PICTURE_WIDTH = 400;
const float MAX_TIME_FROM_LAST_JUMP = 0.3;
const float MIN_SPEED = 70;
const float MAX_SPEED = 300;
const float STD_WINDOW_WIDTH = 1600;
const float STD_WINDOW_HEIGHT = 900;
const float WINDOW_WIDTH = 1600;
const float WINDOW_HEIGHT = 900;
const float BIKE_ANIMATION_DELTA = 0.4;
const int MAX_LIFE = 5;


//don't change
const float BIKE_ORIGIN_WIDTH_COEFFICIENT = 0.5;
const float BIKE_ORIGIN_HEIGHT_COEFFICIENT = 0.7;

#include "lib/common_elements.h"
#include "lib/death_dot.h"
#include "lib/motorbike_strip.h"
#include "lib/motorbike.h"
#include "lib/game_manager.h"

//don't change
const int EXIT_APP = -666;
const int  RED_BIKE_DIED = 1;
const int BLUE_BIKE_DIED = 2;
const float EPS = 0.0001;


void input_moving(sf::Event &event, 
    motorbike &red_bike, motorbike &blue_bike);

int check_death_collisions(motorbike &red_bike, motorbike &blue_bike);

int main_menu(sf::RenderWindow &window, sf::Event &event);

int death_func(int bike_died, sf::RenderWindow &window, sf::Event &event, 
    game_manager &manager, sf::Sprite &background_sprite, sf::Font &font);

int end_func(sf::RenderWindow &window, sf::Event &event, sf::Sprite end_menu_sprite,
    game_manager &manager, sf::Sprite &background_sprite);

int game_cycle(sf::RenderWindow &window, sf::Event &event, 
    game_manager &manager, sf::Sprite &background_sprite);

int play_game(sf::RenderWindow &window, sf::Event &event, 
    game_manager &manager, sf::Sprite &background_sprite);


int wait_space(sf::RenderWindow &window, sf::Event &event, int cycles)
{
    for (int i = 0; i < cycles; ++i)
    {
        ;
        if (window.pollEvent(event) && event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
        {
            //window.pollEvent(event);
            return 1;
        }
    }
    return 0;
}

void input_moving(sf::Event &event, 
    motorbike &red_bike, motorbike &blue_bike)
{
    //printf("a\n");
    //red bike movement and animation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        red_bike.speed_ += SPEED_ADDITION;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        red_bike.speed_ -= SPEED_ADDITION;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        red_bike.speed_angle_ += ANGLE_ADDITION;        
        red_bike.frame_y_ = 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        red_bike.speed_angle_ -= ANGLE_ADDITION;        
        red_bike.frame_y_ = 2;
    }


    //blue bike movement and animation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
            || event.type == sf::Event::MouseWheelScrolled
            && event.mouseWheelScroll.delta > 0)
        blue_bike.speed_ += SPEED_ADDITION;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
            || event.type == sf::Event::MouseWheelScrolled
            && event.mouseWheelScroll.delta < 0)
        blue_bike.speed_ -= SPEED_ADDITION;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::SemiColon)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
            || sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        blue_bike.speed_angle_ += ANGLE_ADDITION;        
        blue_bike.frame_y_ = 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
            || sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        blue_bike.speed_angle_ -= ANGLE_ADDITION;        
        blue_bike.frame_y_ = 2;
    }

    //returning to default animation
    if (event.type == sf::Event::KeyReleased
            && (event.key.code == sf::Keyboard::K || event.key.code == sf::Keyboard::SemiColon
            || event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
            || event.type == sf::Event::MouseButtonReleased
            && (event.mouseButton.button == sf::Mouse::Right || event.mouseButton.button == sf::Mouse::Left))
        blue_bike.frame_y_ = 0;

    if (event.type == sf::Event::KeyReleased
            && (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D))
        red_bike.frame_y_ = 0;
}

int check_death_collisions(motorbike &red_bike, motorbike &blue_bike)
{
    if (red_bike.out_rect(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)))
        return RED_BIKE_DIED;

    if (blue_bike.out_rect(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)))
        return BLUE_BIKE_DIED;


    int red_strip_len  =  red_bike.strip_.len_;
    for (int i = 0; i < red_strip_len; ++i)
    {
        if (red_bike.strip_.dots_[i].visibility_
            && i != red_bike.strip_.visible_dots_.back()
            && red_bike.touch(red_bike.strip_.dots_[i].sprite_.getGlobalBounds()))
                return RED_BIKE_DIED;

        if (red_bike.strip_.dots_[i].visibility_
            && blue_bike.touch(red_bike.strip_.dots_[i].sprite_.getGlobalBounds()))
                return BLUE_BIKE_DIED;
    }


    int blue_strip_len = blue_bike.strip_.len_;
    for (int i = 0; i < blue_strip_len; ++i)
    {
        if (blue_bike.strip_.dots_[i].visibility_
            && i != blue_bike.strip_.visible_dots_.back()
            && blue_bike.touch(blue_bike.strip_.dots_[i].sprite_.getGlobalBounds()))
                return BLUE_BIKE_DIED;

        if (blue_bike.strip_.dots_[i].visibility_
            && red_bike.touch(blue_bike.strip_.dots_[i].sprite_.getGlobalBounds()))
                return RED_BIKE_DIED;
    }


    return 0;
}

int main_menu(sf::RenderWindow &window, sf::Event &event)
{
    sf::Font font;
    font.loadFromFile("font.otf");
    sf::Text text("", font, 80);
    text.setFillColor(sf::Color::Red);
    //text.setStyle(sf::Text::Bold);

    sf::Texture menu_texture;
    menu_texture.loadFromFile("textures/menu.png");
    sf::Sprite menu_sprite(menu_texture);
    menu_texture.setSmooth(true);
    common_elements menu(&window, 0, 0, 
        menu_sprite, STD_WINDOW_WIDTH, STD_WINDOW_HEIGHT);
    menu.sprite_.setScale(WINDOW_WIDTH / STD_WINDOW_WIDTH, WINDOW_HEIGHT / STD_WINDOW_HEIGHT);

    int life_count = 1;
    std::string life_count_s = std::to_string(life_count);
    text.setPosition(WINDOW_WIDTH * 0.485, WINDOW_HEIGHT * 0.47);

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
                return EXIT_APP;
            }
            
            if (event.type == sf::Event::KeyPressed
                && (event.key.code == sf::Keyboard::Down))
            {
                menu.frame_y_ = (menu.frame_y_ + 1) % 3;
                //window.pollEvent(event);
            }
            
            if (event.type == sf::Event::KeyPressed
                && (event.key.code == sf::Keyboard::Up))
            {
                menu.frame_y_ = (menu.frame_y_ + 2) % 3;
                //window.pollEvent(event);
            }
            
            if (event.type == sf::Event::KeyPressed
                && (menu.frame_y_ == 1)
                && (event.key.code == sf::Keyboard::Right))
            {
                life_count = (life_count) % MAX_LIFE + 1;
                //window.pollEvent(event);
            }
            
            if (event.type == sf::Event::KeyPressed
                && (menu.frame_y_ == 1)
                && (event.key.code == sf::Keyboard::Left))
            {
                life_count = (life_count + MAX_LIFE - 2) % MAX_LIFE + 1;
                //window.pollEvent(event);
            }
            
            if (event.type == sf::Event::KeyPressed
                && (event.key.code == sf::Keyboard::Return))
            {
                if (menu.frame_y_ == 2)
                    return EXIT_APP;
                else if (menu.frame_y_ == 0)
                    return life_count;
                //window.pollEvent(event);
            }
        }

        window.clear();
            menu.draw();
            life_count_s = std::to_string(life_count);
            text.setString(life_count_s.c_str());
            window.draw(text);
        window.display();
    }

    return life_count;
}

int death_func(int bike_died, sf::RenderWindow &window, sf::Event &event, 
    game_manager &manager, sf::Sprite &background_sprite, sf::Font &font)
{
    motorbike  &red_bike = *(motorbike*)manager.game_objects_[0];
    motorbike &blue_bike = *(motorbike*)manager.game_objects_[1];

    switch (bike_died)
        {
            case RED_BIKE_DIED:
                window.setTitle("RED DIED");
                red_bike.frame_y_ = 3;
                red_bike.frame_x_ = 0;
                red_bike.lives_--;
                break;

            case BLUE_BIKE_DIED:
                window.setTitle("BLUE DIED");
                blue_bike.frame_y_ = 3;
                blue_bike.frame_x_ = 0;
                blue_bike.lives_--;
                break;

            default:
                assert(0);
        }


    window.clear();
        window.draw(background_sprite);
        //red_bike.draw_phys();
        //blue_bike.draw_phys();
        manager.draw();
    window.display();

    //sleep(1);
    //printf("1\n");
    wait_space(window, event, 400000);
    //printf("2\n");


    char text_str[15] = "";

    sf::Text red_text("", font, 80);
    red_text.setOutlineThickness(5);
    red_text.setOutlineColor(sf::Color::White);
    red_text.setFillColor(sf::Color::Red);
    std::string red_life_count_s = std::to_string(red_bike.lives_);
    strcpy(text_str, "red lives: ");
    strcat(text_str, red_life_count_s.c_str());
    red_text.setString(text_str);
    red_text.setPosition(70 / STD_WINDOW_WIDTH * WINDOW_WIDTH, 70 / STD_WINDOW_HEIGHT * WINDOW_HEIGHT);
    
    sf::Text blue_text("", font, 80);
    blue_text.setOutlineThickness(5);
    blue_text.setOutlineColor(sf::Color::White);
    blue_text.setFillColor(sf::Color::Blue);
    std::string blue_life_count_s = std::to_string(blue_bike.lives_);
    strcpy(text_str, "blue lives: ");
    strcat(text_str, blue_life_count_s.c_str());
    blue_text.setString(text_str);
    blue_text.setPosition(1000 / STD_WINDOW_WIDTH * WINDOW_WIDTH, 70 / STD_WINDOW_HEIGHT * WINDOW_HEIGHT);



    window.clear();
        window.draw(background_sprite);
        window.draw(red_text);
        window.draw(blue_text);
    window.display();

    //sleep(1);
    //printf("3\n");
    wait_space(window, event, 400000);
    //printf("4\n");


    motorbike new_red_bike(&window, 400 / STD_WINDOW_WIDTH * WINDOW_WIDTH, 700 / STD_WINDOW_HEIGHT * WINDOW_HEIGHT, 
        red_bike.sprite_, BIKE_PICTURE_WIDTH, BIKE_PICTURE_HEIGHT,
        red_bike.strip_.sprite_, DOT_PICTURE_SIZE, DOT_PICTURE_SIZE, DEATH_DOT_COUNT, red_bike.lives_);
    motorbike new_blue_bike(&window, 1200 / STD_WINDOW_WIDTH * WINDOW_WIDTH, 700 / STD_WINDOW_HEIGHT * WINDOW_HEIGHT, 
        blue_bike.sprite_, BIKE_PICTURE_WIDTH, BIKE_PICTURE_HEIGHT,
        blue_bike.strip_.sprite_, DOT_PICTURE_SIZE, DOT_PICTURE_SIZE, DEATH_DOT_COUNT, blue_bike.lives_);

    std::swap(red_bike, new_red_bike);
    std::swap(blue_bike, new_blue_bike);

    window.setTitle("TRON");

    return 0;
}

int end_func(sf::RenderWindow &window, sf::Event &event, sf::Sprite end_menu_sprite,
    game_manager &manager, sf::Sprite &background_sprite)
{

    motorbike  &red_bike = *(motorbike*)manager.game_objects_[0];
    motorbike &blue_bike = *(motorbike*)manager.game_objects_[1];


    sf::Font font;
    font.loadFromFile("font.otf");
    sf::Text text("", font, 80);
    text.setPosition(WINDOW_WIDTH * 0.4, WINDOW_HEIGHT * 0.1);

    if (red_bike.lives_)
    {
        text.setFillColor(sf::Color::Red);
        text.setString("RED WIN");
    }
    if (blue_bike.lives_)
    {
        text.setFillColor(sf::Color::Blue);
        text.setString("BLUE WIN");
    }


    common_elements end_menu(&window, 0, 0, 
        end_menu_sprite, STD_WINDOW_WIDTH, STD_WINDOW_HEIGHT);

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
                return 0;
            }
            
            if (event.type == sf::Event::KeyPressed
                    && (event.key.code == sf::Keyboard::Down
                    || event.key.code == sf::Keyboard::Up))
            {
                end_menu.frame_y_ = (end_menu.frame_y_ + 1) % 2;
                //window.pollEvent(event);
            }
            
            if (event.type == sf::Event::KeyPressed
                    && (event.key.code == sf::Keyboard::Return))
            {
                if (end_menu.frame_y_ == 1)
                    return 0;
                else if (end_menu.frame_y_ == 0)
                {
                    play_game(window, event, manager, background_sprite);
                    return 0;
                }
                //window.pollEvent(event);
            }
        }

        window.clear();
            end_menu.draw();
            window.draw(text);
        window.display();
    }

    return 0;
}

int game_cycle(sf::RenderWindow &window, sf::Event &event, 
    game_manager &manager, sf::Sprite &background_sprite)
{

    motorbike  &red_bike = *(motorbike*)manager.game_objects_[0];
    motorbike &blue_bike = *(motorbike*)manager.game_objects_[1];

    sf::Clock main_clock;
    float dt = 0;
    float time_from_last_animation = 0;
    sf::Font font;
    font.loadFromFile("font.otf");
    
    sf::Texture end_menu_texture;
    end_menu_texture.loadFromFile("textures/end_menu.png");
    end_menu_texture.setSmooth(true);
    sf::Sprite end_menu_sprite(end_menu_texture);
    end_menu_sprite.setScale(WINDOW_WIDTH / STD_WINDOW_WIDTH, WINDOW_HEIGHT / STD_WINDOW_HEIGHT);
    common_elements end_menu(&window, 0, 0, 
        end_menu_sprite, WINDOW_WIDTH, WINDOW_HEIGHT);


    while (window.isOpen())
    {
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
                return 0;
            }

        input_moving(event, red_bike, blue_bike);

        if (time_from_last_animation / 1000000 >= BIKE_ANIMATION_DELTA)
        {
            if (red_bike.speed_ > EPS)
                red_bike.frame_x_ = (red_bike.frame_x_ + 1) % 2;

            if (blue_bike.speed_ > EPS)
                blue_bike.frame_x_ = (blue_bike.frame_x_ + 1) % 2;

            time_from_last_animation = 0;
        }

        manager.move(dt);


        window.clear();
            window.draw(background_sprite);
            //red_bike.draw_phys();
            // blue_bike.draw_phys();
            manager.draw();
        window.display();


        int bike_died = check_death_collisions(red_bike, blue_bike);
        if (bike_died)
        {
            death_func(bike_died, window, event, manager, background_sprite, font);
            main_clock.restart();
        }

        if (!(blue_bike.lives_ * red_bike.lives_))
            break;

        dt = main_clock.getElapsedTime().asMicroseconds();
        time_from_last_animation += dt;
        main_clock.restart();
    }

    end_func(window, event, end_menu_sprite, manager, background_sprite);

    return 0;
}

int play_game(sf::RenderWindow &window, sf::Event &event, 
    game_manager &manager, sf::Sprite &background_sprite)
{
    motorbike  &red_bike = *(motorbike*)manager.game_objects_[0];
    motorbike &blue_bike = *(motorbike*)manager.game_objects_[1];


    int life_count = main_menu(window, event);
    if (life_count == EXIT_APP)
        return 0;

    red_bike.lives_  = life_count;
    blue_bike.lives_ = life_count;

    game_cycle(window, event, manager, background_sprite);

    return 0;
}



/////////////////////////////
//g++ game.cpp -c
//g++ game.o -o game -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
/////////////////////////////



int main(int argc, char const *argv[])
{    
    sf::Music main_theme;
    main_theme.openFromFile("music/mus1.ogg");
    main_theme.setLoop(true);
    if (argc > 1 && !strcmp(argv[1], "play_music"))
        main_theme.play();

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TRON");

    
    sf::Texture background_texture;
    background_texture.loadFromFile("textures/back1.png");
    background_texture.setSmooth(true);
    sf::Sprite background_sprite(background_texture);
    background_sprite.setScale(WINDOW_WIDTH / STD_WINDOW_WIDTH, WINDOW_HEIGHT / STD_WINDOW_HEIGHT);

    sf::Texture red_bike_texture;
    red_bike_texture.loadFromFile("textures/red_bike.png");
    red_bike_texture.setSmooth(true);
    sf::Sprite  red_bike_sprite(red_bike_texture);

    sf::Texture blue_bike_texture;
    blue_bike_texture.loadFromFile("textures/blue_bike.png");
    blue_bike_texture.setSmooth(true);
    sf::Sprite blue_bike_sprite(blue_bike_texture);

    sf::Texture dot_texture;
    dot_texture.loadFromFile("textures/dymt.png");
    dot_texture.setSmooth(true);
    sf::Sprite dot_sprite(dot_texture);

    sf::Event event;


    motorbike red_bike(&window, 400 / STD_WINDOW_WIDTH * WINDOW_WIDTH, 700 / STD_WINDOW_HEIGHT * WINDOW_HEIGHT, 
        red_bike_sprite, BIKE_PICTURE_WIDTH, BIKE_PICTURE_HEIGHT,
        dot_sprite, DOT_PICTURE_SIZE, DOT_PICTURE_SIZE, DEATH_DOT_COUNT, 1);
    motorbike blue_bike(&window, 1200 / STD_WINDOW_WIDTH * WINDOW_WIDTH, 700 / STD_WINDOW_HEIGHT * WINDOW_HEIGHT, 
        blue_bike_sprite, BIKE_PICTURE_WIDTH, BIKE_PICTURE_HEIGHT,
        dot_sprite, DOT_PICTURE_SIZE, DOT_PICTURE_SIZE, DEATH_DOT_COUNT, 1);

    //motorbike *red_bike_ptr = &red_bike;
    //motorbike *blue_bike_ptr = &blue_bike;

    game_manager manager;
    manager.add_obj(&red_bike);
    manager.add_obj(&blue_bike);

    play_game(window, event, manager, background_sprite);


    return 0;
}