#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <math.h>
#include <queue>

const float PI = 3.1415926535;
const float MIN_SPEED = 30;
const float MAX_SPEED = 300;




float distance(float x1, float y1, float x2, float y2)
{
    return sqrt( (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) );
}



class common_elements
{
public:
    common_elements(sf::RenderWindow *window, float x0, float y0, sf::Sprite sprite);
    common_elements();
   ~common_elements();

    float x_;
    float y_;
    float speed_angle_;
    float speed_;
    sf::Sprite sprite_;
    sf::RenderWindow *window_ptr_;

    virtual void draw();
    virtual void move(float dt);
};

common_elements::common_elements(sf::RenderWindow *window_ptr, float x0, float y0, sf::Sprite sprite):
    x_(x0),
    y_(y0),
    speed_angle_(0),
    speed_(0),
    sprite_(sprite),
    window_ptr_(window_ptr)
    {
        this->sprite_.setPosition(x0, y0);
    }

common_elements::common_elements():
    x_(0),
    y_(0),
    speed_angle_(0),
    speed_(0),
    sprite_(sf::Sprite()),
    window_ptr_(nullptr)
    {
        this->sprite_.setPosition(0, 0);
    }

common_elements::~common_elements()
{}

void common_elements::draw()
{
    this->window_ptr_->draw(this->sprite_);
}

void common_elements::move(float dt)
{
    float speed_x_ = this->speed_ * sin(this->speed_angle_ * PI / 180);
    float speed_y_ = this->speed_ * -cos(this->speed_angle_ * PI / 180);

    this->x_ += speed_x_ * (dt / 1000000);
    this->y_ += speed_y_ * (dt / 1000000);

    this->sprite_.setPosition(this->x_, this->y_);
}







class death_dot: public common_elements
{
public:
    death_dot(sf::RenderWindow *window_ptr, float x0, float y0, sf::Sprite sprite);
    death_dot();
   ~death_dot();

   bool visibility_;

   void jump(float x, float y);
   virtual void draw(); //всё работает и без virtual ->??????????????!!!!!!!!!!!
   virtual void move(float dt); //всё работает и без virtual ->??????????????!!!!!!!!!!!
};

death_dot::death_dot(sf::RenderWindow *window_ptr, float x0, float y0, sf::Sprite sprite):
    common_elements(window_ptr, x0, y0, sprite),
    visibility_(false)
    {
        this->sprite_.setOrigin
            (this->sprite_.getLocalBounds().width  / 2, 
             this->sprite_.getLocalBounds().height / 2);

        this->sprite_.setPosition(x0, y0);
        this->sprite_.setScale(0.05, 0.05);
    }

death_dot::death_dot():
    common_elements(),
    visibility_(false)
    {
        this->sprite_.setOrigin
            (this->sprite_.getLocalBounds().width  / 2, 
             this->sprite_.getLocalBounds().height / 2);

        this->sprite_.setPosition(0, 0);
    }


death_dot::~death_dot()
{}

void death_dot::jump(float x, float y)
{
    this->x_ = x;
    this->y_ = y;

    this->sprite_.setPosition(x, y);

    //printf("jump\n");
}

void death_dot::move(float dt)
{}

void death_dot::draw()
{
    if (this->visibility_)
        this->common_elements::draw();
}






class motorbike_strip: public common_elements
{
public:
    motorbike_strip(sf::RenderWindow *window_ptr, float x0, float y0, sf::Sprite sprite, int len);
   ~motorbike_strip();

    int len_;
    bool lengthened_;
    std::vector<death_dot> dots_;
    // int last_dot_num_;
    // int first_dot_num_;
    float time_from_last_jump_;
    std::queue<int> invisible_dots_;
    std::queue<int> visible_dots_;

    void move(float dt, float x, float y);
    virtual void move(float dt); //всё работает и без virtual ->??????????????!!!!!!!!!!!
    virtual void draw(); //всё работает и без virtual ->??????????????!!!!!!!!!!!
};

motorbike_strip::motorbike_strip(sf::RenderWindow *window_ptr, float x0, float y0, sf::Sprite sprite, int len):
    common_elements(window_ptr, x0, y0, sprite),
    len_(len),
    time_from_last_jump_(0),
    lengthened_(false),
    dots_( std::vector<death_dot>(len * 2) ),
    visible_dots_( std::queue<int>() ),
    invisible_dots_( std::queue<int>() )/*,
    last_dot_num_(0),
    first_dot_num_(0)*/
    {
        for (int i = 0; i < len * 2; ++i)
            this->dots_[i] = death_dot(window_ptr, x0, y0, sprite);

        this->dots_[0].visibility_ = true;
        this->visible_dots_.push(0);

        for (int i = 1; i < len; ++i)
            this->invisible_dots_.push(i);

    }

motorbike_strip::~motorbike_strip()
{}

