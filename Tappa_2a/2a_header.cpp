#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib> 
#include <ctime> 
#include <cmath>
#include <SFML/System/Clock.hpp>
#include "../risorse/textures_fonts.hpp"

using namespace std; 

////////////////FINESTRA////////////////
const char* window_title = "Header Graphics";
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

sf::Color text_color = sf::Color::Black;
sf::Color text_border_color = sf::Color::White;
const float text_thickness = 2; 

////////////////PANNELLI GENERALE////////////////

sf::Color panel_background_color = sf::Color(210,180,140);
sf::Color panel_border_color = sf::Color(92,51,23); 
sf::Color focus_color = sf::Color::Red; 
const float panel_thickness = 15;

////////////////CELLA////////////////

enum class cell_type{Mine,Empty, Number}; 
enum class cell_state{ Covered, Revealed, Flag}; 

////////////////HEADER////////////////

const float header_parameter_gap = 30; //AGGIUNTA: gap tra il bordo dell'header e i suoi parametri 
const float header_border_thickness = 5; //AGGIUNTA: spessore dell'bordo dell'header 
sf::Color header_background_color = sf::Color(192, 192, 192); //AGGIUNTA: colore dello sfondo dell'header 
sf::Color header_border_color = sf::Color::Black; //AGGIUNTA: colore del bordo dell'header 

////////////////GAME STOP////////////////

const float stop_width = (window_width/3)*2;
const float stop_height = (window_height/3)*2;
const float stop_pos_x = window_width/6; 
const float stop_pos_y = window_height/6; 
const float stop_gap = 20; 
enum class stop_type{None, Win, Lose}; 
const unsigned stop_title_size = 90; 
const unsigned stop_subtitle_size = 30;

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
    int num_revealed; 

    Grid (sf::Vector2i cell_num, int mine_num, float& cell_size, float gap);
    void place_mines(int starting_index_cell); 
    void place_numbers(); 
    void draw (sf::RenderWindow& window);
};

//AGGIUNTA: struttura per rappresentare i numeri dell'orologio e del contatore delle bandierine in partita
struct Number{
    sf::Vector2f num_pos; //posizione di ogni numero
    sf::Vector2f num_size; //grandezza di ogni numero 
    sf::Texture* num_texture; //texture di ogni numero

    Number(sf::Vector2f pos, sf::Vector2f size): 
                                                num_pos(pos), 
                                                num_size(size), 
                                                num_texture(&Clock_textures[0]) //inizialmente tutti i numeri saranno sempre a zero
                                                {}

    void draw (sf::RenderWindow& window);
    
};

//AGGIUNTA: struttura del timer di gioco 
struct Timer
{
    vector<Number> timer_numbers; //il timer sarà formato da tre numeri 
    sf::Vector2f timer_pos; //posizione del timer nell'header 
    sf::Vector2f timer_size; //dimensione del timer nell'header 
    int real_timer; //il vero timer che verrà incrementato ogni secondo
    float acc; //velocità di incremento del timer 
    bool isRunning; //se il timer sta andando o meno

    Timer(sf::Vector2f header_pos, float cell_size, float pos_y, sf::Vector2f size);
    void draw (sf::RenderWindow& window);
}; 

//AGGIUNTA: struttura della faccina nel centro dell'header 
struct Face
{
    sf::Vector2f face_pos; //posizione della faccina nell'header 
    sf::Vector2f face_size; //dimensione della faccina nell'header 
    sf::Texture* face_texture; //texture della faccina (varierà in base alla azioni compiute in partita)

    //la faccina viene messa al centro dell'header e sarà un quadrato con lato lungo quanto l'altezza dei numeri dell'orologio e delle bandierine
    Face(sf::Vector2f header_pos, sf::Vector2f header_size, float cell_size, float pos_y, float size): 
                                                                                                        face_pos({(header_pos.x+ (header_size.x/2))- (size/2), pos_y}),
                                                                                                        face_size({size,size}), 
                                                                                                        face_texture(&smile_face_texture) //la texture iniziale sarà sempre la faccina sorridente e in base alle mosse fatte in gioco 
                                                                                                        {} 

