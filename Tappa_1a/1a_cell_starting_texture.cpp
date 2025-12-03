#include <SFML/Graphics.hpp>
#include <vector> 
#include <string> 
#include "../risorse/textures_fonts.hpp"//file per le varie texture 

using namespace std; 

////////////////FINESTRA////////////////
const char* window_title = "Cell Starting Texture"; 
const unsigned window_width = 800; 
const unsigned window_height = 600;  
const float max_frame_rate = 60; 
const float window_horizontal_displacement = 100; //AGGIUNTA: distanza orizzontale dal bordo della finestra 
const float window_vertical_displacement = 100; //AGGIUNTA: distanza verticale dal bordo della finestra 
sf::Color window_background_color = sf::Color(144, 238, 144); //AGGIUNTA: colore dello sfondo della finestra (verde chiaro)

////////////////BORDO FINESTRA////////////////
const float window_border_thickness = 15; //AGGIUNTA: spessore del bordo della 
sf::Color window_border_color = sf::Color(0, 100, 0); //AGGIUNTA: colore del bordo della finestra (colore del bordo della finestra)

////////////////CELLA////////////////

enum class cell_type{Mine,Empty, Number}; //AGGIUNTA: stati nascosti possibili della cella 
enum class cell_state{ Covered, Revealed, Flag}; // AGGIUNTA: stati visibili possibili della cella 

////////////////STRUCT////////////////

//AGGIUNTA: cella della griglia di gioco 
struct Cell
{
    sf::Vector2f cell_pos; //posizione della cella nella griglia
    float cell_size; //dimensione della cella (nelle fasi successive cambierà in base al numero di celle presenti nella griglia determinate dalla difficoltà)
    sf::Texture* cell_texture;//texture della cella (per ora sara' solo Covered)
    cell_type cell_type; //cosa nasconde la cella (mina, nulla, numero)
    int mine_adj; //numero di mine nascoste nelle celle intorno alla cella considerata
    cell_state cell_state; //stato visibile della cella

    //creazione della cella
    Cell (sf::Vector2f pos, float size) : 
                                            cell_pos (pos),
                                            cell_size (size),
                                            cell_texture (&Covered_texture), //essendo che la cella è di tipo covered all'inizio, allora avrà anche la texture covered 
                                            cell_type(cell_type::Empty), //non sono state ancora posizionate le mine quindi il tipo nsscosto sarà inizialmente Empty
                                            mine_adj(0), //non sono state ancora posizionate le mine quindi il numero di mine adiacenti è inzialmente zero
                                            cell_state(cell_state::Covered) //inizialmente lo stato visibile della cella sarà sempre Covered 
                                            {}
    void draw (sf::RenderWindow& window); 
};

//AGGIUNTA: griglia contenente tutte le celle 
struct Grid
{
    std::vector<Cell> cells; //vettore delle varie celle che comporranno la griglia 
    sf::Vector2i cell_num; //numero di celle nella griglia 
    sf::Vector2f Grid_pos; //posizione della griglia nella finestra basata sul pannello 
    sf::Vector2f Grid_size; //dimensione della griglia

    Grid (sf::Vector2i cell_num, float cell_size); //metodo per creazione della griglia con impostato il numero di celle e la dimensione di una singola cella
    void draw (sf::RenderWindow& window);
};

//AGGIUNTA: pannello di gioco (che per ora conterrà solo la griglia)
struct Game_Panel
{
    float cell_size; //dimensione di ogni cella presente nella griglia
    Grid grid;  //griglia di gioco 
    
    //creazione del pannello di gioco
    Game_Panel(sf::Vector2i cell_num):
                                        cell_size(((window_height - (window_vertical_displacement * 2)) / cell_num.y)), //la dimensione delle celle per ora terrà solo conto dello spazio per la griglia 
                                        grid(cell_num, cell_size) {} 
    void draw (sf::RenderWindow& window);
};

//AGGIUNTA: stato generale della finestra 
struct State 
{
    Game_Panel game_panel; 

    //creazione dello stato
    State () : game_panel({9,9}) {} //per ora sarà disponibile solo il pannello di gioco con una griglia 9x9
    void draw (sf::RenderWindow& window);
};

