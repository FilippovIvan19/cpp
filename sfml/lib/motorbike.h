//don't change
const float BIKE_SPRITE_SIZE_COEFFICIENT = BIKE_SPRITE_HEIGHT / BIKE_PICTURE_HEIGHT;
const float PHYSICAL_WIDTH_COEFFICIENT = 0.3;
const float PHYSICAL_HEIGTH_COEFFICIENT = 0.9;



class motorbike: public common_elements
{
public:
    motorbike(sf::RenderWindow *window_ptr, float x0, float y0,
        sf::Sprite bike_sprite, float bike_frame_width, float bike_frame_height,
        sf::Sprite dot_sprite, float dot_frame_width, float dot_frame_height, int strip_len, int lives);
   ~motorbike();

    motorbike_strip strip_;
    sf::CircleShape physical_shape_;
    int lives_;

    virtual void move(float dt) override;
    virtual void draw() override;
    void draw_phys();
    bool out_rect(sf::FloatRect rect);
    bool touch(sf::FloatRect rect);
};

motorbike::motorbike(sf::RenderWindow *window_ptr, float x0, float y0,
        sf::Sprite bike_sprite, float bike_frame_width, float bike_frame_height,
        sf::Sprite dot_sprite, float dot_frame_width, float dot_frame_height, int strip_len, int lives):
    common_elements(window_ptr, x0, y0, bike_sprite, bike_frame_width, bike_frame_height),
    strip_(window_ptr, x0, y0, dot_sprite, dot_frame_width, dot_frame_height, strip_len),
    lives_(lives),
    physical_shape_(sf::CircleShape())
    {
        this->sprite_.setOrigin
            (this->sprite_.getLocalBounds().width * BIKE_ORIGIN_WIDTH_COEFFICIENT, 
             this->sprite_.getLocalBounds().height * BIKE_ORIGIN_HEIGHT_COEFFICIENT);

        this->sprite_.setScale(BIKE_SPRITE_SIZE_COEFFICIENT, BIKE_SPRITE_SIZE_COEFFICIENT);

        this->physical_shape_.setPointCount(8);
        this->physical_shape_.setRadius(this->sprite_.getGlobalBounds().height / 2);
        this->physical_shape_.setScale
            (this->sprite_.getGlobalBounds().width / this->sprite_.getGlobalBounds().height * PHYSICAL_WIDTH_COEFFICIENT, PHYSICAL_HEIGTH_COEFFICIENT);


        this->physical_shape_.setOrigin
            (this->physical_shape_.getLocalBounds().width * BIKE_ORIGIN_WIDTH_COEFFICIENT, 
             this->physical_shape_.getLocalBounds().height * BIKE_ORIGIN_HEIGHT_COEFFICIENT);

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

    this->physical_shape_.setPosition(this->x_, this->y_);
    this->physical_shape_.setRotation(speed_angle_);
    
    this->strip_.move(dt, this->x_ - DEATH_DOT_FROM_BIKE_DISTANCE * sin(this->speed_angle_ * PI / 180),
        this->y_ + DEATH_DOT_FROM_BIKE_DISTANCE * cos(this->speed_angle_ * PI / 180));
}

void motorbike::draw()
{
    this->common_elements::draw();
    this->strip_.draw();
}

void motorbike::draw_phys()
{
    this->window_ptr_->draw(physical_shape_);

    sf::CircleShape circ(5);
    int count = this->physical_shape_.getPointCount();
    
    for (int i = 0; i < count; ++i)
    {
        circ.setPosition(this->physical_shape_.getTransform().transformPoint(this->physical_shape_.getPoint(i)));
        circ.setFillColor (sf::Color::Green);
        this->window_ptr_->draw(circ);
    }   
}

bool motorbike::touch(sf::FloatRect rect)
{
    bool ans = false;

    int count = this->physical_shape_.getPointCount();

    for (int i = 0; i < count; ++i)
    {
        ans = ans || rect.contains(this->physical_shape_.getTransform().
            transformPoint(this->physical_shape_.getPoint(i)));
    }

    return ans;
}

bool motorbike::out_rect(sf::FloatRect rect)
{
    bool ans = true;

    int count = this->physical_shape_.getPointCount();

    for (int i = 0; i < count; ++i)
    {
        ans = ans && rect.contains(this->physical_shape_.getTransform().
            transformPoint(this->physical_shape_.getPoint(i)));
    }

    return !ans;
}
