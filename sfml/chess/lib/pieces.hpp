#include <set>


class piece: public common_elements
{
public:
    piece(sf::RenderWindow *window, Color color, int x, int y, sf::Texture &texture, Kind kind);
   ~piece() {};

    Kind kind_;

    //virtual void move(int x, int y);
    void relocate(int x, int y);
};

piece::piece(sf::RenderWindow *window, Color color, int x, int y, sf::Texture &texture, Kind kind):
    common_elements(window, color, x, y),
    kind_(kind)
    {
        this->sprite_ = sf::Sprite(texture);
        this->sprite_.setTextureRect(sf::IntRect(this->kind_ * PIC_W, this->color_ * PIC_H, PIC_W, PIC_H));
        this->sprite_.setScale
            (SQUARE_SIZE / this->sprite_.getGlobalBounds().height,
             SQUARE_SIZE / this->sprite_.getGlobalBounds().height);
        this->sprite_.setPosition(x * SQUARE_SIZE, y * SQUARE_SIZE);
    }

void piece::relocate(int x, int y)
{
    this->x_ = x;
    this->y_ = y;
    this->sprite_.setPosition(x * SQUARE_SIZE, y * SQUARE_SIZE);
}


class pawn: public piece
{
public:
    pawn(sf::RenderWindow *window, Color color, int x, int y, sf::Texture &texture);
   ~pawn() {};
};

pawn::pawn(sf::RenderWindow *window, Color color, int x, int y, sf::Texture &texture):
    piece(window, color, x, y, texture, PAWN)
    {
        // this->sprite_.setTextureRect(sf::IntRect(this->kind_ * PIC_W, this->color_ * PIC_H, PIC_W, PIC_H));
        // this->sprite_.setScale
        //     (SQUARE_SIZE / this->sprite_.getGlobalBounds().height,
        //      SQUARE_SIZE / this->sprite_.getGlobalBounds().height);
    }
