#include <SFML/Graphics.hpp>
#include <vector> 
#include <string> 
#include "../risorse/textures_fonts.hpp"//file per le varie texture 

using namespace std; 

////////////////FINESTRA////////////////

const char* window_title = "Victory, Defeat and Restart"; 
const unsigned window_width = 800; 
const unsigned window_height = 600;  
const float max_frame_rate = 60; 
const float window_horizontal_displacement = 100; 
const float window_vertical_displacement = 100;
sf::Color window_background_color = sf::Color(144, 238, 144); 

////////////////BORDO FINESTRA////////////////

const float window_border_thickness = 15; 
sf::Color window_border_color = sf::Color(0, 100, 0);

////////////////TESTO GENERALE////////////////
sf::Color text_color = sf::Color::Black;//AGGIUNTA: colore del testo nel gioco
sf::Color text_border_color = sf::Color::White; //AGGIUNTA: colore del bordo del testo nel gioco
const float text_thickness = 2; //AGGIUNTA: spessore del bordo del testo del gioco

////////////////PANNELLI GENERALE////////////////
sf::Color panel_background_color = sf::Color(210,180,140); //AGGIUNTA: Colore che verrà utilizzato come background per tutti i pannelli nel gioco a parte il game_panel
sf::Color panel_border_color = sf::Color(92,51,23); //AGGIUNTA: Colore che verrà utilizzato per il bordo di tutti i pannelli nel gioco a parte il game_panel
sf::Color focus_color = sf::Color::Red; 
const float panel_thickness = 15; //AGGIUNTA: spessore del bordo di tutti i pannelli di gioco a parte il game panel

////////////////CELLA////////////////

enum class cell_type{Mine,Empty, Number}; 
enum class cell_state{ Covered, Revealed, Flag}; 

////////////////GAME STOP////////////////

const float stop_width = (window_width/3)*2;//AGGIUNTA: larghezza del pannello di stop uguale a due terzi di quella della finestra
const float stop_height = (window_height/3)*2;//AGGIUNTA: altezza del pannello di stop uguale a due terzi di quella della finestra
const float stop_pos_x = window_width/6; //AGGIUNTA: posizione x del pannello di stop
const float stop_pos_y = window_height/6; //AGGIUNTA: posizione y del pannello di stop
const float stop_gap = 20; //AGGIUNTA: gap tra le scritte del fine gioco
enum class stop_type{None, Win, Lose}; //AGGIUNTA: possibili tipi di schermate di stop (per ora solo vittoria e sconfitta)
const unsigned stop_title_size = 90; //AGGIUNTA: dimensione del titolo del pannello (per la vittoria/sconfitta)
const unsigned stop_subtitle_size = 30; //AGGIUNTA: dimensione dei sottotitoli del pannello di stop (per la vittoria/sconfitta)


////////////////STRUCT////////////////


struct Cell
{
    sf::Vector2f cell_pos; 
    float cell_size; 
    int row_index, column_index;
    sf::FloatRect bounds; 
    bool mouse_focus; 
    sf::Texture* cell_texture;
    cell_type cell_type; 
    int mine_adj; 
    cell_state cell_state;
    float gap;


    Cell (sf::Vector2f pos, float size, int column_index, int row_index, float gap) : 
                                            cell_pos (pos),
                                            cell_size (size),
                                            row_index(row_index),
                                            column_index(column_index),
                                            bounds (cell_pos, {cell_size, cell_size}),
                                            mouse_focus(false),
                                            cell_texture (&Covered_texture), 
                                            cell_type(cell_type::Empty), 
                                            mine_adj(0), 
                                            cell_state(cell_state::Covered),
                                            gap(gap) {}
    void draw (sf::RenderWindow& window); 
};

struct Grid
{
    std::vector<Cell> cells; 
    sf::Vector2i cell_num; 
    sf::Vector2f Grid_pos; 
    sf::Vector2f Grid_size; 
    int mine_num;
    int num_revealed; //AGGIUNTA: numero di celle rivelate per avere un modo per capire quando si ha vinto 

