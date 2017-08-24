#include "Application.h"

#include <cstdlib>
#include <ctime>

Application::Application(const Config& config)
:   CONFIG        (config),
    m_window      ({config.windowWidth, config.windowHeight}, "Conway's Game of Life", sf::Style::Close),
    m_cellsGrid   (config)
{

    m_window.setKeyRepeatEnabled (false);
    m_window.setFramerateLimit   (300);

    m_cells.assign     (CONFIG.highestRow * CONFIG.highestCol, Cell::Dead);
    m_buttons.assign   (CONFIG.highestRow * CONFIG.highestCol, Button());


    // Constructing buttons and randomly making cell dead or alive
    srand(time(0));

    for(int col = 0; col < CONFIG.highestCol; col++)
    for(int row = 0; row < CONFIG.highestRow; row++)
    {
        m_cells     [getIndex(row, col)] =  rand() % 2 == 0 ? Cell::Dead : Cell::Alive;
        m_buttons   [getIndex(row, col)] =  Button(row * CONFIG.quadSize, col * CONFIG.quadSize, CONFIG.quadSize);
    }

}

void Application::run()
{
    while(m_window.isOpen())
    {
        handleInput();
        update();
        draw();
    }
}

void Application::handleInput()
{
    sf::Event event;
    while(m_window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
            m_window.close();

        if(event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2i mousePos = m_mouse.getPosition(m_window);

            for(int col = 0; col < CONFIG.highestCol; col++)
            for(int row = 0; row < CONFIG.highestRow; row++)
            {
                if(m_buttons[getIndex(row, col)].isUnderMouse(mousePos))
                {
                    Cell& cell = m_cells[getIndex(row, col)];
                    cell = cell == Cell::Alive ? Cell::Dead : Cell::Alive;
                }
            }
        }

        // VVV Keyboard keys assignment VVV

        // Toggles drawing cells state
        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
        {
            CONFIG.drawingCells = !CONFIG.drawingCells;
        }

        // Toggles grid
        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::G)
        {
            CONFIG.visibleGrid = !CONFIG.visibleGrid;
        }

        // Clear all the cells
        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::C)
        {
            std::fill(m_cells.begin(), m_cells.end(), Cell::Dead);
        }

    }
}

void Application::update()
{
    // If i'm not drawing new cells on the board, run the logic.
    if(!CONFIG.drawingCells)
    {
        std::vector<Cell> newCells(CONFIG.windowWidth * CONFIG.windowHeight);

        for(int col = 0; col < CONFIG.highestCol; col++)
        for(int row = 0; row < CONFIG.highestRow; row++)
        {
            Cell  cell      = m_cells  [getIndex(row, col)];
            Cell& newCell   = newCells [getIndex(row, col)];

            int count = aliveNeighborsCount(row, col, m_cells);

            switch(cell)
            {
                case Cell::Alive:
                    newCell = count < 2 || count > 3 ? Cell::Dead : Cell::Alive;
                    break;

                case Cell::Dead:
                    newCell = count == 3 ? Cell::Alive : Cell::Dead;
                    break;
            }
        }

        m_cells = std::move(newCells);
    }

    m_cellsGrid.update(&m_cells);
}

void Application::draw()
{
    // Clearing the window
    m_window.clear(CONFIG.backgroundColor);

    // Drawing on the window

    m_window.draw(m_cellsGrid);

    if(CONFIG.visibleGrid)
        drawGrid();

    // Internally swapping buffers
    m_window.display();
}

void Application::drawGrid()
{
    for(int x = 0; x < CONFIG.windowWidth;  x+= CONFIG.quadSize)
    for(int y = 0; y < CONFIG.windowHeight; y+= CONFIG.quadSize)
    {
        sf::Vertex horizontalLines[] =
                {
                        sf::Vertex(sf::Vector2f(x, y)),
                        sf::Vertex(sf::Vector2f(x + CONFIG.windowWidth, y))
                };

        sf::Vertex verticalLines[] =
                {
                        sf::Vertex(sf::Vector2f(x, y)),
                        sf::Vertex(sf::Vector2f(x, y + CONFIG.windowHeight))
                };

        m_window.draw (horizontalLines, 2, sf::Lines);
        m_window.draw (verticalLines, 2, sf::Lines);
    }
}

int Application::getIndex(int row, int col) const
{
    return col * CONFIG.highestRow + row;
}

int Application::aliveNeighborsCount(int row, int col, std::vector<Cell> cells) const
{
    int count = 0;

    // If we are NOT in the leftmost side, we can always check LEFT MIDDLE
    if(row != 0)
    {
        if(cells[getIndex(row - 1, col)] == Cell::Alive) ++count;

        // AND if we are NOT at the uppermost side either, we can also check LEFT TOP
        if(col != 0)
        {
            if(cells[getIndex(row - 1, col -1)] == Cell::Alive) ++count;
        }

        // AND if we are NOT at the dowmost side either, we can also check LEFT BOTTOM
        if(col != CONFIG.highestCol - 1)
        {
            if(cells[getIndex(row - 1, col + 1)] == Cell::Alive) ++count;
        }
    }

    // If we are NOT in the rightmost side, we can always check RIGHT MIDDLE
    if(row != CONFIG.highestRow - 1)
    {
        if(cells[getIndex(row + 1, col)] == Cell::Alive) ++count;

        // AND if we are NOT at the uppermost side either, we can also check RIGHT TOP
        if(col != 0)
        {
            if(cells[getIndex(row + 1, col - 1)] == Cell::Alive) ++count;
        }

        // AND if we are NOT at the dowmost side either, we can also check RIGHT BOTTOM
        if(col != CONFIG.highestCol - 1)
        {
            if(cells[getIndex(row + 1, col + 1)] == Cell::Alive) ++count;
        }
    }

    // If we are NOT in the uppermost side, we can always check TOP MIDDLE
    if(col != 0)
    {
        if(cells[getIndex(row, col - 1)] == Cell::Alive) ++count;
    }

    // If we are NOT in the downmost side, we can always check BOTTOM MIDDLE
    if(col != CONFIG.highestCol - 1)
    {
        if(cells[getIndex(row, col + 1)] == Cell::Alive) ++count;
    }

    return count;
}
