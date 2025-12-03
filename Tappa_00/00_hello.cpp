#include <SFML/Graphics.hpp>

using namespace std; 

////////////////FINESTRA////////////////
const char* window_title = "Base Window"; //titolo della finestra 
const unsigned window_width = 800; //larghezza finestra 
const unsigned window_height = 600; //altezza finestra 
const float max_frame_rate = 60; //frame rate massimo della finestra 

////////////////EVENTI////////////////

//evento di chiusura della finestra 
void handle_close (sf::RenderWindow& window)
{
    window.close(); //comando per la chiusura della finestra 
}

//evento di ridimensionamento della finestra 
void handle_resize (const sf::Event::Resized& resized, sf::RenderWindow& window)
{
    float aspect = static_cast<float>(window_width)/static_cast<float>(window_height); //salvo in una variabile le proporzioni iniziali della finestra
    sf::Vector2u ws = resized.size; //recupero le nuove dimensioni richieste per la finestra dall'utente 
    float new_aspect = static_cast<float>(ws.x)/static_cast<float>(ws.y); //salvo in una variabile le nuove proporzioni della finestra 
   
    //Nel caso le nuove proporzioni siano minori delle originali
    if (new_aspect < aspect)
        ws = {ws.x,static_cast<unsigned>(ws.x/aspect)}; //modifico solamente l'altezza in modo che nonostante la diminuzione della grandezza della finestra rimanga sempre con le proporzioni corrette 
        else
        ws = {static_cast<unsigned>(ws.y*aspect),ws.y}; //modifico solamente la larghezza in modo che nonostante la diminuzione della grandezza della finestra rimanga sempre con le proporzioni corrette 

    window.setSize(ws); //comando per applicare le nuove dimensioni calcolate 
}

////////////////LOOP////////////////

int main()
{
    //creazione della finestra 
    sf::RenderWindow window (sf::VideoMode ({window_width, window_height}), window_title); 

    //impostazione del frame rate massimo
    window.setFramerateLimit (max_frame_rate);

    //impostare la grandezza minima della finestra 
    window.setMinimumSize(window.getSize());

    // ciclo che parte dal momento in cui la finestra è aperta 
    while (window.isOpen())
    {
        //gestione di tutti i possibili eventi 
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