////////////////CREAZIONE////////////////

//AGGIUNTA: creazione della griglia
Grid::Grid (sf::Vector2i bs, float cell_size){
    cell_num = bs; //imposta il numero di celle 

    Grid_size = {cell_size * cell_num.x, cell_size * cell_num.y}; //calcolo della grandezza della griglia in base al numero di celle al suo interno

    //imposto la posizione della griglia centralmente verticalmente mentre a destra orizzontalmente
    Grid_pos = { 
        (window_width - Grid_size.x) - window_horizontal_displacement,
        (window_height - Grid_size.y - cell_num.y) / 2
    };

    //dichiaro una variabile vettore pos in modo da non dover dichiarare una nuova variabile ad ogni fase del ciclo e poter salvare la posizione della cella così da poterla usare per la creazione della cella stessa 
    sf::Vector2f pos; 
    
    //per ogni riga della griglia 
    for (float row = 0; row < cell_num.y; row++) {
        //per ogni cella in detta riga considerata (per ogni colonna della griglia)
        for (float col = 0; col < cell_num.x; col++) {
            //calcolo la posizione della cella considerata
            pos = {
                Grid_pos.x + col * cell_size,
                Grid_pos.y + row * cell_size
            };

            //creo (con la posizione calcolata precedentemente) e inserisco una nuova cella nel vettore rappresentante la griglia
            cells.push_back(Cell(pos, cell_size)); 
        }
    }
}
////////////////DRAW////////////////

//AGGIUNTA: cella 
void Cell::draw (sf::RenderWindow& window)
{
    sf::RectangleShape c ({cell_size,cell_size}); //ogni cella è un quadrato (quindi in rettangolo con altezza uguale alla larghezza)
    c.setTexture(cell_texture); //imposta la texture della cella
    c.setPosition(cell_pos); //imposta la cella nella posizione corretta 
    window.draw(c); //disegna la cella nella finestra 

}

//AGGIUNTA: griglia
void Grid::draw (sf::RenderWindow& window)
{
    //disegna ogni cella della griglia (una alla volta)
    for (auto& cell : cells) 
        cell.draw (window);
}

//AGGIUNTA: pannello di gioco
void Game_Panel::draw(sf::RenderWindow& window)
{
    grid.draw(window); //disegna la griglia 
}

//AGGIUNTA: stato
void State::draw (sf::RenderWindow& window){
    game_panel.draw (window); //disegna il pannello di gioco
}

////////////////EVENTI////////////////

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

////////////////LOOP////////////////

int main()
{
    load_textures_fonts(); //AGGIUNTA: loading del file delle texture 
    sf::RenderWindow window (sf::VideoMode ({window_width, window_height}), window_title); 
    window.setFramerateLimit (max_frame_rate);
    window.setMinimumSize(window.getSize());

    //AGGIUNTA: creazione di un bordo per la finestra 
    sf::RectangleShape border({(window_width - window_border_thickness*2),(window_height- window_border_thickness*2)}); //AGGIUNTA: viene lasciato il bordo dello spessore del bordo dai confini della finestra in modo da rendere visibile il bordo 
    border.setPosition({window_border_thickness, window_border_thickness}); //AGGIUNTA: il bordo viene messo leggermente più sotto dell'origine per lo stesso motivo spiegato precedentemente
    border.setFillColor(sf::Color::Transparent); //AGGIUNTA: la figura creata deve essere solo un bordo quindi il resto di detta figura deve essere trasparente 
    border.setOutlineThickness(window_border_thickness); //AGGIUNTA: imposta lo spessore del bordo indicato precedentemente
    border.setOutlineColor(window_border_color);  //AGGIUNTA: imposta il colore del bordo indicato precedentemente

    State state;

    while (window.isOpen())
    {
        window.handleEvents (
                             [&window](const sf::Event::Closed&) { handle_close (window); },
                             [&window](const sf::Event::Resized& event) { handle_resize (event, window); }
        );

        window.clear(window_background_color);//MODIFICATO: lo sfondo al posto di essere nero sarà del colore indicato precedentemente 
        state.draw(window); 
        window.draw(border); 
        window.display();
    }
}