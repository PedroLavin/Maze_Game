// Laby.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
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
#include "game_elements.h"
#include "labyrinth.h"
#include "game.h"

int main()
{
    game g = game();
    g.draw();
    return 0;
}
