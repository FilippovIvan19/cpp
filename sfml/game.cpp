#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <math.h>
#include <queue>
#include <unistd.h>

//can be changed
const int DEATH_DOT_COUNT = 50;
const float SPEED_ADDITION = 0.3;
const float ANGLE_ADDITION = 0.4;
const float DOT_SPRITE_SIZE = 18;
const float DOT_PICTURE_SIZE = 600;
const float BIKE_SPRITE_HEIGHT = 300;
const float BIKE_PICTURE_HEIGHT = 650;
const float BIKE_PICTURE_WIDTH = 400;
const float MAX_TIME_FROM_LAST_JUMP = 0.3;
const float MIN_SPEED = 0;
const float MAX_SPEED = 300;
const float WINDOW_WIDTH = 1600;
const float WINDOW_HEIGHT = 900;
const float BIKE_ANIMATION_DELTA = 0.6;


//don't change
const float BIKE_ORIGIN_WIDTH_COEFFICIENT = 0.5;
const float BIKE_ORIGIN_HEIGHT_COEFFICIENT = 0.7;

#include "lib/common_elements.h"
#include "lib/death_dot.h"
#include "lib/motorbike_strip.h"
#include "lib/motorbike.h"
#include "lib/game_manager.h"

//don't change
const int  RED_BIKE_DIED = 1;
const int BLUE_BIKE_DIED = 2;
const float EPS = 0.0001;


void input_processing(sf::RenderWindow *window_ptr, sf::Event *event_ptr, 
    motorbike *red_bike_ptr, motorbike *blue_bike_ptr)
{
    //red bike movement and animation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        red_bike_ptr->speed_ += SPEED_ADDITION;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        red_bike_ptr->speed_ -= SPEED_ADDITION;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        red_bike_ptr->speed_angle_ += ANGLE_ADDITION;        
        red_bike_ptr->frame_y_ = 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        red_bike_ptr->speed_angle_ -= ANGLE_ADDITION;        
        red_bike_ptr->frame_y_ = 2;
    }


    //blue bike movement and animation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)
            || event_ptr->type == sf::Event::MouseWheelScrolled
            && event_ptr->mouseWheelScroll.delta > 0)
        blue_bike_ptr->speed_ += SPEED_ADDITION;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)
            || event_ptr->type == sf::Event::MouseWheelScrolled
            && event_ptr->mouseWheelScroll.delta < 0)
        blue_bike_ptr->speed_ -= SPEED_ADDITION;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::SemiColon)
            || sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        blue_bike_ptr->speed_angle_ += ANGLE_ADDITION;        
        blue_bike_ptr->frame_y_ = 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)
            || sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        blue_bike_ptr->speed_angle_ -= ANGLE_ADDITION;        
        blue_bike_ptr->frame_y_ = 2;
    }

    //returning to default animation
    if (event_ptr->type == sf::Event::KeyReleased
            && (event_ptr->key.code == sf::Keyboard::K || event_ptr->key.code == sf::Keyboard::SemiColon)
            || event_ptr->type == sf::Event::MouseButtonReleased
            && (event_ptr->mouseButton.button == sf::Mouse::Right || event_ptr->mouseButton.button == sf::Mouse::Left))
        blue_bike_ptr->frame_y_ = 0;

    if (event_ptr->type == sf::Event::KeyReleased
            && (event_ptr->key.code == sf::Keyboard::A || event_ptr->key.code == sf::Keyboard::D))
        red_bike_ptr->frame_y_ = 0;
    



}


int check_death_collisions(motorbike *red_bike_ptr, motorbike *blue_bike_ptr)
{
    int red_strip_len  =  red_bike_ptr->strip_.len_;
    for (int i = 0; i < red_strip_len; ++i)
    {
        if (red_bike_ptr->strip_.dots_[i].visibility_
            && i != red_bike_ptr->strip_.visible_dots_.back()
            && red_bike_ptr->touch(red_bike_ptr->strip_.dots_[i].sprite_.getGlobalBounds()))
                return RED_BIKE_DIED;

        if (red_bike_ptr->strip_.dots_[i].visibility_
            && blue_bike_ptr->touch(red_bike_ptr->strip_.dots_[i].sprite_.getGlobalBounds()))
                return BLUE_BIKE_DIED;
    }

    int blue_strip_len = blue_bike_ptr->strip_.len_;
    for (int i = 0; i < blue_strip_len; ++i)
    {
        if (blue_bike_ptr->strip_.dots_[i].visibility_
            && i != blue_bike_ptr->strip_.visible_dots_.back()
            && blue_bike_ptr->touch(blue_bike_ptr->strip_.dots_[i].sprite_.getGlobalBounds()))
                return BLUE_BIKE_DIED;

        if (blue_bike_ptr->strip_.dots_[i].visibility_
            && red_bike_ptr->touch(blue_bike_ptr->strip_.dots_[i].sprite_.getGlobalBounds()))
                return RED_BIKE_DIED;
    }

    return 0;
}