    Grid (sf::Vector2i cell_num, int mine_num, float& cell_size, float gap);
    void place_mines(int starting_index_cell); 
    void place_numbers(); 
    void draw (sf::RenderWindow& window);
};

struct Game_Panel
{
    float cell_size; 
    float gap; 
    Grid grid;  

    
    Game_Panel(sf::Vector2i cell_num, int mine_num):
                                        cell_size(((window_height - (window_vertical_displacement * 2)) / cell_num.y)),
                                        gap(2),
                                        grid(cell_num, mine_num, cell_size, gap) 
                                        {} 
    void draw (sf::RenderWindow& window);
};

//AGGIUNTA: schermata pausa o fine partita (per ora solo di fine partita)
struct Stop_Panel{ 
    sf::Text title{font}; //scritte nel pannello
    stop_type stop_type; //indica il tipo di pannello di stop che dovrà apparire (vittoria e sconfitta)
    sf::Vector2f stop_size; //dimensione del pannello
    sf::Vector2f stop_pos; //posizione del pannello
    bool visible; //indica se il pannello è visibile o meno
    
    Stop_Panel (): 
                visible(false), //la schermata non sarà visibile inizialmente 
                stop_type(stop_type::None), 
                stop_size({stop_width, stop_height}), 
                stop_pos({stop_pos_x,stop_pos_y}) {}

    void draw (sf::RenderWindow& window);
}; 

struct State 
{
    Game_Panel game_panel;
    Stop_Panel stop_p; //AGGIUNTA: creazione del pannello di stop
    int mouse_cell;
    bool focus; 
    bool first_move; 
    bool game_ended; //AGGIUNTA: indice se la partita è o meno finita

    State () : 
            game_panel({9,9}, 15),
            stop_p(),
            focus(false), 
            first_move(true),
            mouse_cell(-1), 
            game_ended(false) {} //game_ended è inizialemnte false poichè la aprtita sta iniziando ora 

    void reveal(Grid& g, int starting_index_cell);
    void flood_reveal(Grid& g, int starting_index_cell, Cell& start_c);
    void ending_reveal(Grid& g, int starting_index_cell); //AGGIUNTA: funzione per l'animazione di fine partita 
    void reset(); //AGGIUNTA: funzione per il reset del gioco dopo la fine di una partita
    void draw (sf::RenderWindow& window);
};

////////////////CREAZIONE////////////////

Grid::Grid (sf::Vector2i bs, int bn, float& cell_size, float gap){
    cell_num = bs; 
    mine_num = bn;
    num_revealed = 0; //AGGIUNTA: a inizio partita il numero di celle rivelate è sempre zero
    Grid_size = {
        (cell_size * cell_num.x) + (gap * (cell_num.x - 1)),
        (cell_size * cell_num.y) + (gap * (cell_num.y - 1))
    };

    Grid_pos = { 
        window_width - Grid_size.x - window_horizontal_displacement,
        (window_height - Grid_size.y - cell_num.y) / 2
    };

    sf::Vector2f pos; 

    for (int row = 0; row < cell_num.y; row++) {
        for (int col = 0; col < cell_num.x; col++) {
            pos = {
                Grid_pos.x + col * (cell_size + gap), 
                Grid_pos.y + row * (cell_size + gap)
            };
            cells.push_back(Cell(pos, cell_size, col, row, gap)); 
        }
    } 
}
////////////////DRAW////////////////

void Cell::draw (sf::RenderWindow& window)
{
    sf::RectangleShape c ({cell_size,cell_size}); 
    c.setTexture(cell_texture); 
    c.setPosition(cell_pos); 

    if(mouse_focus){
        c.setOutlineThickness(gap); 
        c.setOutlineColor(focus_color); 
    }

    window.draw(c);

}

void Grid::draw (sf::RenderWindow& window)
{
    for (auto& cell : cells) 
        cell.draw (window);
}

void Game_Panel::draw(sf::RenderWindow& window)
{
    grid.draw(window); 
}

