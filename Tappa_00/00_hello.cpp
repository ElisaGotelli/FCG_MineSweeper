#include <SFML/Graphics.hpp>

////////////////FINESTRA////////////////
const char* window_title = "00 - Base Window"; //titolo della finestra 
const unsigned window_width = 1200; //larghezza finestra 
const unsigned window_height = 900; //altezza finestra 
const float max_frame_rate = 60; //frame rate massimo della finestra 

////////////////EVENTI////////////////

//chiusura della finestra 
void handle_close (sf::RenderWindow& window)
{
    window.close();
}

//ridimensionamento della finestra 
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

////////////////LOOP////////////////

int main()
{
    //creazione della finestra 
    sf::RenderWindow window (sf::VideoMode ({window_width, window_height}), window_title);

    //impostazione del frame limit 
    window.setFramerateLimit (max_frame_rate);

    //impostare la grandezza minima della finestra 
    window.setMinimumSize(window.getSize());

    //dal momento in cui la finestra è aperta 
    while (window.isOpen())
    {
        //possibili eventi
        window.handleEvents (
                             [&window](const sf::Event::Closed&) { handle_close (window); },
                             [&window](const sf::Event::Resized& event) { handle_resize (event, window); }
        );

        //pulire la finestra e mettere come colore di sfondo il nero 
        window.clear (sf::Color::Black);

        //mostrare tutto a schermo
        window.display ();
    }
}