#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib> 
#include <ctime> 
#include <cmath>
#include <SFML/System/Clock.hpp> //AGGIUNTO
#include "../textures_fonts.hpp"

using namespace std; 

////////////////FINESTRA////////////////
const char* window_title = "Header";
const unsigned window_width = 1200;
const unsigned window_height = 900;
const float max_frame_rate = 60;

////////////////PANNELLO DI GIOCO////////////////

const float panel_horizontal_displacement = 100; 
const float panel_vertical_displacement = 100; 
const float gap = 2.f;

////////////////GAME STOP////////////////

const float stop_gap = 15.f; 
enum class stop_type{None, Win, Lose}; 
const unsigned stop_height = window_height/2.f; 
const unsigned stop_width = window_width/2.f; 

////////////////HEADER////////////////

const float header_parameter_gap = 30.f; //AGGIUNTA: gap tra il bordo dell'header e i parametri
const float header_border_gap = 5.f; //AGGIUNTA: bordo dell'header 

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
    sf::Texture* texture;
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
    int num_revealed; 

    Grid (sf::Vector2i cell_num, int mine_num, float cell_size); 
    void place_mines(int starting_index_cell); 
    void place_numbers(); 
    void draw (sf::RenderWindow& window);
}; 

//AGGIUNTA: struttura per i numeri dell'orologio e in contatore delle bandierine 
struct Number{
    sf::Vector2f num_pos; //posizione di ogni numero
    sf::Vector2f num_size; //grandezza di ogni numero 
    sf::Texture* num_texture; //texture che inizialmente sarà sempre 0 
    int timer_number; //vero timer che viene incremetato ogni secondo 

    Number(sf::Vector2f pos, sf::Vector2f size): 
                                                num_pos(pos), 
                                                num_size(size), 
                                                num_texture(&Clock_textures[0]),
                                                timer_number(0) {}

    void draw (sf::RenderWindow& window);
    
};

//AGGIUNTA: struttura del timer di gioco 
struct Timer
{
    vector<Number> timer_numbers; //il timer sarà formato da tre numeri 
    sf::Vector2f timer_pos; //posizione del timer nell'header 
    sf::Vector2f timer_size; //dimensione del timer nell'header 
    int real_timer; //il vero timer che verrà incrementato al secondo 
    float acc; //velocità di incremento del timer 
    bool isRunning; //se il timer è bloccato meno 

    Timer(sf::Vector2f header_pos, float cell_size, float pos_y, sf::Vector2f size);
    void draw (sf::RenderWindow& window);
}; 

//AGGIUNTA: struttura della faccina centro header 
struct Face
{
    sf::Vector2f face_pos; //posizione della faccina 
    sf::Vector2f face_size; //dimensione della faccina 
    sf::Texture* face_texture; //la texture iniziale sarà sempre la faccina sorridente e in base alle mosse fatte in gioco 

    //la faccina viene messa al centro dell'header e sarà un quadrato con lato lungo quanto l'altezza dei numeri dell'orologio e delle bandierine
    Face(sf::Vector2f header_pos, sf::Vector2f header_size, float cell_size, float pos_y, float size): 
                                                                                                        face_pos({(header_pos.x+ (header_size.x/2.f))- (size/2), pos_y}),
                                                                                                        face_size({size,size}), 
                                                                                                        face_texture(&smile_face_texture) {} 

    void draw (sf::RenderWindow& window);
};

//AGGIUNTA: struttura per il contatore delle bandierine nella griglia 
struct Flag_Counter
{
    vector<Number> flag_numbers; //i conteggio del bandierine sarà fatto con tre numeri come per il timer 
    sf::Vector2f flag_pos; //posizione del contatore 
    sf::Vector2f flag_size; //dimensione del contatore 
    int num_flag; //vero contatore 

    Flag_Counter(sf::Vector2f header_pos, sf::Vector2f header_size, float cell_size, float pos_y, sf::Vector2f size);
    void draw (sf::RenderWindow& window);
}; 