//AGGIUNTA: 
void Stop_Panel::draw(sf::RenderWindow& window){
    if(!visible) return; //il pannello viene rappresentata solo a fine partita o in pausa 

    sf::RectangleShape s(stop_size); //il pannello è un rettangolo
    s.setPosition(stop_pos);//posizionamento del pannello
    s.setFillColor(panel_background_color); //impostazione colore di background del pannelo
    s.setOutlineThickness(panel_thickness); //impostazione dello spessore del bordo del pannello
    s.setOutlineColor(panel_border_color); //impostazione del colore del bordo del pannello
    window.draw(s); 
    float stop_text_pos_x = stop_pos.x + stop_size.x/2;

    switch(stop_type)//il testo della schermata varia in base al risultato della partita
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
    title.setCharacterSize(stop_title_size); //dimensione della scritta 
    title.setFillColor(text_color); //imposto il colore del testo
    title.setOutlineThickness(text_thickness); //imposto lo spessore del bordo del testo
    title.setOutlineColor(text_border_color); //imposto il colore del bordo del testo
    auto b = title.getLocalBounds(); 
    //prendo prima la posizione del pannello nella finestra in modo da poi centrare la scritta correttamente per larghezza. Per altezza invece la scritta viene leggermente spostata ogni volta partendo dalla parte alta del pannello
    title.setOrigin({b.position.x + b.size.x/2, b.position.y}); 
    title.setPosition({stop_text_pos_x, stop_pos.y + stop_gap});
    window.draw(title);

    //stesso ragionamento fatto per la scritta precedente 
    title.setString("Premere SPACE");
    title.setCharacterSize(stop_subtitle_size);
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x/2, b.position.y});
        title.setPosition({stop_text_pos_x,title.getPosition().y + stop_title_size + stop_gap});  
    window.draw(title);

    //stesso ragionamento fatto per la scritta precedente 
    title.setString("per cominciare una nuova");
    title.setCharacterSize(stop_subtitle_size);
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x/2, b.position.y});
        title.setPosition({stop_text_pos_x,title.getPosition().y + stop_subtitle_size + stop_gap});  
    window.draw(title);

    //stesso ragionamento fatto per la scritta precedente 
    title.setString("PARTITA");
    title.setCharacterSize(stop_subtitle_size);
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x/2, b.position.y});
        title.setPosition({stop_text_pos_x,title.getPosition().y + stop_subtitle_size + stop_gap});  
    window.draw(title);
}

void State::draw (sf::RenderWindow& window){
    game_panel.draw (window); 
    stop_p.draw(window); //AGGIUNTA: disegno del pannello di stop 
}

////////////////ALTRE FUNZIONI////////////////

void Grid::place_mines(int starting_cell_index){

    int x = cells[starting_cell_index].row_index; 
    int y = cells[starting_cell_index].column_index;
    int casual_index;

    int i=0; 
    while(i<mine_num){ 
        casual_index = (rand()% cells.size()+1)-1; 

       if(abs(cells[casual_index].row_index -x)<=1 && abs(cells[casual_index].column_index - y)<=1) 
            continue;
        
        else if(cells[casual_index].cell_type == cell_type::Mine) 
            continue;

        else{
            cells[casual_index].cell_type = cell_type::Mine; 
            i++; 
        }
    }

    for (Cell& c : cells) c.mine_adj = 0;

    int num_cols;

    for(int j=0; j<cells.size(); j ++){

        if(cells[j].cell_type == cell_type::Mine){ 
            x = cells[j].row_index; 
            y = cells[j].column_index; 
            num_cols = cell_num.x; 

            if(x > 0){
                cells[j-num_cols].mine_adj++; 
                if(y > 0) cells[j-num_cols-1].mine_adj++; 
                if(y < (num_cols-1)) cells[j-num_cols+1].mine_adj++;
            }

            if(x < (cell_num.y-1)){
                cells[j+num_cols].mine_adj++; 
                if(y > 0) cells[j+num_cols-1].mine_adj++; 
                if(y < (num_cols-1)) cells[j+num_cols+1].mine_adj++; 
            }

            if(y > 0) cells[j-1].mine_adj++; 
            if(y < (num_cols-1)) cells[j+1].mine_adj++; 
        }
    }
}

