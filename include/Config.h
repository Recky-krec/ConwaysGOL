#pragma once
#include <SFML/Graphics/Color.hpp>

struct Config
{
    unsigned    windowWidth,
                windowHeight,
                quadSize,
                highestRow,
                highestCol,
                FPS;

    sf::Color   backgroundColor,
                cellColor;

    bool        visibleGrid,
                drawingCells,
                showInfo;

};

