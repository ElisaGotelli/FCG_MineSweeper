#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib> 
#include <ctime> 
#include <cmath>
#include "../textures_fonts.hpp"

using namespace std; 

////////////////FINESTRA////////////////
const char* window_title = "Victory, defeat and restart";
const unsigned window_width = 1200;
const unsigned window_height = 900;
const float max_frame_rate = 60;

////////////////PANNELLO DI GIOCO////////////////

const float panel_horizontal_displacement = 100; 
const float panel_vertical_displacement = 100; 
const float gap = 2.f; 

////////////////GAME STOP////////////////

const float stop_gap = 15.f; //AGGIUNTA: gap tra le scritte del fine gioco
enum class stop_type{None, Win, Lose}; //AGGIUNTA: possibili tipi di schermate di stop (per ora solo vittoria e sconfitta)
const unsigned stop_height = window_height/2.f; //altezza della schermata di stop uguale alla metà di quella della finestra
const unsigned stop_width = window_width/2.f;  //larghezza della schermata di stop uguale alla metà di quella della finestra

////////////////BLOCCO////////////////

enum class cell_type{Mine,Empty, Number}; 
enum class cell_state{ Covered, Revealed, Flag}; 

////////////////STRUCT////////////////
struct Cell
{
    sf::Vector2f pos; 
    float size; 
    int row_index, column_index; 
    sf::FloatRect bounds; 
    bool mouse_focus; 
    sf::Texture*  texture;
    cell_type type; 
    int mine_adj;
    cell_state state;
    
    Cell (sf::Vector2f pos, float size, int row_index, int column_index) : pos (pos),
                                                  size (size),
                                                  row_index(row_index),
                                                  column_index(column_index),
                                                  bounds (pos, {size, size}),
                                                  mouse_focus(false),
                                                  texture (&Covered_texture),
                                                  type(cell_type::Empty), 
                                                  mine_adj(0),
                                                  state(cell_state::Covered) {}
    void draw (sf::RenderWindow& window);
};

struct Grid
{
    vector<Cell> cells;  
    sf::Vector2i cell_num;  
    sf::Vector2f Grid_size;
    sf::Vector2f Grid_pos;
    int mine_num; 
    int num_revealed; //AGGIUNTA: numero di celle rivelate per avere un modo per capire quando si ha vinto 

    Grid (sf::Vector2i cell_num, int mine_num, float cell_size); 
    void place_mines(int starting_index_cell); 
    void place_numbers(); 
    void draw (sf::RenderWindow& window);
};
struct Game_Panel
{
    float cell_size;
    Grid grid;  
    
    Game_Panel(sf::Vector2i cell_num, int mine_num):
                                                    cell_size(((window_height - (panel_vertical_displacement * 2)) / cell_num.y) * 0.85f),
                                                    grid(cell_num, mine_num, cell_size) {} 
    void draw (sf::RenderWindow& window);
};

//AGGIUNTA: schermata di vittoria/sconfitta 
struct Game_Stop{ 
    sf::Text title{font}; //scritta 'Hai vinto!' o 'Hai perso!' e di premere ENTER 
    stop_type type; //indica il tipo di schermata che dovrà apparire (vittoria e sconfitta)
    sf::Vector2f gs_size; //dimensione della schermata
    sf::Vector2f gs_pos; //posizione della schermata 
    bool visible; //indica se la schermata è visibile o meno (visibile solo a partita finita)
    
    Game_Stop (): //la schermata non sarà visibile inizialmente 
                visible(false), 
                type(stop_type::None), 
                gs_size({stop_width, stop_height}), 
                gs_pos({(window_width - stop_width)/2.f, (window_height - stop_height)/2.f}) {}
                
    void draw (sf::RenderWindow& window);
}; 
struct State  
{
    Game_Panel game_panel;
    Game_Stop gs; //AGGIUNTA: creazione della schermata 
    int mouse_cell; 
    bool focus; 
    bool game_paused; 
    bool first_move; 
    bool game_ended; //AGGIUNTA: indice se la partita è o meno finita    

    State (): 
                game_panel({9,9}, 15), 
                gs(),
                focus(false), 
                game_paused(true), 
                first_move(true),
                mouse_cell(-1), 
                game_ended(false) {} //game_ended a false visto che la partita sta appena iniziando 
    
    void reveal(Grid& g, int starting_index_cell); 
    void flood_reveal(Grid& g, int starting_index_cell, Cell& start_c); 
    void ending_reveal(Grid& g, int starting_index_cell); //AGGIUNTA: funzione per l'animazione di fine partita 
    void reset(); //AGGIUNTA: funzione per il reset del gioco dopo la fine di una partita 
    void draw (sf::RenderWindow& window);
};

////////////////CREAZIONE////////////////

