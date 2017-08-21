#include "Button.h"

Button::Button()
{
}

Button::Button(int x, int y, int side)
{
    m_position = sf::Vector2i(x, y);
    m_size = sf::Vector2i(side, side);
}


bool Button::isUnderMouse(const sf::Vector2i& mousePos)
{

    if(mousePos.x > m_position.x && mousePos.x < m_position.x + m_size.x)
    {
        if (mousePos.y > m_position.y && mousePos.y < m_position.y + m_size.y)
            return true;
    }

    return false;

}

