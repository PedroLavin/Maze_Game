
#pragma once
class game_elements {
protected:
    int x{};
    int y{};
public:
    game_elements(int x, int y) : x{ x }, y{ y } {}
    ~game_elements() {}
    virtual void info() = 0;
    virtual int get_x() = 0;
    virtual int get_y() = 0;


};

class player : public game_elements {
protected:
    int speed{};
public:
    player(int x, int y, int speed) : game_elements{ x, y }, speed{ speed } {}
    ~player() {}
    void info() { std::cout << "X: " << x << ", Y: " << y << std::endl; }
    void move_right() { x = x + speed; }
    void move_left() { x = x - speed; }
    void move_up() { y = y - speed; }
    void move_down() { y = y + speed; }
    int get_x() { return x; }
    int get_y() { return y; }
    void set_x(int new_x) { x = new_x; }
    void set_y(int new_y) { y = new_y; }
};


class bullet : public game_elements {
protected:
    int speed{};
    int original_x{};
    int original_y{};
    char dir;
public:
    bullet(int x, int y, char dir, int speed) : game_elements{ x, y }, dir{ dir }, speed{ speed }{
        original_x = x;
        original_y = y;
    }
    ~bullet() {}
    int get_x() { return x; }
    int get_y() { return y; }
    void info() { std::cout << x << y; }
    int get_range() { return pow(pow(original_x - x, 2), 0.5); }
    void shoot();
};

void bullet::shoot() {
    if (dir == 'l')
        x = x - speed;
    else if (dir == 'r')
        x = x + speed;
}


class key : public game_elements {
protected:
    static int number_of_keys;
    int is_found{};
public:
    key(int x, int y) : game_elements{ x, y } { number_of_keys++; }
    ~key() {
        number_of_keys--;
    }
    void info() { std::cout << x << y; }
    int get_x() { return x; }
    int get_y() { return y; }
    int get_number() { return number_of_keys; }
    int is_it_found() { return is_found; }
    void found() { is_found = 1; }
};

int key::number_of_keys{};


class enemy : public game_elements {
protected:
    int top_y{ 150 };
    int min_y{ 700 };
    int counter{};
    int direction{ 0 };
    int y{};
    int x{};

public:
    enemy(int dir_x, int dir_y) : game_elements{ dir_x, dir_y } {
        y = dir_y;
        x = dir_x;
    }
    ~enemy() {}
    void info() { std::cout << x << y; }
    int get_x() { return x; }
    int get_y() { return y; }
    void move();
};

void enemy::move() {
    if (y == top_y)
        direction = 0;
    else if (y == min_y)
        direction = 1;
    if (direction == 0)
        y = y + 2;
    else
        y = y - 2;
}