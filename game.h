#pragma once

#include <iostream>
#include <math.h>
#include "CImg.h"
#include <vector>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
//#include "game_elements.cpp"


#define PI 3.14159265

unsigned int h = 800;
unsigned int w = 1600;
unsigned width_begin = w / 5;
unsigned int width_end{ 4 * w / 5 };
unsigned int height_begin{ h / 4 };
unsigned int height_end{ 3 * h / 4 };
unsigned int thickness{ 8 };
using namespace cimg_library;
const unsigned char green[] = { 0, 0, 0 }; //Outside
const unsigned char brown[] = { 28, 63, 110 };
const unsigned char light_brown[] = { 0, 0, 153 };  // Inside, 
const unsigned char orange[] = { 4, 255, 247 };  //Walls
const unsigned char pinky[] = { 255, 0, 127 };  //PLayer
const unsigned char yellow[] = { 255, 255, 0 };  //Key
const unsigned char dark[] = { 255, 129, 3 };  //Enemy

const unsigned char bluegreen[] = { 0, 170, 255 };
const unsigned char white[] = { 255, 255, 255 };
const unsigned char black[] = { 0, 0, 0 };



namespace text_output
{
    template < typename T > std::string stringer(const T& integer)
    {
        std::ostringstream ostring;
        ostring << integer;
        return ostring.str();
    }
}


template <class element1> int collides(std::shared_ptr<element1> a, std::shared_ptr<element1> b, double length) {
    double range{ pow(pow(a->get_x() - b->get_x(), 2) + pow(a->get_y() - b->get_y(), 2), 0.5) };
    if (range < length)
        return 0;
    else
        return 1;
}



class game {
protected:
    char up;
    char down;
    char right;
    char left;
    bool quit;
    std::shared_ptr<player> player_1;
    CImg <unsigned char> bg;
    std::shared_ptr<labyrinth> laby;
    std::vector<std::shared_ptr<game_elements>> enemies;


public:
    game();
    void game_over(CImg <unsigned char> img) { bg.draw_rectangle(0, 0, 1800, 1800, "blue"); }
    std::string stringer(std::string string, int variable);
    void draw();
};
game::game() {
    quit = false;
    up = 'w';
    right = 'd';
    left = 'a';
    down = 's';
    player_1 = std::shared_ptr<player>(new player(725, 159, 1.5));
    bg = CImg <unsigned char>{ w, h, 1, 3, 255 };
    laby = std::shared_ptr<labyrinth>(new labyrinth( 280, 150, 6));
}

std::string game::stringer(std::string string, int variable) {
    std::string string_1{ text_output::stringer(variable) };
    std::string string_2{ string + string_1 };

    return string_2;
}