/////////////////////////////
//g++ game.cpp -c
//g++ game.o -o game -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
/////////////////////////////


int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "THRON");

    sf::Texture background_texture;
    background_texture.loadFromFile("textures/background.png");
    sf::Sprite background_sprite(background_texture);
    background_sprite.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));


    sf::Texture red_bike_texture;
    red_bike_texture.loadFromFile("textures/red_bike.png");
    red_bike_texture.setSmooth (true);

    sf::Texture blue_bike_texture;
    blue_bike_texture.loadFromFile("textures/blue_bike.png");
    blue_bike_texture.setSmooth (true);

    sf::Texture dot_texture;
    dot_texture.loadFromFile("textures/dymt.png");
    dot_texture.setSmooth (true);

    sf::Sprite  red_bike_sprite(red_bike_texture);
    sf::Sprite blue_bike_sprite(blue_bike_texture);
    sf::Sprite dot_sprite(dot_texture);


    motorbike  red_bike(&window, 400, 700, 
        red_bike_sprite, BIKE_PICTURE_WIDTH, BIKE_PICTURE_HEIGHT,
        dot_sprite, DOT_PICTURE_SIZE, DOT_PICTURE_SIZE, DEATH_DOT_COUNT);
    motorbike  blue_bike(&window, 1200, 700, 
        blue_bike_sprite, BIKE_PICTURE_WIDTH, BIKE_PICTURE_HEIGHT,
        dot_sprite, DOT_PICTURE_SIZE, DOT_PICTURE_SIZE, DEATH_DOT_COUNT);

    motorbike *red_bike_ptr = &red_bike;
    motorbike *blue_bike_ptr = &blue_bike;

    game_manager manager;
    manager.add_obj(red_bike_ptr);
    manager.add_obj(blue_bike_ptr);

    sf::Clock main_clock;
    sf::Event event;
    float dt = 0;
    float time_from_last_animation = 0;


    while (window.isOpen())
    {
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return 0;
            }

        input_processing(&window, &event, red_bike_ptr, blue_bike_ptr);

        if (time_from_last_animation / 1000000 >= BIKE_ANIMATION_DELTA)
        {
            if (red_bike_ptr->speed_ > EPS)
                red_bike_ptr->frame_x_ = (red_bike_ptr->frame_x_ + 1) % 2;

            if (blue_bike_ptr->speed_ > EPS)
                blue_bike_ptr->frame_x_ = (blue_bike_ptr->frame_x_ + 1) % 2;

            time_from_last_animation = 0;
        }

        //red_bike_ptr->frame_x_ = (red_bike_ptr->frame_x_ + 1) % 2;
        //blue_bike_ptr->frame_x_ = (blue_bike_ptr->frame_x_ + 1) % 2;
        manager.move(dt);

        int bike_died = check_death_collisions(red_bike_ptr, blue_bike_ptr);
        switch (bike_died)
        {
            case RED_BIKE_DIED:
                window.setTitle("RED DIED");
                red_bike_ptr->frame_y_ = 3;
                break;

            case BLUE_BIKE_DIED:
                window.setTitle("BLUE DIED");
                blue_bike_ptr->frame_y_ = 3;
                break;

            default:
                break;
        }


        window.clear();
            window.draw(background_sprite);
            // red_bike_ptr->draw_phys();
            // blue_bike_ptr->draw_phys();
            manager.draw();
        window.display();

        if (bike_died)
        {
            break;
        }

        dt = main_clock.getElapsedTime().asMicroseconds();
        time_from_last_animation += dt;
        main_clock.restart();
    }


    sleep(3);

    return 0;
}