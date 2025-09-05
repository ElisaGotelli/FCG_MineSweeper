#include <SFML/Graphics.hpp>
#include <vector>

//------------FINESTRA------------------
const char* window_title = "01 - cell(9x9)";
const unsigned window_width = 1200;
const unsigned window_height = 900;
const float max_frame_rate = 60;

//-------------GRIGLIA---------------
const float wall_horizontal_displacement = 100; 
const float wall_vertical_displacement = 100; 
sf::Vector2f block_num; 

//------------CELLA----------------

enum class Block_type{Mine,Empty, Number}; //texture per la cella 
enum class Block_state{ Covered, Revealed, Flag}; //stati per la cella 

struct Block
{
    sf::Vector2f pos; 
    float size; 
    sf::Texture texture; 
    Block_type type = Block_type::Empty; //default 
    int bomb_number = 0; //inidica il numero di bombe intorno alla cella
    Block_state state = Block_state::Covered; 

    Block (sf::Vector2f pos, float size); 

    void draw (sf::RenderWindow& window);
};

struct Grid
{
    std::vector<Block> blocks;
    sf::Vector2f block_num; 

    Grid (sf::Vector2f block_num);
    void draw (sf::RenderWindow& window);
};

struct State
{
    Grid grid; 

    State () : grid({9,9}) {}
    void draw (sf::RenderWindow& window);
};

//-----------CREAZIONE-----------

Block::Block (sf::Vector2f p, float s){
    pos = p; 
    size = s; 
    if(!texture.loadFromFile("Resources/pdf/cells/cellup.jpg")){ }
}

Grid::Grid (sf::Vector2f block_num){
    unsigned int Grid_size_y = (window_height - (wall_vertical_displacement * 2));

    unsigned int block_size = (Grid_size_y / block_num.y) * 0.85f;

    // dimensione totale della griglia in pixel
    float grid_width  = block_size * block_num.x;
    float grid_height = block_size * block_num.y;

    // calcolo offset: centrata verticalmente, allineata a destra orizzontalmente 
    float offset_x = window_width - grid_width - wall_horizontal_displacement;
    float offset_y = (window_height - grid_height) / 2.0f;

    for (unsigned hb = 0; hb < block_num.x; hb++) {        // colonne
        for (unsigned vb = 0; vb < block_num.y; vb++) {    // righe
            sf::Vector2f pos = {
                hb * block_size + offset_x,
                vb * block_size + offset_y
            };
            blocks.push_back(Block(pos, block_size)); 
        }
    }
}
//-------------DRAW------------------

void Block::draw (sf::RenderWindow& window)
{
    sf::RectangleShape b ({size,size}); 
    b.setTexture(&texture);
    b.setPosition(pos); 
    window.draw(b);

}

void Grid::draw (sf::RenderWindow& window)
{
    for (auto& block : blocks)
        block.draw (window);
}

void State::draw (sf::RenderWindow& window)
{
    grid.draw (window);
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