//AGGIUNTA: struttura contenente tutti i parametri di gioco (l'orologio, il contatore di bandierine e la faccina)
struct Header
{
    sf::Vector2f h_size; //dimensione dell'header 
    sf::Vector2f h_pos; //posizione dell'header 
    float details_pos_y; //coordinata  y della posizione dei parametri all'interno dell'header 
    sf::Vector2f details_size; //dimensione (o parte della dimensione come nel caso della faccina) dei parametri all'interno dell'header
    Timer timer; //timer della partita 
    Face face; //faccina 
    Flag_Counter f_counter; //contatore delle bandierine nella griglia

    /*nel costruttore dell'header: 
        - la posizione dell'header viene impostata come esattamente sopra la griglia 
        - la dimensione dell'header corrisponde per larghezza a quella della griglia mentre per altezza ad un quatro della griglia 
        - tutti i parametri all'interno della griglia hanno la stessa coordinata y di posizione (a distanza 1/6 dal bordo dell'header) e la stessa altezza (uguale a 2/3 della griglia)
        - l'orologio e il contatore di faccine hanno anche la stessa dimensione di larghezza 
    */
    Header(float cell_size, Grid& grid): 
                                        h_size({grid.Grid_size.x + (gap*(grid.cell_num.x-1)) - (2*header_border_gap), grid.Grid_size.y/4.f - (2*header_border_gap)}),
                                        h_pos({grid.Grid_pos.x + header_border_gap, grid.Grid_pos.y - (grid.Grid_size.y/4.f + gap)+ header_border_gap}),
                                        details_size({(h_size.x - (cell_size * 4.5f))/2, h_size.y - (h_size.y/3.f)}),
                                        details_pos_y(h_pos.y + (h_size.y/6.f)),
                                        timer(h_pos, cell_size, details_pos_y, details_size), 
                                        face(h_pos, h_size, cell_size, details_pos_y, details_size.y), 
                                        f_counter(h_pos, h_size, cell_size, details_pos_y, details_size) {}
    void draw (sf::RenderWindow& window);
};

struct Game_Panel
{
    float cell_size;
    Grid grid;  
    Header header; //AGGIUNTA: aggiunta dell'header al pannello di gioco 

    //all'header viene passato il grid e la dimensione delle celle in modo da poterne usare i parametri per il calcolo della dimensione e posizione dell'header 
    Game_Panel(sf::Vector2i cell_num, int mine_num):
                                                    cell_size(((window_height - (panel_vertical_displacement * 2)) / (cell_num.y + (cell_num.y/4.f))) * 0.85f), //aggiunto lo spazio per l'header 
                                                    grid(cell_num, mine_num, cell_size), 
                                                    header(cell_size, grid) {} 
    void draw (sf::RenderWindow& window);
};
struct Game_Stop
{ 
    sf::Text title{font}; 
    stop_type type; 
    sf::Vector2f gs_size; 
    sf::Vector2f gs_pos;
    bool visible; 
    
    Game_Stop (): 
                visible(false), 
                type(stop_type::None),
                gs_size({stop_width, stop_height}), 
                gs_pos({(window_width - stop_width)/2.f, (window_height - stop_height)/2.f}) {}
                
    void draw (sf::RenderWindow& window);
};
struct State  
{
    Game_Panel game_panel;
    Game_Stop gs; 
    int mouse_cell; 
    bool focus; 
    bool game_paused; 
    bool first_move; 
    bool game_ended;  

    State (): 
                game_panel({9,9}, 15), 
                gs(),
                focus(false), 
                game_paused(false), 
                first_move(true),
                mouse_cell(-1), 
                game_ended(false) {} 
    
    void reveal(Grid& g, int starting_index_cell); 
    void flood_reveal(Grid& g, int starting_index_cell, Cell& start_c); 
    void ending_reveal(Grid& g, int starting_index_cell); 
    void reset(); 
    void draw (sf::RenderWindow& window);
};

////////////////CREAZIONE////////////////

