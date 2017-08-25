#include <iostream>

#include "Application.h"
#include "Config.h"


int main()
{
    Config config;

    char option;
    std::cout << "Would you like to use the default configuration? [Y/N]" << std::endl;
    std::cin >> option;
    option = std::toupper(option);

    int R,G,B;
    if (option == 'N')
    {
        std::cout << "Enter window width: ";
        std::cin >> config.windowWidth;

        std::cout << "Enter window height: ";
        std::cin >> config.windowHeight;

        std::cout << "Enter cell size: ";
        std::cin >> config.quadSize;

        std::cout << "Enter FPS limit: ";
        std::cin >> config.FPS;

        std::cout << "Would you like to configure the colors? [Y/N]" << std::endl;
        std::cin >> option;
        option = std::toupper(option);

        if (option == 'Y')
        {
            std::cout << "Enter RGB background color: ";
            std::cin >> R >> G >> B;
            config.backgroundColor = sf::Color(R,G,B);

            std::cout << "Enter RGB cell color: ";
            std::cin >> R >> G >> B;
            config.cellColor = sf::Color(R,G,B);
        }
        else
        {
            config.backgroundColor  = sf::Color(100, 100, 100, 255);
            config.cellColor        = sf::Color::Black;
        }

        // Make it so that it becomes divisible by the cell size and therefore fit the screen
        config.windowWidth  = config.windowWidth - config.windowWidth % config.quadSize;
        config.windowHeight = config.windowHeight - config.windowHeight % config.quadSize;
    }
    else
    {
        config.windowWidth      = 1024;
        config.windowHeight     = 768;
        config.quadSize         = 8;
        config.backgroundColor  = sf::Color(100, 100, 100, 255);
        config.cellColor        = sf::Color::Black;
        config.FPS              = 12;
    }

    // Those apply for both manual and default configs.
    config.visibleGrid      = false;
    config.drawingCells     = true;
    config.showInfo         = true;

    config.highestRow       = config.windowWidth  / config.quadSize;
    config.highestCol       = config.windowHeight / config.quadSize;



    Application app(config);

    try
    {
        app.run();
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}