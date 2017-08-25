#include "Application.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

Application::Application(const Config& config)
:   m_config      (config),
    m_window      ({config.windowWidth, config.windowHeight}, "Conway's Game of Life", sf::Style::Close),
    m_cellsGrid   (config),
    m_generation  (0)
{
    m_window.setKeyRepeatEnabled (false);
    m_window.setFramerateLimit   (m_config.FPS);

    m_cells.assign         (m_config.highestRow * m_config.highestCol, Cell::Dead);
    m_buttons.assign       (m_config.highestRow * m_config.highestCol, Button());

    // Constructing buttons and randomly making cell dead or alive
    srand(time(0));
    for(int col = 0; col < m_config.highestCol; col++)
    for(int row = 0; row < m_config.highestRow; row++)
    {
        m_cells     [getIndex(row, col)] =  rand() % 2 == 0 ? Cell::Dead : Cell::Alive;
        m_buttons   [getIndex(row, col)] =  Button(row * m_config.quadSize, col * m_config.quadSize, m_config.quadSize);
    }

    m_font.loadFromFile        ("../fonts/Lato-Black.ttf");
    m_text.setFont             (m_font);
    m_text.setFillColor        (sf::Color::White);
    m_text.setOutlineColor     (sf::Color::Black);
    m_text.setOutlineThickness (3);
    m_text.setCharacterSize    (20);
    m_text.setPosition         (5, 5);

    using namespace std::string_literals;
    m_text.setString           ("Click cell to change it to alive/ dead.\n"s +
                                "Press \"C\" to clear the cells\n"s +
                                "Press \"G\" to toggle the grid\n"s +
                                "Press \"I\" to show/hide this info and show/hide gen counter\n"s +
                                "Press \"Space\" to pause and be able to draw, and vice-versa.");

    // Copying those configs to m_genCounter
    m_genCounter = m_text;
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

            for(int col = 0; col < m_config.highestCol; col++)
            for(int row = 0; row < m_config.highestRow; row++)
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
            m_config.drawingCells = !m_config.drawingCells;
        }

        // Toggles grid
        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::G)
        {
            m_config.visibleGrid = !m_config.visibleGrid;
        }

        // Clear all the cells
        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::C)
        {
            m_config.drawingCells = true;
            std::fill(m_cells.begin(), m_cells.end(), Cell::Dead);
        }

        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::I)
        {
            m_config.showInfo = !m_config.showInfo;
        }

    }
}

void Application::update()
{
    // If i'm not drawing new cells on the board, run the logic.
    if(!m_config.drawingCells)
    {
        // Increase the generation counter
        m_generation++;
        m_genCounter.setString("Generation: " + std::to_string(m_generation));

        std::vector<Cell> newCells(m_config.highestRow * m_config.highestCol);

        for(int col = 0; col < m_config.highestCol; col++)
        for(int row = 0; row < m_config.highestRow; row++)
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
    m_window.clear(m_config.backgroundColor);

    // Drawing on the window

    m_window.draw(m_cellsGrid);

    if(m_config.visibleGrid)
        drawGrid();

   m_window.draw( m_config.showInfo ? m_text : m_genCounter);



    // Internally swapping buffers
    m_window.display();
}

void Application::drawGrid()
{
    for(int x = 0; x < m_config.windowWidth;  x+= m_config.quadSize)
    for(int y = 0; y < m_config.windowHeight; y+= m_config.quadSize)
    {
        sf::Vertex horizontalLines[] =
                {
                        sf::Vertex(sf::Vector2f(x, y)),
                        sf::Vertex(sf::Vector2f(x + m_config.windowWidth, y))
                };

        sf::Vertex verticalLines[] =
                {
                        sf::Vertex(sf::Vector2f(x, y)),
                        sf::Vertex(sf::Vector2f(x, y + m_config.windowHeight))
                };

        m_window.draw (horizontalLines, 2, sf::Lines);
        m_window.draw (verticalLines, 2, sf::Lines);
    }
}

int Application::getIndex(int row, int col) const
{
    return col * m_config.highestRow + row;
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
        if(col != m_config.highestCol - 1)
        {
            if(cells[getIndex(row - 1, col + 1)] == Cell::Alive) ++count;
        }
    }

    // If we are NOT in the rightmost side, we can always check RIGHT MIDDLE
    if(row != m_config.highestRow - 1)
    {
        if(cells[getIndex(row + 1, col)] == Cell::Alive) ++count;

        // AND if we are NOT at the uppermost side either, we can also check RIGHT TOP
        if(col != 0)
        {
            if(cells[getIndex(row + 1, col - 1)] == Cell::Alive) ++count;
        }

        // AND if we are NOT at the dowmost side either, we can also check RIGHT BOTTOM
        if(col != m_config.highestCol - 1)
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
    if(col != m_config.highestCol - 1)
    {
        if(cells[getIndex(row, col + 1)] == Cell::Alive) ++count;
    }

    return count;
}
