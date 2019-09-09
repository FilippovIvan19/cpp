class game_manager
{
public:
    game_manager();
   ~game_manager();

    std::vector<common_elements*> game_objects_;

    void move(float dt);
    void draw();
    void add_obj(common_elements *obj_ptr);
};

game_manager::game_manager():
game_objects_(std::vector<common_elements*> ())
{}

game_manager::~game_manager()
{}

void game_manager::add_obj(common_elements *obj_ptr)
{
    this->game_objects_.push_back(obj_ptr);
}

void game_manager::move(float dt)
{
    int count = this->game_objects_.size();
    for (int i = 0; i < count; ++i)
        this->game_objects_[i]->move(dt);
}

void game_manager::draw()
{
    int count = this->game_objects_.size();
    for (int i = 0; i < count; ++i)
        this->game_objects_[i]->draw();
}
