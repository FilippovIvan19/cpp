//don't change
const float PI = 3.1415926535;



class common_elements
{
public:
    common_elements(sf::RenderWindow *window, float x0, float y0, 
        sf::Sprite sprite, float frame_width, float frame_height);
    common_elements();
   ~common_elements();

    float x_;
    float y_;
    float speed_angle_;
    float speed_;
    float frame_width_;
    float frame_height_;
    sf::Sprite sprite_;
    sf::RenderWindow *window_ptr_;
    int frame_x_;
    int frame_y_;

    virtual void draw();
    virtual void move(float dt);
};

common_elements::common_elements(sf::RenderWindow *window_ptr, float x0, float y0,
        sf::Sprite sprite, float frame_width, float frame_height):
    x_(x0),
    y_(y0),
    speed_angle_(0),
    speed_(0),
    sprite_(sprite),
    frame_width_(frame_width),
    frame_height_(frame_height),
    window_ptr_(window_ptr),
    frame_x_(0),
    frame_y_(0)
    {
        this->sprite_.setPosition(x0, y0);
        this->sprite_.setTextureRect(sf::IntRect(0, 0, frame_width_, frame_height_));
    }

common_elements::common_elements():
    x_(0),
    y_(0),
    speed_angle_(0),
    speed_(0),
    sprite_(sf::Sprite()),
    frame_width_(100),
    frame_height_(100),
    window_ptr_(nullptr),
    frame_x_(0),
    frame_y_(0)
    {
        this->sprite_.setPosition(0, 0);
    }

common_elements::~common_elements()
{}

void common_elements::draw()
{
    this->sprite_.setTextureRect(sf::IntRect
        (frame_x_ * frame_width_, frame_y_ * frame_height_, frame_width_, frame_height_));
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