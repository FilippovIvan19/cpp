class common_elements
{
public:
    common_elements(sf::RenderWindow *window_ptr, Color color, int x, int y);
    common_elements();
   ~common_elements() {};

    int x_;
    int y_;
    Color color_;
    sf::Sprite sprite_;
    sf::RenderWindow *window_ptr_;

    void draw();
};

common_elements::common_elements(sf::RenderWindow *window_ptr, Color color, int x, int y):
    x_(x),
    y_(y),
    window_ptr_(window_ptr),
    color_(color)
    {}

common_elements::common_elements():
    x_(0),
    y_(0),
    window_ptr_(nullptr),
    color_(WHITE)
    {}

void common_elements::draw()
{
    this->window_ptr_->draw(this->sprite_);
}