void game::draw() {

    CImgDisplay dsp(w, h, "MAZE", 0);
    CImg<unsigned char> img{ bg };

    //The walls are constructed
    laby->make_walls();

    //The enemy and bullets vector are defined
    std::vector <std::shared_ptr<bullet>> bullets;
    std::vector<std::shared_ptr<enemy>> enemies;

    //The dimensions of the maze are initialized
    int x_dim{ 280 };
    int y_dim{ 150 };
    int size{ 6 };

    int counter{ 0 };
    double radius{ 5 };
    int player_x{ };
    int player_y{};
    const unsigned char* wall_color = orange;
    int breaky{ 0 };
    int win_or_lose{ 0 };
    int num_keys{ laby->get_keys()[0]->get_number() };


    //The background colors are set up
    bg.draw_rectangle(0, 0, w, h, green);
    bg.draw_rectangle(x_dim, y_dim, x_dim + laby->get_vector()[0].size() * size, y_dim + laby->get_vector().size() * size, light_brown);

    //The labyrinth is drawn using the map of the labyrinth class
    for (int j{}; j < laby->get_vector().size(); j++) {
        for (int i{}; i < laby->get_vector()[0].size(); i++) {
            if (laby->get_vector()[j][i] == 1)
                bg.draw_rectangle(x_dim + (size)*i, y_dim + (size)*j, x_dim + size * (1 + i), y_dim + size * (1 + j), orange);
        }
    }
    dsp.display(bg);



    //The two enemies are created
    std::shared_ptr<enemy> enemy1(new enemy(180, 150));
    std::shared_ptr<enemy> enemy2(new enemy(1200, 680));
    enemies.push_back(enemy1);
    enemies.push_back(enemy2);


    while (!dsp.is_closed()) {
        img = bg;


        //Check if the game has finished

        if (breaky == 1)
            break;

        // Get the player's updated position
        player_x = player_1->get_x();
        player_y = player_1->get_y();



        //Draw the player in its current position//
        img.draw_rectangle(player_x, player_y, player_x + radius, player_y + radius, pinky);

        counter++;


        // Iterate through the vector of enemies and add bullets if the counter is in the required number//
        for (int e{}; e < enemies.size(); e++) {
            img.draw_circle(enemies[e]->get_x(), enemies[e]->get_y(), 7, dark);
            enemies[e]->move();
            if (counter % 90 == 0) {
                if (e == 0) {
                    std::shared_ptr<bullet> bu(new bullet(enemies[e]->get_x(), enemies[e]->get_y(), 'r', 2));
                    bullets.push_back(bu);
                }
                else if (e == 1) {
                    std::shared_ptr<bullet> bu(new bullet(enemies[e]->get_x(), enemies[e]->get_y(), 'l', 2));
                    bullets.push_back(bu);
                }
            }
        }

        // Iterate over the vector of bullets to draw them and iterate their position in their respective direction//
        // Also, for every bullet, it is checked whether it has collided with the player, in which case the game ends//
        for (int b{}; b < bullets.size(); b++) {
            img.draw_circle(bullets[b]->get_x(), bullets[b]->get_y(), 6, dark);
            bullets[b]->shoot();
            if (bullets[b]->get_range() > 1000)
                bullets.erase(bullets.begin());
            if (!collides<game_elements>(player_1, bullets[b], 8))
                breaky = 1;
        }


        // The door is drawn, in a different colour if all the keys have been found//
        img.draw_rectangle(719, 678, 735, 684, wall_color);



        // Take input from the user in the form of a key from the keyboard and update the position of the player consequently//
        if (dsp.is_keyD() == 1)
            if (laby->can_pass(player_x - size + 2 * radius, player_y, 'r') == 0 || laby->can_pass(player_x - size + 2 * radius, player_y - size + 2 * radius, 'r') == 0)
                player_x = player_x;
            else
                player_1->move_right();
        if (dsp.is_keyW() == 1)
            if (laby->can_pass(player_x, player_y + size - radius, 'u') == 0 || laby->can_pass(player_x - size + radius, player_y + size - radius, 'u') == 0)
                player_y = player_y;
            else
                player_1->move_up();
        if (dsp.is_keyA() == 1)
            if ((laby->can_pass(player_x + size - radius, player_y, 'l') == 0) || laby->can_pass(player_x + size - radius, player_y + radius, 'l') == 0)
                player_x = player_x;
            else
                player_1->move_left();
        if (dsp.is_keyS() == 1)
            if (laby->can_pass(player_x, player_y - size + 2 * radius, 'd') == 0 || laby->can_pass(player_x - size + 2 * radius, player_y - size + 2 * radius, 'd') == 0)
                player_y = player_y;
            else
                player_1->move_down();



        // Iterate through the vector of keys to draw them and check whether the player has found it, in which case it will not be 
        // drawn again.//

        for (int k{}; k < laby->get_keys().size(); k++) {
            if (laby->get_keys()[k]->is_it_found() == 0)
                img.draw_circle(laby->get_keys()[k]->get_x(), laby->get_keys()[k]->get_y(), 5, yellow);
            if (!collides<game_elements>(player_1, laby->get_keys()[k], 8) && laby->get_keys()[k]->is_it_found() == 0) {
                laby->get_keys()[k]->found();
                num_keys--;
            }
            if (num_keys == 0)
                wall_color = brown;
        }


        // Check if the player has reached the door, and that the keys have all been found//
        if (player_x > 719 && player_x < 735 && player_y > 665 && num_keys == 0) {
            win_or_lose = 1;
            breaky = 1;
        }


        // Write the number of keys left for the player to reach
        const char* text;
        std::string string;
        string = stringer("Number of Keys Left: ", num_keys);
        text = string.c_str();
        img.draw_text(250, 90, text, white, black, 1, 25);
        if (num_keys == 0) {
            img.draw_text(550, 70, "The Doors Are Open!", orange, black, 1, 50);
        }
        dsp.display(img);
    }
    while (!dsp.is_closed()) {
        //If the player loses, a window pops up with the game over message//
        if (win_or_lose == 0) {
            img.draw_rectangle(0, 0, 1700, 1700, brown);
            const char* text;
            text = "GAME OVER\n Better luck \n  next time";
            img.draw_text(550, 300, text, "red", brown, 100, 100);
            dsp.display(img);
        }
        else {
            // In case the player wins, a victory window will appear
            img.draw_rectangle(0, 0, 1700, 1700, light_brown);
            const char* text;
            text = "Congratulations, you \n       have beaten \n      the maze";
            img.draw_text(250, 300, text, orange, light_brown, 100, 100);
            dsp.display(img);
        }
    }
}