Grid::Grid (sf::Vector2i bs, int bn, float cell_size){
    cell_num = bs; 
    mine_num = bn; 
    num_revealed = 0; //inizialmente il numero di celle rivelate è zero prima della prima mossa 

    Grid_size = {cell_size * cell_num.x, cell_size * cell_num.y};
 
    Grid_pos = { 
        (window_width - Grid_size.x) - panel_horizontal_displacement - (gap * cell_num.x),
        (window_height - Grid_size.y - (gap * cell_num.y)) / 2.0f
    };
    sf::Vector2f pos;  
    
    for (unsigned hb = 0; hb < cell_num.x; hb++) {
        for (unsigned vb = 0; vb < cell_num.y; vb++) {
            pos = {
                Grid_pos.x + hb * (cell_size + gap), 
                Grid_pos.y + vb * (cell_size + gap)
            };
            cells.push_back(Cell(pos, cell_size, hb, vb)); 
        }
    } 
}

////////////////DRAW////////////////

void Cell::draw (sf::RenderWindow& window)
{
    sf::RectangleShape c ({size,size});
    c.setTexture(texture);
    c.setPosition(pos);

    if(mouse_focus){
        c.setOutlineThickness(2.0f); 
        c.setOutlineColor(sf::Color::Red);
    }

    window.draw(c);

}

void Grid::draw (sf::RenderWindow& window)
{
    for (auto& cell : cells)
        cell.draw (window);
}

//AGGIUNTA: rappresentazione della schermata di vittoria/sconfitta 
void Game_Stop::draw(sf::RenderWindow& window){
    if(!visible) return; //la schermata viene rappresentata solo a fine partita

    sf::RectangleShape s(gs_size); //la schermata è un rettangolo di dimensione metà della finestra 
    s.setPosition(gs_pos);//la schermata sarà al centro della finestra 
    s.setFillColor(sf::Color(210,180,140)); //la schermata avrà colore marrone chiaro 
    s.setOutlineThickness(20.f); //la schermata avrà un bordo
    s.setOutlineColor(sf::Color(92,51,23)); // il bordo della schermata sarà di colore scuro 
    window.draw(s); 

    switch(type)//il testo della schermata varia in base al risultato della partita 
    {      
        case stop_type::Win: 
            title.setString("Hai vinto!"); 
            break; 

        case stop_type::Lose: 
            title.setString("Hai perso!"); 
            break; 

        default: 
            return; 
    } 
    title.setCharacterSize(140); //dimensione della scritta 
    title.setFillColor(sf::Color::Black); //la scritta è di colore nero 
    title.setOutlineThickness(2.f); //la scritta avrà un bordo
    title.setOutlineColor(sf::Color::White); //il bordo sarà bianco della scritta 
    auto b = title.getLocalBounds(); 
    //prendo prima la posizione della schermata nella finestra in modo da poi centrare la scritta correttamente per larghezza. Per altezza invece la scritta viene leggermente spostata ogni volta partendo dalla parte alta
    title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y}); 
    title.setPosition({s.getPosition().x + s.getSize().x/2.f, gs_pos.y + stop_gap});               
    window.draw(title);

    //stesso ragionamento fatto per la scritta precedente 
    title.setString("Premere SPACE");
    title.setCharacterSize(40);
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
    title.setPosition({title.getPosition().x,  title.getPosition().y + 140.f + stop_gap}); 
    window.draw(title);

    //stesso ragionamento fatto per la prima scritta 
    title.setString("per cominciare una nuova partita");
    title.setCharacterSize(40);
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
    title.setPosition({title.getPosition().x,title.getPosition().y + title.getCharacterSize() + stop_gap});
    window.draw(title);
}

void Game_Panel::draw(sf::RenderWindow& window)
{
    grid.draw(window); 
}

void State::draw (sf::RenderWindow& window)
{
    game_panel.draw (window);
    gs.draw(window); 
}

////////////////ALTRE FUNZIONI////////////////

void Grid::place_mines(int starting_cell_index){

    int x = cells[starting_cell_index].row_index; 
    int y = cells[starting_cell_index].column_index;
    int casual_index; 

    srand(time(NULL)); 

   int i=0; 
    while(i<mine_num){ 
        casual_index = (rand()% cells.size()+1)-1; 

        if(abs(cells[casual_index].row_index -x)<=1 && abs(cells[casual_index].column_index - y)<=1) 
            continue;
        
        else if(cells[casual_index].type == cell_type::Mine) 
            continue;

        else{
            cells[casual_index].type = cell_type::Mine; 
            i++; 
        }
    }

    for (Cell& c : cells) c.mine_adj = 0;

    for(int j=0; j<cells.size(); j ++){

        if(cells[j].type == cell_type::Mine){
            x = cells[j].row_index; 
            y = cells[j].column_index; 

            if(x > 0){
                cells[j-cell_num.y].mine_adj++; 

                if(y > 0) cells[j-cell_num.y-1].mine_adj++; 
                if(y < (cell_num.y-1)) cells[j-cell_num.y+1].mine_adj++;
            }

            if(x < (cell_num.x-1)){
                cells[j+cell_num.y].mine_adj++; 
                if(y > 0) cells[j+cell_num.y-1].mine_adj++;
                if(y < (cell_num.y-1)) cells[j+cell_num.y+1].mine_adj++; 
            }

            if(y > 0) cells[j-1].mine_adj++; 
            if(y < (cell_num.y-1)) cells[j+1].mine_adj++; 
        }
    }
}

