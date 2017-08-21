#include "Application.h"

Application::Application()
: m_width       (640),
  m_height      (480),
  m_drawing     (true),
  m_highest_row (m_width  / Cell::Side),
  m_highest_col (m_height / Cell::Side),
  m_window      (sf::VideoMode(640, 480), "Conway's Game of Life")
{
    srand(time(0));
    m_window.setFramerateLimit(10);


    // Allocating memory for two dimensional array (m_grid)
    m_grid = new Cell* [m_highest_row];
    for(int i = 0; i < m_highest_row + 1; i++)
    {
        m_grid[i] = new Cell[m_highest_col + 1];
    }

    // Constructing grid
    for(int row = 0; row < m_highest_row; row++)
    {
        for (int col = 0; col < m_highest_col; col++)
            m_grid[row][col] = Cell(row, col, Cell::DEAD);//(rand()%2) == 0 ? Cell::DEAD : Cell::ALIVE);
    }

    // Allocating memory for two dimensional array (m_buttons)
    m_buttons = new Button* [m_highest_row];
    for(int i = 0; i <= m_highest_row; i++)
    {
        m_buttons[i] = new Button[m_highest_col];
    }

    // Constructing buttons
    for(int row = 0; row < m_highest_row; row++)
    {
        for (int col = 0; col < m_highest_col; col++)
            m_buttons[row][col] = Button(row * Cell::Side, col * Cell::Side, Cell::Side);
    }


}

Application::~Application()
{
    // Dealocate memory
    for (int i=0; i<m_highest_row; i++)
        delete [] m_grid[i];
    delete [] m_grid;

    for (int i=0; i<m_highest_row; i++)
        delete [] m_buttons[i];
    delete [] m_buttons;
}

void Application::run()
{
    sf::Clock clock;
    sf::Time deltaTime = sf::Time::Zero;

    while(m_window.isOpen())
    {
        deltaTime = clock.restart();
        handleInput();
        update(deltaTime);
        draw();
    }
}

void Application::handleInput()
{
    sf::Event event;
    while(m_window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                m_window.close();
                break;

            case sf::Event::MouseButtonPressed:
            {
                sf::Vector2i mousePos = m_mouse.getPosition(m_window);

                for(int row = 0; row < m_highest_row; row++)
                {
                    for (int col = 0; col < m_highest_col; col++)
                    {
                        if(m_buttons[row][col].isUnderMouse(mousePos))
                        {
                            m_grid[row][col].revive();
                            std::cout << "ROW: " << row << " COL: "<< col << std::endl;
                            std::cout << m_grid[row][col].aliveNeighborsCount(m_grid) << std::endl;
                        }
                    }
                }
                break;
            }
        }

        switch (event.key.code)
        {
            case(sf::Keyboard::Z):
                m_drawing = true;
                break;

            case(sf::Keyboard::X):
                m_drawing = false;
                break;
        }

    }
}


void Application::update(sf::Time dt)
{

    if(!m_drawing)
    {
        for(int row = 0; row < m_highest_row; row++)
        {
            for(int col = 0; col < m_highest_col; col++)
            {
                Cell* cell = &m_grid[row][col];


                if(cell->isAlive())
                {
                    if( cell->aliveNeighborsCount(m_grid) < 2 || cell->aliveNeighborsCount(m_grid) > 3)
                    {
                        cell->kill();
                    }


                    if((cell->aliveNeighborsCount(m_grid) == 2 || cell->aliveNeighborsCount(m_grid) == 3))
                    {
                        cell->revive();
                    }

                }

                if(cell->isDeath())
                {
                    if(cell->aliveNeighborsCount(m_grid) == 3)
                        cell->revive();
                }


            }

        }

    }

}

void Application::draw()
{
    m_window.clear(sf::Color(100, 100, 100));


    for(int row = 0; row < m_highest_row; row++)
    {
        for(int col = 0; col < m_highest_col; col++)
            m_window.draw(m_grid[row][col]);
    }

    drawGrid();

    m_window.display();
}

void Application::drawGrid()
{
    for(int x = 0; x<m_width; x+=Cell::Side)
    {
        for(int y = 0; y<m_height; y+=Cell::Side)
        {
            sf::Vertex line[] =
                    {
                            sf::Vertex(sf::Vector2f(x, y)),
                            sf::Vertex(sf::Vector2f(x+m_width, y))
                    };

            sf::Vertex line2[] =
                    {
                            sf::Vertex(sf::Vector2f(x, y)),
                            sf::Vertex(sf::Vector2f(x, y+m_height))
                    };

            m_window.draw(line, 2, sf::Lines);
            m_window.draw(line2, 2, sf::Lines);
        }
    }
}