void motorbike_strip::draw()
{
    int dot_count = this->len_;
    for (int i = 0; i < dot_count; ++i)
        this->dots_[i].draw();
}


void motorbike_strip::move(float dt)
{}

void motorbike_strip::move(float dt, float x, float y)
{
    //////////////////////////////////////////////////////////////////////////////////////
    this->time_from_last_jump_ += dt;

    if (this->time_from_last_jump_ / 1000000 >= 0.3)
    {
        this->time_from_last_jump_ = 0;

        if (this->visible_dots_.size() > 1)
        {
            int dot_num = this->visible_dots_.front();
            this->visible_dots_.pop();
            this->dots_[dot_num].visibility_ = false;

            this->invisible_dots_.push(dot_num);
            //this->last_dot_num_  =  ++this->last_dot_num_ % this->len_;
        }

    }

    int num = this->visible_dots_.back();
    if (30 <= distance(x, y,
        this->dots_[num].x_, this->dots_[num].y_))
    {
        if (!this->invisible_dots_.empty())
        {
            int dot_num = this->invisible_dots_.front();
            this->invisible_dots_.pop();
            this->dots_[dot_num].visibility_ = true;

            this->visible_dots_.push(dot_num);
            this->dots_[dot_num].jump(x, y);
        }
        else
        {
            int dot_num = this->visible_dots_.front();
            this->visible_dots_.pop();

            this->visible_dots_.push(dot_num);
            this->dots_[dot_num].jump(x, y);
        }
    }




    //this->first_dot_num_ = this->last_dot_num_;
}






class motorbike: public common_elements
{
public:
    motorbike(sf::RenderWindow *window_ptr, float x0, float y0, sf::Sprite bike_sprite, sf::Sprite dot_sprite, int strip_len);
   ~motorbike();

    virtual void move(float dt); //всё работает и без virtual ->??????????????!!!!!!!!!!!
    //virtual void draw(); //всё работает и без virtual ->??????????????!!!!!!!!!!!

    motorbike_strip strip_;
};

motorbike::motorbike(sf::RenderWindow *window_ptr, float x0, float y0, sf::Sprite bike_sprite, sf::Sprite dot_sprite, int strip_len):
    common_elements(window_ptr, x0, y0, bike_sprite),
    strip_(window_ptr, x0, y0, dot_sprite, strip_len)
    {
        this->sprite_.setOrigin
            (this->sprite_.getLocalBounds().width / 2, 
             this->sprite_.getLocalBounds().height * 0.7);

        this->sprite_.setScale(0.2, 0.2);
        this->speed_ = MIN_SPEED;
    }

motorbike::~motorbike()
{}

void motorbike::move(float dt)
{
    if (this->speed_ > MAX_SPEED)
        this->speed_ = MAX_SPEED;
    if (this->speed_ < MIN_SPEED)
        this->speed_ = MIN_SPEED;

    this->common_elements::move(dt);

    this->sprite_.setRotation(speed_angle_);

    //int num = this->strip_.first_dot_num_;
    
    
    this->strip_.move(dt, this->x_, this->y_);
    

}

/*void motorbike::draw()
{
    this->common_elements::draw();
    this->strip_.draw();
}*/




/////////////////////////////
//g++ game.cpp -c
//g++ game.o -o game -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
/////////////////////////////


int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 800), "THRON");

    sf::Texture bike_texture;
    bike_texture.loadFromFile("Krasny_pryamoy.png");
    bike_texture.setSmooth (true);

    sf::Texture dot_texture;
    dot_texture.loadFromFile("dymt.png");
    dot_texture.setSmooth (true);

    sf::Sprite bike_sprite(bike_texture);
    bike_sprite.setTextureRect(sf::IntRect(0, 0, 600, 1000));

    sf::Sprite dot_sprite(dot_texture);
    dot_sprite.setTextureRect(sf::IntRect(0, 0, 600, 600));


    motorbike bike0(&window, 400, 300, bike_sprite, dot_sprite, 30);

    
    //bike0.sprite_.setTextureRect(sf::IntRect(0, 0, 50, 33));

    sf::Clock main_clock;
    sf::Event event;
    float dt = 0;

    // bike0.draw();
    // window.draw(sf::Sprite(bike_texture));
    // window.display();
    //(sf::Mouse::isButtonPressed(sf::Mouse::Left))

    while (window.isOpen())
    {
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            bike0.speed_ += 0.3;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            bike0.speed_ -= 0.3;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            bike0.speed_angle_ += 0.2;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            bike0.speed_angle_ -= 0.2;


        window.clear();
        bike0.move(dt);
        bike0.strip_.move(dt);

        bike0.strip_.draw();
        bike0.draw();
        window.display();

        //++i;
        dt = main_clock.getElapsedTime().asMicroseconds();
        main_clock.restart();
    }

    return 0;
}