    void draw (sf::RenderWindow& window);
};

//AGGIUNTA: struttura per il contatore delle bandierine nella griglia 
struct Flag_Counter
{
    vector<Number> flag_numbers; //il contatore delle bandierine sarà formato da tre numeri 
    sf::Vector2f flag_pos; //posizione del contatore nell'header
    sf::Vector2f flag_size; //dimensione del contatore nell'header 
    int num_flag; //variabile che terrà il conto delle bandierine ancora disponibili rispetto alle mine

    Flag_Counter(sf::Vector2f header_pos, sf::Vector2f header_size, float cell_size, float pos_y, sf::Vector2f size);
    void draw (sf::RenderWindow& window);
}; 

//AGGIUNTA: struttura contenente tutti i parametri di gioco (l'orologio, il contatore di bandierine e la faccina)
struct Header
{
    sf::Vector2f h_size; //dimensione dell'header 
    sf::Vector2f h_pos; //posizione dell'header 
    float details_pos_y; //coordinata y della posizione dei parametri all'interno dell'header 
    sf::Vector2f details_size; //dimensione (o parte della dimensione come nel caso della faccina) dei parametri all'interno dell'header
    Timer timer; //timer della partita 
    Face face; //faccina 
    Flag_Counter f_counter; //contatore delle bandierine

    /*nel costruttore dell'header: 
        - la dimensione dell'header corrisponde per larghezza a quella della griglia mentre l'altezza mentre per altezza ad un quatro della griglia (per ora)
        - la posizione dell'header viene impostata come esattamente sopra la griglia
        - tutti i parametri all'interno della griglia hanno la stessa coordinata y di posizione (a distanza 1/6 dal bordo dell'header) e la stessa altezza (uguale a 2/3 della griglia)
        - l'orologio e il contatore di faccine hanno anche la stessa dimensione di larghezza 
    */
    Header(float cell_size, Grid& grid): 
                                        h_size({ grid.Grid_size.x - (header_border_thickness*2), grid.Grid_size.y/4.f - (2*header_border_thickness)}), 
                                        h_pos({ grid.Grid_pos.x + header_border_thickness, grid.Grid_pos.y - (grid.Grid_size.y/4 + 2) + header_border_thickness }), 
                                        details_pos_y(h_pos.y + (h_size.y/6)),
                                        details_size({(h_size.y - (h_size.y/3))*3/2, h_size.y - (h_size.y/3)}),
                                        timer(h_pos, cell_size, details_pos_y, details_size), 
                                        face(h_pos, h_size, cell_size, details_pos_y, details_size.y), 
                                        f_counter(h_pos, h_size, cell_size, details_pos_y, details_size) {}
    void draw (sf::RenderWindow& window);
};

struct Game_Panel
{
    float cell_size; 
    float gap; 
    Grid grid;
    Header header; //AGGIUNTA: aggiunta dell'header al pannello di gioco 

    Game_Panel(sf::Vector2i cell_num, int mine_num):
                                        cell_size((window_height - (window_vertical_displacement * 2)) / (cell_num.y + (cell_num.y/4.f))), //aggiunto lo spazio per l'header 
                                        gap(2),
                                        grid(cell_num, mine_num, cell_size, gap),
                                        header(cell_size, grid)
                                        {} 
    void draw (sf::RenderWindow& window);
};

struct Stop_Panel{ 
    sf::Text title{font}; 
    stop_type stop_type; 
    sf::Vector2f stop_size;
    sf::Vector2f stop_pos;
    bool visible;
    
    Stop_Panel (): 
                visible(false),
                stop_type(stop_type::None), 
                stop_size({stop_width, stop_height}), 
                stop_pos({stop_pos_x,stop_pos_y}) {}

    void draw (sf::RenderWindow& window);
}; 

struct State 
{
    Game_Panel game_panel;
    Stop_Panel stop_p; 
    int mouse_cell;
    bool focus; 
    bool first_move; 
    bool game_ended;

