//don't change
const float DISTANCE_BETWEEN_DOTS = DOT_SPRITE_SIZE;
const float DEATH_DOT_FROM_BIKE_DISTANCE = DOT_SPRITE_SIZE / 2
    + BIKE_SPRITE_HEIGHT * (1 - BIKE_ORIGIN_HEIGHT_COEFFICIENT);



float distance(float x1, float y1, float x2, float y2)
{
    return sqrt( (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) );
}


class motorbike_strip: public common_elements
{
public:
    motorbike_strip(sf::RenderWindow *window_ptr, float x0, float y0,
        sf::Sprite sprite, float frame_width, float frame_height, int len);
   ~motorbike_strip();

    int len_;
    bool lengthened_;
    std::vector<death_dot> dots_;
    float time_from_last_jump_;
    std::queue<int> invisible_dots_;
    std::queue<int> visible_dots_;

    void move(float dt, float x, float y);
    virtual void move(float dt) override;
    virtual void draw()override;
};

motorbike_strip::motorbike_strip(sf::RenderWindow *window_ptr, float x0, float y0,
        sf::Sprite sprite, float frame_width, float frame_height, int len):
    common_elements(window_ptr, x0, y0, sprite, frame_width, frame_height),
    len_(len),
    time_from_last_jump_(0),
    lengthened_(false),
    dots_( std::vector<death_dot>(len * 2) ),
    visible_dots_( std::queue<int>() ),
    invisible_dots_( std::queue<int>() )
    {
        for (int i = 0; i < len * 2; ++i)
            this->dots_[i] = death_dot(window_ptr, x0, y0, sprite, frame_width, frame_height);

        this->dots_[0].visibility_ = true;
        this->visible_dots_.push(0);
        this->dots_[0].jump(x0, y0 + DEATH_DOT_FROM_BIKE_DISTANCE);

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
    this->time_from_last_jump_ += dt;

    if (this->time_from_last_jump_ / 1000000 >= MAX_TIME_FROM_LAST_JUMP)
    {
        this->time_from_last_jump_ = 0;

        if (this->visible_dots_.size() > 1)
        {
            int dot_num = this->visible_dots_.front();
            this->visible_dots_.pop();
            this->dots_[dot_num].visibility_ = false;

            this->invisible_dots_.push(dot_num);
        }
    }

    int num = this->visible_dots_.back();
    if (DISTANCE_BETWEEN_DOTS <= distance(x, y,
        this->dots_[num].x_, this->dots_[num].y_))
    {
        int dot_num = -1;
        if (!this->invisible_dots_.empty())
        {
            dot_num = this->invisible_dots_.front();
            this->invisible_dots_.pop();
            this->dots_[dot_num].visibility_ = true;
        }
        else
        {
            dot_num = this->visible_dots_.front();
            this->visible_dots_.pop();
        }

        this->visible_dots_.push(dot_num);
        this->dots_[dot_num].jump(x, y);
        //closest_dot_num_ = dot_num;

    }


}
