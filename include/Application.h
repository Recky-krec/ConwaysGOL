#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>
#include <cstdint>  // uint8_t

#include "Button.h"
#include "Grid.h"
#include "Config.h"

enum class Cell : uint8_t
{
    Dead,
    Alive
};


class Application
{
public:
    explicit Application     (const Config& config);
    void run                 ();

private:
    void handleInput         ();
    void update              ();
    void draw                ();

    // 1. Returns the index of a 1D array from a (row/col) like 2D array
    // 2. Counts the alive neighbors for a cell in (row/col) and returns it
    // 3. Draws a white square grid on the screen
    int  getIndex            (int row, int col) const;
    int  aliveNeighborsCount (int row, int col, std::vector<Cell> cells) const;
    void drawGrid            ();

private:
    sf::RenderWindow         m_window;
    sf::Mouse                m_mouse;

    std::vector<Cell>        m_cells;       // mgiht be
    std::vector<Button>      m_buttons;     // dought it is

    Grid                     m_cellsGrid;   // might be
    Config                   CONFIG;
};