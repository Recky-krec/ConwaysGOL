#include "Cell.h"

int Cell::Side = 16;
sf::Vector2i Cell::WindowSize = sf::Vector2i(640, 480);

Cell::Cell()
{
}

Cell::Cell(int row, int col, State state)
            : m_row         (row),
              m_col         (col),
              m_state       (state),
              m_highestRow  (WindowSize.x / Side),
              m_highestCol  (WindowSize.y / Side),
              m_vertices    (sf::Quads, 4)
    {
        int x = m_row * Side;
        int y = m_col * Side;

    m_vertices[0].position      = sf::Vector2f(x, y);                   // Top left
    m_vertices[1].position      = sf::Vector2f(x, y + Side);            // Bottom left
    m_vertices[2].position      = sf::Vector2f(x + Side, y + Side);     // Bottom right
    m_vertices[3].position      = sf::Vector2f(x + Side, y);            // Top right

    m_vertices[0].color         = sf::Color::Black;                     // Top left
    m_vertices[1].color         = sf::Color::Black;                     // Bottom left
    m_vertices[2].color         = sf::Color::Black;                     // Bottom right
    m_vertices[3].color         = sf::Color::Black;                     // Top right
}

void Cell::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // If the cell isn't alive don't bother draw it
    if(this->isAlive())
        target.draw(m_vertices, states);
}

int Cell::aliveNeighborsCount(Cell** grid)
{
    int count = 0;

    // If we are NOT in the leftmost side, we can always check LEFT MIDDLE
    if(m_row != 0)
    {
        if(grid[m_row - 1][m_col].isAlive()) ++count;

        // AND if we are NOT at the uppermost side either, we can also check LEFT TOP
        if(m_col != 0)
        {
            if(grid[m_row - 1][m_col - 1].isAlive()) ++count;
        }

        // AND if we are NOT at the dowmost side either, we can also check LEFT BOTTOM
        if(m_col != m_highestCol)
        {
            if(grid[m_row - 1][m_col + 1].isAlive()) ++count;
        }
    }

    // If we are NOT in the rightmost side, we can always check RIGHT MIDDLE
    if(m_row != m_highestRow)
    {
        if(grid[m_row + 1][m_col].isAlive()) ++count;

        // AND if we are NOT at the uppermost side either, we can also check RIGHT TOP
        if(m_col != 0)
        {
            if(grid[m_row + 1][m_col - 1].isAlive()) ++count;
        }

        // AND if we are NOT at the dowmost side either, we can also check RIGHT BOTTOM
        if(m_col != m_highestCol)
        {
            if(grid[m_row + 1][m_col + 1].isAlive()) ++count;
        }
    }

    // If we are NOT in the uppermost side, we can always check TOP MIDDLE
    if(m_col != 0)
    {
        if(grid[m_row][m_col - 1].isAlive()) ++count;
    }

    // If we are NOT in the downmost side, we can always check BOTTOM MIDDLE
    if(m_col != m_highestCol)
    {
        if(grid[m_row][m_col + 1].isAlive()) ++count;
    }

    return count;
}


void Cell::revive()
{
    m_state = ALIVE;
}

void Cell::kill()
{
    m_state = DEAD;
}
