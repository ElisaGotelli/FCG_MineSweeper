#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

////////////////FINESTRA////////////////
const char* window_title = "01_cell_starting_texture";
const unsigned window_width = 1200;
const unsigned window_height = 900;
const float max_frame_rate = 60;

////////////////GRIGLIA////////////////

//distanza minima dai bordi della finestra 
const float wall_horizontal_displacement = 100; //distanza dal bordo destro 
const float wall_vertical_displacement = 100; //distanza dall'alto e dal basso 


////////////////BLOCCO////////////////

enum class Block_type{Mine,Empty, Number}; //stati nascosti possibili della cella 
enum class Block_state{ Covered, Revealed, Flag}; //stati visibili possibili della cella 
sf::Texture Covered_texture("risorse/pdf/cells/cellup.jpg"); //texture iniziale per ogni blocco 

////////////////STRUCT////////////////
struct Block
{
    sf::Vector2f pos; //posizione del blocco nella finestra 
    float size; //dimensione del blocco (diversa in base al numero di blocchi)
    sf::Texture texture;//texture del blocco (per ora sara' solo Covered)
    Block_type type; //texture nascosta del blocco
    int bomb_number; //numero di bombe intorno alla cella
    Block_state state; //stato iniziale della cella (sempre covered)

    //creazione del blocco di default (quindi con la texture e stato Covered)
    Block (sf::Vector2f pos, float size) : pos (pos),
                                                  size (size),
                                                  texture (Covered_texture),
                                                  state(Block_state::Covered) {}
    void draw (sf::RenderWindow& window);
};

struct Grid
{
    std::vector<Block> blocks; //vettore dei vari blocchi che comporranno la griglia 
    sf::Vector2i block_num; //numero di blocchi in griglia 
    sf::Vector2f Grid_size;

    Grid (sf::Vector2i block_num); //metodo per creazione della griglia con impostato il numero di blocchi 
    void draw (sf::RenderWindow& window);
};

struct State //stato generale della finestra 
{
    Grid grid; 

    State () : grid({9,9}) {} //per ora vi potrà solo essere disponibile una griglia 9x9 
    void draw (sf::RenderWindow& window);
};

////////////////CREAZIONE////////////////

Grid::Grid (sf::Vector2i bs){
    //imposto il numero di blocchi 
    block_num = bs; 

    //ho deciso di usare solo la y poichè è la parte della finestra più corta e di ridurli leggeremnet (del 15%)
    float block_size = ((window_height - (wall_vertical_displacement * 2)) / block_num.y) * 0.85f;

    //calcolo della grandezza della griglia 
    Grid_size = {block_size * block_num.x, block_size * block_num.y};

    //per ogni colonna della griglia 
    for (unsigned hb = 0; hb < block_num.x; hb++) {
        //per ogni blocco in detta colonna considerata (per ogni riga della griglia)
        for (unsigned vb = 0; vb < block_num.y; vb++) {
            //calcolo la posizione del blocco considerato 
            sf::Vector2f pos = {
                hb * block_size + (window_width - Grid_size.x - wall_horizontal_displacement),
                vb * block_size + ((window_height - Grid_size.y) / 2.0f)
            };

            //inserisco il blocco nel vettore rappresentante la griglia 
            blocks.push_back(Block(pos, block_size)); 
        }
    }
}
////////////////DRAW////////////////

void Block::draw (sf::RenderWindow& window)
{
    sf::RectangleShape b ({size,size}); //il blocco è un quadrato (quindi in rettangolo con altezza=larghezza)
    b.setTexture(&texture); //imposta la texture del blocco 
    b.setPosition(pos); //metti il blocco nella posizione corretta 
    window.draw(b); //disegna il blocco nella finestra 

}

void Grid::draw (sf::RenderWindow& window)
{
    for (auto& block : blocks) //disegna ogni blocco (uno alla volta)
        block.draw (window);
}

void State::draw (sf::RenderWindow& window)
{
    grid.draw (window); //disegna la griglia
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

    //creazione di un bordo per la finestra 
    sf::RectangleShape border({(window_width - 40.f),(window_height-40.f)}); //vengono lasciati 40 pixel dal bordo della finestra in modo da rendere visibile il bordo 
    border.setPosition({20.f, 20.f}); //viene messo leggermente più sotto dell'origine per lo stesso motivo spiegato precedentemente
    border.setFillColor(sf::Color::Transparent); //deve essere solo un bordo quindi il resto deve essere trasparente in modo che si possano vedere le altre figure 
    border.setOutlineThickness(20.f); //spessore del bordo 
    border.setOutlineColor(sf::Color(0, 100, 0));  //il bordo è verde scuro 

    State state;

    while (window.isOpen())
    {
        // events
        window.handleEvents (
                             [&window](const sf::Event::Closed&) { handle_close (window); },
                             [&window](const sf::Event::Resized& event) { handle_resize (event, window); }
        );

        // display
        window.clear(sf::Color(144, 238, 144));//lo sfondo al posto di essere nero sarà verde chiaro 
        state.draw(window); 
        window.draw(border); 
        window.display();

    }
}