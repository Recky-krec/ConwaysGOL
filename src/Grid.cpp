#include "Grid.h"
#include "Application.h"

#include <iostream>

Grid::Grid(const Config& config)
:   CONFIG          (config),
    m_cells         (nullptr)
{
    // Setting all the grid squares
    for(int col = 0; col < CONFIG.highestCol; col++)
    for(int row = 0; row < CONFIG.highestRow; row++)
    {
        sf::RectangleShape     square;
        square.setPosition     (row * CONFIG.quadSize, col * CONFIG.quadSize);
        square.setSize         (sf::Vector2f(CONFIG.quadSize, CONFIG.quadSize));
        square.setFillColor    (CONFIG.cellColor);

        m_squares.push_back    (square);
    }

}

void Grid::update(std::vector<Cell>* cells)
{
    m_cells = cells;
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Only draws the cell if it's alive
    for(int i = 0; i < m_squares.size(); i++)
    {
        if( (*m_cells)[i] == Cell::Alive)
            target.draw(m_squares[i], states);
    }
}