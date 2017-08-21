#pragma once

#include <SFML/Graphics.hpp>

class Button
{
public:
    Button();
    Button(int x, int y, int side);

    bool isUnderMouse(const sf::Vector2i& mousePos);

private:
    sf::Vector2i m_position;
    sf::Vector2i m_size;
};