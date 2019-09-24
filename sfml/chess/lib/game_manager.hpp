class game_manager
{
public:
    game_manager();
   ~game_manager();

    std::set<piece*> game_objects_;
    king *white_king_;
    king *black_king_;
    piece *jumped_pawn_;

    void draw();
    void add_obj(piece *obj_ptr);
    void update_available_squares(board &chessboard);
};

game_manager::game_manager():
    game_objects_(std::set<piece*> ()),
    jumped_pawn_(nullptr),
    white_king_(nullptr),
    black_king_(nullptr)
    {}

void game_manager::add_obj(piece *obj_ptr)
{
    this->game_objects_.insert(obj_ptr);
    if (obj_ptr->kind_ == KING)
        if (obj_ptr->color_ == WHITE)
            white_king_ = dynamic_cast<king*>(obj_ptr);
        else
            black_king_ = dynamic_cast<king*>(obj_ptr);
}

void game_manager::draw()
{
    for (piece *obj: this->game_objects_)
        obj->draw();
}

void game_manager::update_available_squares(board &chessboard)
{
    for (piece *obj: this->game_objects_)
        obj->update_available_squares(*this, chessboard);
}

game_manager::~game_manager()
{
    for (piece *obj: this->game_objects_)
        delete(obj);
}
