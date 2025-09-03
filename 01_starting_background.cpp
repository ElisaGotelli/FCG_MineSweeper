#include <SFML/Graphics.hpp>

//------------FINESTRA------------------
const char* window_title = "01 - start_background";
const unsigned window_width = 800;
const unsigned window_height = 600;
const float max_frame_rate = 60;

//start size 
const sf::Vector2f start_size = {600.0, 400.0};

struct Start
{
    sf::Vector2f size;
    sf::Vector2f pos;
    sf::Texture texture;

    Start ();
    void draw (sf::RenderWindow& window);
};

struct State
{
    Start start; 

    State () {}
    void draw (sf::RenderWindow& window);
};

Start::Start ()
{
    size = start_size; 
    float start_px = (window_width  / 2.f) - (start_size.x / 2.f);
    float start_py = (window_height / 2.f) - (start_size.y / 2.f);
    pos = {start_px, start_py};
}

//--------DRAW---------------
void Start::draw (sf::RenderWindow& window)
{
    sf::RectangleShape s (size);
    s.setPosition (pos);
    s.setFillColor(sf::Color(210, 180, 140));
    s.setOutlineThickness(20.f);
    s.setOutlineColor(sf::Color(92, 51, 23));
    window.draw (s);
}

void State::draw (sf::RenderWindow& window)
{
    start.draw (window);
}

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

    State state;

    while (window.isOpen())
    {
        // events
        window.handleEvents (
                             [&window](const sf::Event::Closed&) { handle_close (window); },
                             [&window](const sf::Event::Resized& event) { handle_resize (event, window); }
        );

        // display
        window.clear(sf::Color(144, 238, 144)); // verde chiaro di sfondo
        state.draw(window);
        window.display();

    }
}