    State () : 
            game_panel({9,9}, 15),
            stop_p(),
            focus(false), 
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

Grid::Grid (sf::Vector2i bs, int bn, float& cell_size, float gap){
    cell_num = bs; 
    mine_num = bn;
    num_revealed = 0; 
    Grid_size = {
        (cell_size * cell_num.x) + (gap * (cell_num.x - 1)),
        (cell_size * cell_num.y) + (gap * (cell_num.y - 1))
    };

    //MODIFICATO: viene contato nella posizione che la fnestra deve contenere anche l'header.
    Grid_pos = { 
        window_width - Grid_size.x - window_horizontal_displacement,
        (window_height - Grid_size.y - cell_num.y + (Grid_size.y/4.f) -gap) / 2
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

//AGGIUNTA: costruttore del timer di gioco 
Timer::Timer(sf::Vector2f header_pos, float cell_size, float pos_y, sf::Vector2f size){
    real_timer = 0; //inizialmente il timer è a zero 
    acc = 0; //inizialmente la velocità del clock è a zero 
    isRunning = false; //il timer partirà solo dopo che è stata fatta la prima mossa dell partita 
    timer_size = {size}; //impostazione della dimensione
    timer_pos= {header_pos.x + header_parameter_gap, pos_y}; //la coordinata x corrisponde a quella dell'header più il gap tra header e parametri

    //creazione dei vari numeri con coordinata y impostata mentre x che dipende da che numero si sta considerando. Per la larghezza dei vari numeri invece viene divisa la larghezza del timer per 3 
    sf::Vector2f pos;
    for(int i = 0; i<3;i++){
        pos = { timer_pos.x+((timer_size.x/3)*i), timer_pos.y}; 
        timer_numbers.push_back(Number(pos, {timer_size.x/3, timer_size.y})); 
    }

}

//AGGIUNTA: costruttore del contatore di bandierine
Flag_Counter::Flag_Counter(sf::Vector2f header_pos, sf::Vector2f header_size, float cell_size, float pos_y, sf::Vector2f size){
    num_flag = 0; //inizialmente il numero di bandierine nella griglia è uguale al numero di mine nella griglia
    flag_size ={size}; 
    flag_pos =  {header_pos.x + header_size.x - (flag_size.x + header_parameter_gap), pos_y}; //la posizione nell'header del contatore è opposta orizzontalmente rispetto a quella del timer

    //creazione dei vari numeri con coordinata y impostata mentre x che dipende da che numero si sta considerando. Per la larghezza dei vari numeri invece viene divisa la larghezza del contatore per 3 
    sf::Vector2f pos;
    for(int i = 0; i<3;i++){
        pos = { flag_pos.x+((flag_size.x/3)*i), flag_pos.y}; 
        flag_numbers.push_back(Number(pos, {flag_size.x/3, flag_size.y})); 
    }

    //impostazione del valore iniziale dei numeri 
    flag_numbers[2].num_texture = &Clock_textures[num_flag%10];
    flag_numbers[1].num_texture = &Clock_textures[(num_flag/10)%10];
    flag_numbers[0].num_texture = &Clock_textures[(num_flag/100)%10];
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

//AGGIUNTA:
void Number::draw(sf::RenderWindow& window)
{
    sf::RectangleShape n (num_size); //i numeri sono un rettangolo della dimensione impostata
    n.setPosition(num_pos); 
    n.setTexture(num_texture); 
    window.draw(n);
}

//AGGIUNTA:
void Timer::draw(sf::RenderWindow& window)
{
    //richiamo del draw di ogni numero che rappresenta il timer
    for (auto& number : timer_numbers)
        number.draw (window);
}

//AGGIUNTA: 
void Flag_Counter::draw(sf::RenderWindow& window){
    //richiamo del draw di ogni numero che rappresenta il contatore
    for(auto& number : flag_numbers)
        number.draw(window);
}

//AGGIUNTA:
void Face::draw(sf::RenderWindow& window){
    sf::RectangleShape f (face_size); //la faccina è un rettangolo della dimensione impostata 
    f.setPosition(face_pos); 
    f.setTexture(face_texture);
    window.draw(f);
}

//AGGIUNTA
void Header::draw(sf::RenderWindow& window)
{
    sf::RectangleShape h(h_size); //l'header è un rettangolo della dimensione impostata
    h.setPosition(h_pos); 
    h.setFillColor(header_background_color); //impostazione del colore di sfondo
    h.setOutlineThickness(header_border_thickness); //impostazione del bordo
    h.setOutlineColor(header_border_color); //impostazione del colore del bordo 
    window.draw(h); //disegno dell'header sulla finestra 
    //successivamente al disegno dell'header ci verranno disegnati sopra tutti i parametri
    timer.draw(window);
    f_counter.draw(window);
    face.draw(window); 
}

void Game_Panel::draw(sf::RenderWindow& window)
{
    header.draw(window); //AGGIUNTA: disegno dell'header nel pannello di gioco 
    grid.draw(window); 
}

void Stop_Panel::draw(sf::RenderWindow& window){
    if(!visible) return; 

    sf::RectangleShape s(stop_size); 
    s.setPosition(stop_pos);
    s.setFillColor(panel_background_color); 
    s.setOutlineThickness(panel_thickness); 
    s.setOutlineColor(panel_border_color); 
    window.draw(s); 
    float stop_text_pos_x = stop_pos.x + stop_size.x/2;

    switch(stop_type)
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
    title.setCharacterSize(stop_title_size); 
    title.setFillColor(text_color);
    title.setOutlineThickness(text_thickness);
    title.setOutlineColor(text_border_color);
    auto b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x/2, b.position.y}); 
    title.setPosition({stop_text_pos_x, stop_pos.y + stop_gap});
    window.draw(title);

    title.setString("Premere SPACE");
    title.setCharacterSize(stop_subtitle_size);
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x/2, b.position.y});
        title.setPosition({stop_text_pos_x,title.getPosition().y + stop_title_size + stop_gap});  
    window.draw(title);