Grid::Grid (sf::Vector2i bs, int bn, float cell_size){
    cell_num = bs; 
    mine_num = bn; 
    num_revealed = 0; 
 
    Grid_size = {cell_size * cell_num.x, cell_size * cell_num.y};
 
    Grid_pos = { 
        window_width - Grid_size.x - panel_horizontal_displacement - (gap * cell_num.x),
        (window_height - Grid_size.y - (gap * cell_num.y) + (Grid_size.y/4.f)) / 2.0f //MODIFICA: aggiunto lo spazio per l'header
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

//AGGIUNTA: 
Timer::Timer(sf::Vector2f header_pos, float cell_size, float pos_y, sf::Vector2f size){
    real_timer = 0; //inizialmente il timer è a zero 
    acc = 0.f; //inizialmente la velocità del clock è a zero 
    isRunning = false; //il timer partirà solo alla prima mossa in partita 
    timer_size = {size}; 
    timer_pos= {header_pos.x + header_parameter_gap, pos_y}; //la posizione x corrisponde a quella dell'header più il gap 

    //creazione dei vari numeri con posizione y impostata mentre x che dipende da che numero si sta considerando. Per la dimensione di larghezza invece viene divisa la larghezza del timer per 3 
    sf::Vector2f pos;
    for(int i = 0; i<3;i++){
        pos = {
            timer_pos.x+((timer_size.x/3)*i), 
            timer_pos.y
        }; 

        timer_numbers.push_back(Number(pos, {timer_size.x/3, timer_size.y})); 
    }

}

//AGGIUNTA:
Flag_Counter::Flag_Counter(sf::Vector2f header_pos, sf::Vector2f header_size, float cell_size, float pos_y, sf::Vector2f size){
    num_flag = 0; //inizialmente il numero di bandierine nella griglia è zero 
    flag_size ={size}; 
    flag_pos =  {header_pos.x + header_size.x - (flag_size.x + header_parameter_gap), pos_y}; 

    //creazione dei vari numeri con posizione y impostata mentre x che dipende da che numero si sta considerando. Per la dimensione di larghezza invece viene divisa la larghezza del timer per 3 
    sf::Vector2f pos;
    for(int i = 0; i<3;i++){
        pos = {
            flag_pos.x+((flag_size.x/3)*i), 
            flag_pos.y
        }; 

        flag_numbers.push_back(Number(pos, {flag_size.x/3, flag_size.y})); 
    }
}

////////////////DRAW/////////////////
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


void Game_Stop::draw(sf::RenderWindow& window){
    if(!visible) return; 

    sf::RectangleShape s(gs_size); 
    s.setPosition(gs_pos);
    s.setFillColor(sf::Color(210,180,140)); 
    s.setOutlineThickness(20.f); 
    s.setOutlineColor(sf::Color(92,51,23));
    window.draw(s); 

    switch(type) 
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
    title.setCharacterSize(140); 
    title.setFillColor(sf::Color::Black); 
    title.setOutlineThickness(2.f); 
    title.setOutlineColor(sf::Color::White); 
    auto b = title.getLocalBounds(); 
    title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y}); 
    title.setPosition({s.getPosition().x + s.getSize().x/2.f, gs_pos.y + stop_gap});                 
    window.draw(title);

    title.setString("Premere SPACE");
    title.setCharacterSize(40);
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
    title.setPosition({title.getPosition().x,  title.getPosition().y + 140.f + stop_gap});
    window.draw(title);

    title.setString("per cominciare una nuova partita");
    title.setCharacterSize(40);
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
    title.setPosition({title.getPosition().x,title.getPosition().y + title.getCharacterSize() + stop_gap});
    window.draw(title);
}

//AGGIUNTA: disegno dell'header 
void Header::draw(sf::RenderWindow& window)
{
    sf::RectangleShape h(h_size); //l'header è un rettangolo 
    h.setPosition(h_pos); 
    h.setFillColor(sf::Color(192, 192, 192)); //il colore di sfondo deve essere grigio chiaro 
    h.setOutlineThickness(header_border_gap); //ha un borso di spessore 5.f
    h.setOutlineColor(sf::Color::Black); //il colore del bordo è nero 
    window.draw(h); //disegno dell'header sulla finestra 
    //successivamente al disegno dell'header ci verranno disegnati sopra tutti i parametri
    timer.draw(window);
    f_counter.draw(window);
    face.draw(window); 
}