void Grid::place_numbers(){
    for(int i=0; i<cells.size();i++){
        if(cells[i].type == cell_type::Mine) continue; 
        if(cells[i].mine_adj == 0){
            cells[i].type = cell_type::Empty; 
        }
        else cells[i].type = cell_type::Number; 
    }
}

//AGGIUNTA: funzione per l'animazione di fine partita 
void State::ending_reveal(Grid& g, int starting_index_cell){

    //a parte la cella della mina esplosa nel caso di una sconfitta vengono rivelate tutte le altre mine e vengono anche rivelate le 'false mine' cioè i casi in cui è stata messa una bandierina su una cella che non nascondeva bombe 
    for(int i = 0; i < g.cells.size(); i++){

    //caso sconfitta e mina esplosa
    if(gs.type == stop_type::None && i == starting_index_cell ) continue;

            //rivelazione mine 
            if(g.cells[i].state == cell_state::Flag && g.cells[i].type != cell_type::Mine){
                g.cells[i].state = cell_state::Revealed; 
                g.cells[i].texture = &False_Mine_texture; 
            }

            //false mine
            if(g.cells[i].type == cell_type::Mine){
                g.cells[i].state = cell_state::Revealed; 
                g.cells[i].texture = &Normal_Mine_texture; 
            }
    }

    //viene impostato il gioco come finito 
    game_ended = true; 
    game_paused=false; //il gioco non è in pausa ma è finito
    //viene resa visibile la schermata di fine partita 
    gs.visible = true; 
    
}

void State::flood_reveal(Grid& g, int starting_index_cell, Cell& start_c){

    if(start_c.row_index > 0){
        if(g.cells[starting_index_cell-g.cell_num.y].type !=  cell_type::Mine) reveal(g, starting_index_cell-g.cell_num.y); 

        if((start_c.column_index > 0) && (g.cells[starting_index_cell-g.cell_num.y-1].type != cell_type::Mine) ) reveal(g, starting_index_cell-g.cell_num.y-1); 

        if((start_c.column_index < (g.cell_num.y-1)) && (g.cells[starting_index_cell-g.cell_num.y+1].type != cell_type::Mine)) reveal(g, starting_index_cell-g.cell_num.y+1);
    }

    if(start_c.row_index < (g.cell_num.x-1)){
        if(g.cells[starting_index_cell+g.cell_num.y].type !=  cell_type::Mine) reveal(g, starting_index_cell+g.cell_num.y);

        if((start_c.column_index > 0) && (g.cells[starting_index_cell+g.cell_num.y-1].type != cell_type::Mine)) reveal(g, starting_index_cell+g.cell_num.y-1);

        if((start_c.column_index < (g.cell_num.y-1)) && (g.cells[starting_index_cell+g.cell_num.y+1].type != cell_type::Mine)) reveal(g, starting_index_cell+g.cell_num.y+1);        
    }

    if((start_c.column_index > 0) && (g.cells[starting_index_cell-1].type != cell_type::Mine)) reveal(g, starting_index_cell-1);

    if((start_c.column_index < (g.cell_num.y-1)) && (g.cells[starting_index_cell+1].type != cell_type::Mine)) reveal(g, starting_index_cell+1);
}

void State::reveal(Grid& g, int starting_index_cell){

    Cell& c = g.cells[starting_index_cell]; 

    if(c.state == cell_state::Revealed) return;  
    c.state = cell_state::Revealed; 
    game_panel.grid.num_revealed++; //AGGIUNTA: per ogni cella rivelata si incrementa il contatore 

    if(c.type == cell_type::Mine){
        c.texture = &Exploded_Mine_texture;
        gs.type = stop_type::Lose;  //AGGIUNTA: si imposta che la partita è finita con una sconfitta 
        ending_reveal(g,starting_index_cell); //AGGIUNTA: si fa aprtire l'animazione di fine partita 
        return; 
    } 
    else if(c.type == cell_type::Number){
        c.texture = &Number_textures[c.mine_adj-1]; 
    }
    else{
        c.texture = &Empty_texture; 
        flood_reveal(g, starting_index_cell, c);
    }

    //AGGIUNTA: nel caso si siano rivelate tutte le celle a parte quelle con sotto una mina si ha vinto la partita quindi
    if (game_panel.grid.num_revealed == static_cast<int>(g.cells.size()) - g.mine_num) {
        gs.type = stop_type::Win; //si imposta che la aprtita è finita con una vittoria 
        ending_reveal(g, starting_index_cell); //si fa aprtire l'animazione di fine partita 
    }
}

