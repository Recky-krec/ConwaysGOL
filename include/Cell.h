#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>


class Cell : public sf::Drawable
{
public:
    enum State
    {
        DEAD    = 0,
        ALIVE   = 1
    };

public:
    static int Side;
    static sf::Vector2i WindowSize;

public:
    Cell();
    Cell(int row, int col, State state);

    // Modifiers
    void revive();
    void kill();

    // Getters
    inline bool isAlive() const { return (bool)m_state; }
    inline bool isDeath() const { return !isAlive(); }

    // Rules
    int  aliveNeighborsCount(Cell** grid);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    int m_row;
    int m_col;
    int m_highestRow;
    int m_highestCol;

    State m_state;
    sf::VertexArray m_vertices;
};