#pragma once

#include <vector>
#include <cstdint>  //uint8_t

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Config.h"

enum class Cell : uint8_t;

class Grid : public sf::Drawable
{
public:
    explicit Grid   (const Config& config);
    void     update (std::vector<Cell>* cells);

private:
    void     draw   (sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::vector<sf::RectangleShape> m_squares;
    std::vector<Cell>*              m_cells;

    const Config                    CONFIG;
};