//AGGIUNTA: disegno dei numeri 
void Number::draw(sf::RenderWindow& window)
{
    sf::RectangleShape n (num_size); //i numeri sono un rettangolo 
    n.setPosition(num_pos); 
    n.setTexture(num_texture); 
    window.draw(n);
}

//AGGIUNTA: disegno del timer
void Timer::draw(sf::RenderWindow& window)
{
    //disegno nella finestra dei vari numeri che rappresentano il timer 
    for (auto& number : timer_numbers)
        number.draw (window);
}

//AGGIUNTA: disegno del contatore di bandierine 
void Flag_Counter::draw(sf::RenderWindow& window){
    //disegno nella finestra dei vari numeri che rappresentano il contatore 
    for(auto& number : flag_numbers)
        number.draw(window);
}

//AGGIUNTA: disegno della faccina
void Face::draw(sf::RenderWindow& window){
    sf::RectangleShape f (face_size); //la faccina è un rettangolo 
    f.setPosition(face_pos); 
    f.setTexture(face_texture);
    window.draw(f);
}

void Game_Panel::draw(sf::RenderWindow& window)
{
    grid.draw(window); 
    header.draw(window); //AGGIUNTA: disegno dell'header nel pannello di gioco 
}

void State::draw (sf::RenderWindow& window){
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

void State::ending_reveal(Grid& g, int starting_index_cell){

    for(int i = 0; i < g.cells.size(); i++){

    if(gs.type == stop_type::None && i == starting_index_cell ) continue;  

            if(g.cells[i].state == cell_state::Flag && g.cells[i].type != cell_type::Mine){
                g.cells[i].state = cell_state::Revealed; 
                g.cells[i].texture = &False_Mine_texture; 
            }

            if(g.cells[i].type == cell_type::Mine){
                g.cells[i].state = cell_state::Revealed; 
                g.cells[i].texture = &Normal_Mine_texture; 
            }
    }

    game_ended = true; 
    game_paused = false;
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
    if(c.state == cell_state::Flag) game_panel.header.f_counter.num_flag --; //AGGIUNTA: aggiunta per il calcolo del numero di bandierine in partita
    c.state = cell_state::Revealed; 
    game_panel.grid.num_revealed++; 

    if(c.type == cell_type::Mine){
        c.texture = &Exploded_Mine_texture;
        gs.type = stop_type::Lose; 
        ending_reveal(g,starting_index_cell); 
        return; 
    } 
    else if(c.type == cell_type::Number){
        c.texture = &Number_textures[c.mine_adj-1]; 
    }
    else{
        c.texture = &Empty_texture; 
        flood_reveal(g, starting_index_cell, c);
    }

    if (game_panel.grid.num_revealed == static_cast<int>(g.cells.size()) - g.mine_num) {
       gs.type = stop_type::Win; 
        ending_reveal(g, starting_index_cell); 
    }
}

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
            state.game_panel.header.f_counter.num_flag ++; //AGGIUNTA: aggiunta per il calcolo del numero di bandierine in partita
            state.game_panel.grid.cells[state.mouse_cell].state = cell_state::Flag; 
            state.game_panel.grid.cells[state.mouse_cell].texture = &Flag_texture;
        }
        else{
            state.game_panel.header.f_counter.num_flag --; //AGGIUNTA: aggiunta per il calcolo del numero di bandierine in partita
            state.game_panel.grid.cells[state.mouse_cell].state = cell_state::Covered; 
            state.game_panel.grid.cells[state.mouse_cell].texture = &Covered_texture;
        }
        
    }

}

void handle(const sf::Event::KeyPressed& key, State& state) 
{
    if (state.game_ended && key.scancode == sf::Keyboard::Scancode::Space) state.reset(); 
}


void handle (const sf::Event::MouseMoved& ev, State& state)
{
    if(state.game_ended) return;
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