//AGGIUNTA: si resetta lo stato della finestra in modo da poter iniziare una nuova partita 
void State::reset(){
    gs = Game_Stop(); 
    game_panel = Game_Panel(game_panel.grid.cell_num, game_panel.grid.mine_num); 
    game_paused = focus = game_ended= false;
    first_move = true;
    mouse_cell = -1; 
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

template <typename T>
void handle (T& event, State& state) {}

void handle (const sf::Event::FocusGained&, State& state)
{
    state.focus = true; 
    state.game_paused = false;
}

void handle (const sf::Event::FocusLost&, State& state)
{
    state.game_paused = true; 
    state.focus = false; 
}

void handle (const sf::Event::MouseButtonPressed& mouse, State& state)
{
    if(state.game_ended) return; 

    if(state.mouse_cell <0 || state.mouse_cell >= state.game_panel.grid.cells.size() || state.game_panel.grid.cells[state.mouse_cell].state == cell_state::Revealed) return;

    if( mouse.button == sf::Mouse::Button::Left){

        if(state.first_move){
            state.first_move = false; 
            state.game_panel.grid.place_mines(state.mouse_cell); 
            state.game_panel.grid.place_numbers(); 
            state.reveal(state.game_panel.grid, state.mouse_cell); 
        }
        else state.reveal(state.game_panel.grid, state.mouse_cell); 
    }

    if(mouse.button == sf::Mouse::Button::Right){
        if(state.first_move) return; 

        if(state.game_panel.grid.cells[state.mouse_cell].state != cell_state::Flag){
            state.game_panel.grid.cells[state.mouse_cell].state = cell_state::Flag; 
            state.game_panel.grid.cells[state.mouse_cell].texture = &Flag_texture;
        }
        else{
            state.game_panel.grid.cells[state.mouse_cell].state = cell_state::Covered; 
            state.game_panel.grid.cells[state.mouse_cell].texture = &Covered_texture;
        }   
    }

}

//AGGIUNTA: gestione dell'evento caratterizzato dal click del pulsante SPACE sulla tastiera 
void handle(const sf::Event::KeyPressed& key, State& state) {
    if (state.game_ended && key.scancode == sf::Keyboard::Scancode::Space) state.reset(); //il premere SPACE deve provocare l'evento di reset solo quando la partita è finita 
}


void handle (const sf::Event::MouseMoved& ev, State& state)
{
    if(state.game_ended) return; //AGGIUNTA: quando la partita è finita non è permesso nessun tipo di mossa a parte enter 
    const sf::Vector2f mouse_float_pos{
        static_cast<float>(ev.position.x),
        static_cast<float>(ev.position.y)
    };

    int new_idx =-1;
    for (int i = 0; i < state.game_panel.grid.cells.size(); ++i) {
        if (state.game_panel.grid.cells[i].bounds.contains(mouse_float_pos)) { new_idx = i; break; }
    }

    if (new_idx == state.mouse_cell) return;
 
    if (state.mouse_cell >= 0)
        state.game_panel.grid.cells[state.mouse_cell].mouse_focus = false;

    state.mouse_cell = new_idx;
    if (state.mouse_cell >= 0)
        state.game_panel.grid.cells[state.mouse_cell].mouse_focus = true;
}

////////////////LOOP////////////////
int main()
{
    load_textures_fonts();
    sf::RenderWindow window (sf::VideoMode ({window_width, window_height}), window_title);
    window.setFramerateLimit (max_frame_rate);
    window.setMinimumSize(window.getSize()); 

    sf::RectangleShape border({(window_width - 40.f),(window_height-40.f)}); 
    border.setPosition({20.f, 20.f}); 
    border.setFillColor(sf::Color::Transparent);  
    border.setOutlineThickness(20.f);
    border.setOutlineColor(sf::Color(0, 100, 0));

    State state;

    while (window.isOpen())
    {
        window.handleEvents (
                             [&window](const sf::Event::Closed&) { handle_close (window); },
                             [&window](const sf::Event::Resized& event) { handle_resize (event, window); }, 
                             [&state] (const auto& event) { handle (event, state); } 
        );

        window.clear(sf::Color(144, 238, 144));
        state.draw(window); 
        window.draw(border); 
        window.display();

    }
}