void Grid::place_numbers(){
    for(int i=0; i<cells.size();i++){
        if(cells[i].cell_type == cell_type::Mine) continue;
        if(cells[i].mine_adj == 0) cells[i].cell_type = cell_type::Empty; 
        else cells[i].cell_type = cell_type::Number; 
    }
}

void State::flood_reveal(Grid& g, int starting_index_cell, Cell& start_c){

    int cols = g.cell_num.x;
    int rows = g.cell_num.y;
    int up_index = starting_index_cell - cols; 
    int down_index = starting_index_cell + cols; 
    int start_col = start_c.column_index; 
    int start_row = start_c.row_index; 

    if(start_row > 0){
        if(g.cells[up_index].cell_type != cell_type::Mine) reveal(g,up_index); 
        if((start_col > 0) && (g.cells[up_index-1].cell_type != cell_type::Mine)) reveal(g, up_index-1); 
        if((start_col < (cols-1)) && (g.cells[up_index+1].cell_type != cell_type::Mine)) reveal(g, up_index+1);
    }

    if(start_row < (rows-1)){
        if(g.cells[down_index].cell_type !=  cell_type::Mine) reveal(g, down_index);
        if((start_col > 0) && (g.cells[down_index-1].cell_type != cell_type::Mine)) reveal(g, down_index-1);
        if((start_col < (cols-1)) && (g.cells[down_index+1].cell_type != cell_type::Mine)) reveal(g, down_index+1);        
    }

    if((start_col > 0) && (g.cells[starting_index_cell-1].cell_type != cell_type::Mine)) reveal(g, starting_index_cell-1);

    if((start_col < (cols-1)) && (g.cells[starting_index_cell+1].cell_type != cell_type::Mine)) reveal(g, starting_index_cell+1);
}

void State::reveal(Grid& g, int starting_index_cell){

    Cell& c = g.cells[starting_index_cell]; 

    if(c.cell_state == cell_state::Revealed) return;
    c.cell_state = cell_state::Revealed;
    game_panel.grid.num_revealed++; //AGGIUNTA: per ogni cella rivelata si incrementa il contatore di celle rivelate in partita 

    if(c.cell_type == cell_type::Mine){
        c.cell_texture = &Exploded_Mine_texture;
        stop_p.stop_type = stop_type::Lose;  //AGGIUNTA: si imposta che la partita è finita con una sconfitta 
        ending_reveal(g,starting_index_cell); //AGGIUNTA: si fa partire l'animazione di fine partita
        return; 
    } 
    else if(c.cell_type == cell_type::Number){
        c.cell_texture = &Number_textures[c.mine_adj-1];
    }
    else{
        c.cell_texture = &Empty_texture; 
        flood_reveal(g, starting_index_cell, c);
    }

    //AGGIUNTA: nel caso si siano rivelate tutte le celle a parte quelle con sotto una mina si ha vinto la partita e si deve far partire l'animazione di fine partita
    if (game_panel.grid.num_revealed == static_cast<int>(g.cells.size()) - g.mine_num) {
        stop_p.stop_type = stop_type::Win;
        ending_reveal(g, starting_index_cell);
    }
}

//AGGIUNTA: funzione per l'animazione di fine partita 
void State::ending_reveal(Grid& g, int starting_index_cell){

    //nel caso di una sconfitta vengono rivelate tutte le celle con tipo Mine (a parte la cella della mina esplosa) e vengono anche rivelate le 'false mine'(i casi in cui è stata messa una bandierina su una cella che non nascondeva una mina)
    for(int i = 0; i < g.cells.size(); i++){

    if(stop_p.stop_type == stop_type::Lose && i == starting_index_cell) continue;

            //rivelazione mine 
            if(g.cells[i].cell_state == cell_state::Flag && g.cells[i].cell_type != cell_type::Mine){
                g.cells[i].cell_state = cell_state::Revealed; 
                g.cells[i].cell_texture = &False_Mine_texture; 
            }

            //false mine
            if(g.cells[i].cell_type == cell_type::Mine){
                g.cells[i].cell_state = cell_state::Revealed; 
                g.cells[i].cell_texture = &Normal_Mine_texture; 
            }
    }

    game_ended = true;  //viene impostato il gioco come finito 
    stop_p.visible = true; //il pannello viene reso visibile 
    
}