    title.setString("per cominciare una nuova");
    title.setCharacterSize(stop_subtitle_size);
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x/2, b.position.y});
        title.setPosition({stop_text_pos_x,title.getPosition().y + stop_subtitle_size + stop_gap});  
    window.draw(title);

    title.setString("PARTITA");
    title.setCharacterSize(stop_subtitle_size);
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x/2, b.position.y});
        title.setPosition({stop_text_pos_x,title.getPosition().y + stop_subtitle_size + stop_gap});  
    window.draw(title);
}

void State::draw (sf::RenderWindow& window){
    game_panel.draw (window); 
    stop_p.draw(window);
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
    game_panel.grid.num_revealed++;

    if(c.cell_type == cell_type::Mine){
        c.cell_texture = &Exploded_Mine_texture;
        stop_p.stop_type = stop_type::Lose;
        ending_reveal(g,starting_index_cell); 
        return; 
    } 
    else if(c.cell_type == cell_type::Number){
        c.cell_texture = &Number_textures[c.mine_adj-1];
    }
    else{
        c.cell_texture = &Empty_texture; 
        flood_reveal(g, starting_index_cell, c);
    }

    if (game_panel.grid.num_revealed == static_cast<int>(g.cells.size()) - g.mine_num) {
        stop_p.stop_type = stop_type::Win;
        ending_reveal(g, starting_index_cell);
    }
}

void State::ending_reveal(Grid& g, int starting_index_cell){
    for(int i = 0; i < g.cells.size(); i++){

        if(stop_p.stop_type == stop_type::Lose && i == starting_index_cell) continue;

        if(g.cells[i].cell_state == cell_state::Flag && g.cells[i].cell_type != cell_type::Mine){
            g.cells[i].cell_state = cell_state::Revealed; 
            g.cells[i].cell_texture = &False_Mine_texture; 
        }

        if(g.cells[i].cell_type == cell_type::Mine){
            g.cells[i].cell_state = cell_state::Revealed; 
            g.cells[i].cell_texture = &Normal_Mine_texture; 
        }
    }

    game_ended = true;
    stop_p.visible = true; 
    
}

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
    if(state.game_ended) return;

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

void handle(const sf::Event::KeyPressed& key, State& state) {
    if (state.game_ended && key.scancode == sf::Keyboard::Scancode::Space) state.reset();
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