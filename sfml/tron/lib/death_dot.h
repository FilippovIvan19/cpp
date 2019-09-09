//don't change
const float DOT_SPRITE_SIZE_COEFFICIENT = DOT_SPRITE_SIZE / DOT_PICTURE_SIZE;



class death_dot: public common_elements
{
public:
    death_dot(sf::RenderWindow *window_ptr, float x0, float y0,
        sf::Sprite sprite, float frame_width, float frame_height);
    death_dot();
   ~death_dot();

    bool visibility_;

    void jump(float x, float y);
    virtual void draw() override;
    virtual void move(float dt) override;
};

death_dot::death_dot(sf::RenderWindow *window_ptr, float x0, float y0,
        sf::Sprite sprite, float frame_width, float frame_height):
    common_elements(window_ptr, x0, y0, sprite, frame_width, frame_height),
    visibility_(false)
    {
        this->sprite_.setOrigin
            (this->sprite_.getLocalBounds().width  / 2, 
             this->sprite_.getLocalBounds().height / 2);

        this->sprite_.setPosition(x0, y0);
        this->sprite_.setScale(DOT_SPRITE_SIZE_COEFFICIENT, DOT_SPRITE_SIZE_COEFFICIENT);
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
}

void death_dot::move(float dt)
{}

void death_dot::draw()
{
    if (this->visibility_)
        this->common_elements::draw();
}