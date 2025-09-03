#include <SFML/Graphics.hpp>

//------------FINESTRA------------------
const char* window_title = "01 - start_background";
const unsigned window_width = 1200;
const unsigned window_height = 900;
const float max_frame_rate = 60;



//-------------griglia---------------
const float wall_horizontal_displacement = 50;
const float wall_vertical_displacement = 100;
const float wall_horizontal_size = 750;
const float wall_vertical_size = 150;
const unsigned int blocks_horizontal_num = 16;
const unsigned int blocks_vertical_num = 6;
sf::Color block_fill_color = sf::Color(220, 200, 20);
float block_outline_thickness = 2.0;
sf::Color block_outline_color = sf::Color(220, 140, 90);


//--------------EVENTI-----------------

void handle_close (sf::RenderWindow& window)
{
    window.close();
}
void handle_resize (const sf::Event::Resized& resized, sf::RenderWindow& window)
{
    float aspect = static_cast<float>(window_width)/static_cast<float>(window_height);
    sf::Vector2u ws = resized.size;
    float new_aspect = static_cast<float>(ws.x)/static_cast<float>(ws.y);
    if (new_aspect < aspect)
        ws = {ws.x,static_cast<unsigned>(ws.x/aspect)};
        else
        ws = {static_cast<unsigned>(ws.y*aspect),ws.y};
    window.setSize(ws);
}

//---------------------LOOP---------------------------

int main()
{
    sf::RenderWindow window (sf::VideoMode ({window_width, window_height}), window_title);
    window.setFramerateLimit (max_frame_rate);
    window.setMinimumSize(window.getSize());

    while (window.isOpen())
    {
        // events
        window.handleEvents (
                             [&window](const sf::Event::Closed&) { handle_close (window); },
                             [&window](const sf::Event::Resized& event) { handle_resize (event, window); }
        );

        // display
        window.clear (sf::Color::Black);
        window.display ();
    }
}