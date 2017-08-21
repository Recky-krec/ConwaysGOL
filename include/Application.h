#pragma once

#include <vector>
#include <cstdlib>

#include <SFML/Graphics.hpp>
#include "Cell.h"
#include "Button.h"

class Application
{
public:
    Application();
    ~Application();
    void run();

private:
    void handleInput();
    void update(sf::Time dt);

    void draw();
    void drawGrid();

private:
    sf::RenderWindow m_window;
    sf::Mouse m_mouse;

    int m_width, m_height;
    int m_highest_row, m_highest_col;

    bool m_drawing;

    Cell** m_grid;
    Button** m_buttons;
};