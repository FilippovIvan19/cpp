#include <set>


class piece: public common_elements
{
public:
    piece(sf::RenderWindow *window, Color color, int x, int y, sf::Texture &texture, board &chessboard, Kind kind);
   ~piece() {};

    Kind kind_;

    virtual void available_squares(board &chessboard, std::set<std::pair<int, int>> &squares) = 0;
    void relocate(int x, int y, board &chessboard);
};

piece::piece(sf::RenderWindow *window, Color color, int x, int y, sf::Texture &texture, board &chessboard, Kind kind):
    common_elements(window, color, x, y),
    kind_(kind)
    {
        this->sprite_ = sf::Sprite(texture);
        this->sprite_.setTextureRect(sf::IntRect(this->kind_ * PIC_W, this->color_ * PIC_H, PIC_W, PIC_H));
        this->sprite_.setScale
            (SQUARE_SIZE / this->sprite_.getGlobalBounds().height,
             SQUARE_SIZE / this->sprite_.getGlobalBounds().height);
        this->sprite_.setPosition(x * SQUARE_SIZE, y * SQUARE_SIZE);
        chessboard.squares_[x][y].piece_ptr_ = this;
    }

void piece::relocate(int x, int y, board &chessboard)
{
    chessboard.squares_[this->x_][this->y_].piece_ptr_ = nullptr;
    this->x_ = x;
    this->y_ = y;
    this->sprite_.setPosition(x * SQUARE_SIZE, y * SQUARE_SIZE);
    chessboard.squares_[x][y].piece_ptr_ = this;
}


#include "many_classes.hpp"
#include "moving.hpp"