//AGGIUNTA: funzione per il reset dello stato della finestra in modo da poter iniziare una nuova partita 
void State::reset(){
    game_panel = Game_Panel(game_panel.grid.cell_num, game_panel.grid.mine_num); 
    stop_p = Stop_Panel(); 
    focus = game_ended= false;
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
}

void handle (const sf::Event::FocusLost&, State& state)
{
    state.focus = false; 
}

void handle (const sf::Event::MouseButtonPressed& mouse, State& state)
{
    if(state.mouse_cell <0 || state.mouse_cell >= state.game_panel.grid.cells.size()) return; 

    Cell& cur_cell = state.game_panel.grid.cells[state.mouse_cell]; 

    if(cur_cell.cell_state == cell_state::Revealed) return; 

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

        if(cur_cell.cell_state != cell_state::Flag){
            cur_cell.cell_state = cell_state::Flag; 
            cur_cell.cell_texture = &Flag_texture;
        }
        else{
            cur_cell.cell_state = cell_state::Covered; 
            cur_cell.cell_texture = &Covered_texture;
        }
    }
}

void handle_mouse_moved (const sf::Event::MouseMoved& mouse, sf::RenderWindow& window, State& state){
    if(state.game_ended) return; //AGGIUNTA: quando la partita è finita non è permesso nessun tipo di mossa a parte enter 

    const sf::Vector2f mouse_float_pos = window.mapPixelToCoords(sf::Vector2i(mouse.position.x, mouse.position.y));

    int new_idx =-1;
    for (int i = 0; i < state.game_panel.grid.cells.size(); ++i) {
        if (state.game_panel.grid.cells[i].bounds.contains(mouse_float_pos)) {
            new_idx = i; 
            break;
        }
    }

    if (new_idx == state.mouse_cell) return;

    if(state.mouse_cell >= 0)
        state.game_panel.grid.cells[state.mouse_cell].mouse_focus = false;

    state.mouse_cell = new_idx;
    if (state.mouse_cell >= 0)
        state.game_panel.grid.cells[state.mouse_cell].mouse_focus = true;
}

//AGGIUNTA: funzione per la gestione dell'evento caratterizzato dal click del pulsante SPACE sulla tastiera 
void handle(const sf::Event::KeyPressed& key, State& state) {
    if (state.game_ended && key.scancode == sf::Keyboard::Scancode::Space) state.reset(); //il premere SPACE deve provocare l'evento di reset solo quando la partita è finita 
}

////////////////LOOP////////////////

int main()
{
    srand(time(NULL));
    load_textures_fonts(); 

    sf::RenderWindow window (sf::VideoMode ({window_width, window_height}), window_title); 
    window.setFramerateLimit (max_frame_rate);
    window.setMinimumSize(window.getSize());

    sf::RectangleShape border({(window_width - window_border_thickness*2),(window_height- window_border_thickness*2)}); 
    border.setPosition({window_border_thickness, window_border_thickness}); 
    border.setFillColor(sf::Color::Transparent); 
    border.setOutlineThickness(window_border_thickness); 
    border.setOutlineColor(window_border_color);  

    State state;

    while (window.isOpen())
    {
        window.handleEvents (
                             [&window](const sf::Event::Closed&) { handle_close (window); },
                            [&window](const sf::Event::Resized& event) { handle_resize (event, window); }, 
                            [&window, &state](const sf::Event::MouseMoved& event) {handle_mouse_moved (event, window, state); },
                            [&state] (const auto& event) { handle (event, state); }
        );

        window.clear(window_background_color);
        state.draw(window); 
        window.draw(border); 
        window.display();
    }
}