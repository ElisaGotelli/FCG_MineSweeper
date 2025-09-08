#include <SFML/Graphics.hpp>

//------------FINESTRA------------------
const char* window_title = "start background";
const unsigned window_width = 800;
const unsigned window_height = 600;
const float max_frame_rate = 60;

//schermata iniziale (start)
const sf::Vector2f start_size = {600.0, 400.0};
const unsigned int start_title_size = 36;


enum class Difficulty { None, Easy, Medium, Hard };
struct Start
{
    sf::Vector2f size;
    sf::Vector2f pos;
    sf::Texture texture;

    //font 
    sf::Font font{"risorse/EpundaSlab-VariableFont_wght.ttf"};
    sf::Text title{font};
    sf::Vector2f title_pos;
    sf::Text subtitle{font};
    sf::Text optEasy{font};
    sf::Text optMedium{font};
    sf::Text optHard{font};

    sf::FloatRect bEasy{}, bMedium{}, bHard{};

    // Scelta corrente (stub)
    Difficulty selected = Difficulty::None;

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

    title_pos = {start_px+ size.x / 2.f,start_py+ 20.f};

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

    // --- "Benvenuto su" ---
    title.setString("Benvenuto su");
    title.setCharacterSize(40);
    title.setFillColor(sf::Color::Black);
    title.setOutlineThickness(2.f);
    title.setOutlineColor(sf::Color::White);
    {
        sf::FloatRect b = title.getLocalBounds();
        title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        title.setPosition(title_pos);
    }
    window.draw(title);

    title.setString("MINESWEEPER"); 
    title.setCharacterSize(90);
    {
        sf::FloatRect b = title.getLocalBounds();
        title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        title.setPosition({title_pos.x, title_pos.y + 50.f});
    }
    window.draw(title);

    title.setString("versione E.0"); 
    title.setCharacterSize(30);
    {
        sf::FloatRect b = title.getLocalBounds();
        title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        title.setPosition({title_pos.x, title_pos.y + 140.f});
    }
    window.draw(title);

    subtitle.setString("Scegliere la modalita' di gioco:");
    subtitle.setCharacterSize(40);
    subtitle.setFillColor(sf::Color::Black);
    subtitle.setOutlineThickness(1.5f);
    subtitle.setOutlineColor(sf::Color::White);
    {
        const sf::FloatRect b = subtitle.getLocalBounds();
        subtitle.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        subtitle.setPosition({title_pos.x, title_pos.y + 250.f});
    }

    auto setupOption = [&](sf::Text& t, const char* label, unsigned sizePx){
        t.setString(label);
        t.setCharacterSize(sizePx);
        t.setFillColor(sf::Color::Black);
        t.setOutlineThickness(1.5f);
        t.setOutlineColor(sf::Color::White);
    };
    window.draw(subtitle);
    setupOption(optEasy,   "facile",  30);
    setupOption(optMedium, "medio",   30);
    setupOption(optHard,   "difficile", 30);

       // --- Opzioni su una riga, centrate nel pannello ---

    // spaziatura orizzontale
    float centerX = pos.x + size.x * 0.5f;
    float rowY    = subtitle.getPosition().y + 50.f; // sotto il sottotitolo
    float spacing = 140.f; // distanza tra le opzioni

    // facile (a sinistra)
    {
        const sf::FloatRect b = optEasy.getLocalBounds();
        optEasy.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        optEasy.setPosition({centerX - spacing, rowY});
        window.draw(optEasy);
        bEasy = optEasy.getGlobalBounds();
    }

    // medio (al centro)
    {
        const sf::FloatRect b = optMedium.getLocalBounds();
        optMedium.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        optMedium.setPosition({centerX, rowY});
        window.draw(optMedium);
        bMedium = optMedium.getGlobalBounds();
    }

    // difficile (a destra)
    {
        const sf::FloatRect b = optHard.getLocalBounds();
        optHard.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        optHard.setPosition({centerX + spacing, rowY});
        window.draw(optHard);
        bHard = optHard.getGlobalBounds();
    }
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

    sf::RectangleShape border; 
    border.setPosition({20.f, 20.f});
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(20.f);
    border.setOutlineColor(sf::Color(0, 100, 0));  
    border.setSize({(window_width - 40.f),(window_height-40.f)});

    State state;

    while (window.isOpen())
    {
        // events
        window.handleEvents (
                             [&window](const sf::Event::Closed&) { handle_close (window); },
                             [&window](const sf::Event::Resized& event) { handle_resize (event, window); }
        );

        // display
        window.clear(sf::Color(144, 238, 144));// verde chiaro di sfondo
        state.draw(window);
        window.draw(border); 
        window.display();

    }
}