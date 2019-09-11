class piece;

class square: public common_elements
{
public:
    square(sf::RenderWindow *window, Color color, int x, int y, sf::Texture &texture);
    square();
   ~square();

    int frame_;
    piece *piece_ptr_;
    
    void change_frame(int frame_num);
};


square::square(sf::RenderWindow *window, Color color, int x, int y, sf::Texture &texture):
    common_elements(window, color, x, y),
    frame_(0),
    piece_ptr_(nullptr)
    {        
        this->sprite_ = sf::Sprite(texture);
        this->sprite_.setTextureRect(sf::IntRect(0, color * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE));
        this->sprite_.setPosition(x * SQUARE_SIZE, y * SQUARE_SIZE);
    }


square::square():
    common_elements(),
    frame_(0),
    piece_ptr_(nullptr)
    {}

square::~square()
{}

void square::change_frame(int frame_num)
{
    frame_ = frame_num;
    this->sprite_.setTextureRect(sf::IntRect(frame_num * SQUARE_SIZE, this->color_ * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE));
}


class board
{
public:
    board(sf::RenderWindow *window, sf::Texture &texture);
   ~board();

    square squares_[8][8];

    void draw();
};

board::board(sf::RenderWindow *window, sf::Texture &texture)
{
    for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 8; ++j)
    {
        this->squares_[j][i] = square(window, (Color)((i + j) % 2), j, i, texture);
    }
}

board::~board()
{}

void board::draw()
{
    for (int i = 0; i < 8; ++i)
    for (int j = 0; j < 8; ++j)
    {
        this->squares_[i][j].draw();
    }
}