class game_manager
{
public:
    game_manager();
   ~game_manager();

    std::set<piece*> game_objects_;

    void draw();
    void add_obj(piece *obj_ptr);
    void clear();
};

game_manager::game_manager():
game_objects_(std::set<piece*> ())
{}

game_manager::~game_manager()
{}

void game_manager::add_obj(piece *obj_ptr)
{
    this->game_objects_.insert(obj_ptr);
}

void game_manager::draw()
{
    for (piece *obj: this->game_objects_)
        obj->draw();
}

void game_manager::clear()
{
    for (piece *obj: this->game_objects_)
        delete(obj);

    // int count = this->game_objects_.size();
    // for (int i = 0; i < count; ++i)
    //     delete(this->game_objects_[i]);
}
