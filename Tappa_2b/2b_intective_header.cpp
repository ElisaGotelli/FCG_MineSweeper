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
const char* window_title = "Interactive Header";
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

const float header_parameter_gap = 30;
const float header_border_thickness = 5; 
sf::Color header_background_color = sf::Color(192, 192, 192); 
sf::Color header_border_color = sf::Color::Black;

////////////////GAME STOP////////////////

const float stop_width = (window_width/3)*2;
const float stop_height = (window_height/3)*2;
const float stop_pos_x = window_width/6; 
const float stop_pos_y = window_height/6; 
const float stop_gap = 20; 
enum class stop_type{None, Win, Lose}; 
const unsigned stop_title_size = 90; 
const unsigned stop_subtitle_size = 30;
const unsigned stop_time_text_size = 15;
sf::Color stop_time_text_color = sf::Color::Red;

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

struct Number{
    sf::Vector2f num_pos;
    sf::Vector2f num_size;
    sf::Texture* num_texture;

    Number(sf::Vector2f pos, sf::Vector2f size): 
                                                num_pos(pos), 
                                                num_size(size), 
                                                num_texture(&Clock_textures[0]) {}

    void draw (sf::RenderWindow& window);
    
};

struct Timer
{
    vector<Number> timer_numbers;
    sf::Vector2f timer_pos;
    sf::Vector2f timer_size;
    int real_timer;
    float acc;
    bool isRunning;

    Timer(sf::Vector2f header_pos, float cell_size, float pos_y, sf::Vector2f size);
    void update(float elapsed);  //AGGIUNTA: funzione per l'aggiornamento del timer durante la partita
    void draw (sf::RenderWindow& window);
}; 

struct Face
{
    sf::Vector2f face_pos; 
    sf::Vector2f face_size; 
    sf::Texture* face_texture;

    Face(sf::Vector2f header_pos, sf::Vector2f header_size, float cell_size, float pos_y, float size): 
                                                                                                        face_pos({(header_pos.x+ (header_size.x/2))- (size/2), pos_y}),
                                                                                                        face_size({size,size}), 
                                                                                                        face_texture(&smile_face_texture)
                                                                                                        {} 

    void draw (sf::RenderWindow& window);
};

struct Flag_Counter
{
    vector<Number> flag_numbers; 
    sf::Vector2f flag_pos; 
    sf::Vector2f flag_size; 
    int num_flag;

    Flag_Counter(sf::Vector2f header_pos, sf::Vector2f header_size, float cell_size, float pos_y, sf::Vector2f size, int mine_num); //MODIFICATO: aggiunto il numero di mine per l'impostazione corretta dei numeri
    void set_number(bool flag_placed); //AGGIUNTA: reso visibile e interattivo (si aggiorna in automatico al click destro) il contatore delle bandierine 
    void draw (sf::RenderWindow& window);
}; 

struct Header
{
    sf::Vector2f h_size;
    sf::Vector2f h_pos;
    float details_pos_y;
    sf::Vector2f details_size;
    Timer timer;
    Face face;
    Flag_Counter f_counter;

    //MODIFICATO: aggiunto il numero di mine come parametro in modo da poterlo usare per il flag_counter
    Header(float cell_size, Grid& grid, int mine_num): 
                                        h_size({ grid.Grid_size.x - (header_border_thickness*2), grid.Grid_size.y/4.f - (2*header_border_thickness)}), 
                                        h_pos({ grid.Grid_pos.x + header_border_thickness, grid.Grid_pos.y - (grid.Grid_size.y/4) + header_border_thickness }), 
                                        details_pos_y(h_pos.y + (h_size.y/6)),
                                        details_size({(h_size.y - (h_size.y/3))*3/2, h_size.y - (h_size.y/3)}),
                                        timer(h_pos, cell_size, details_pos_y, details_size), 
                                        face(h_pos, h_size, cell_size, details_pos_y, details_size.y), 
                                        f_counter(h_pos, h_size, cell_size, details_pos_y, details_size, mine_num) {}
    void draw (sf::RenderWindow& window);
};

struct Game_Panel
{
        float gap; 
    float cell_size; 
    Grid grid;
    Header header;

    Game_Panel(sf::Vector2i cell_num, int mine_num):
                                        gap(2),
                                        cell_size((window_height - (window_vertical_displacement * 2) - (cell_num.y/4.f) - (gap * (cell_num.x - 1))) / cell_num.y),
                                        grid(cell_num, mine_num, cell_size, gap),
                                        header(cell_size, grid, mine_num)
                                        {} 
    void draw (sf::RenderWindow& window);
};

struct Stop_Panel{ 
    sf::Text title{font}; 
    stop_type stop_type; 
    sf::Vector2f stop_size;
    sf::Vector2f stop_pos;
    bool visible;
    int time; //AGGIUNTA: durata della partita
    
    Stop_Panel (): 
                visible(false),
                stop_type(stop_type::None), 
                stop_size({stop_width, stop_height}), 
                stop_pos({stop_pos_x,stop_pos_y}),
                time(0) {} //AGGIUNTA: all'inizio la durata della partita sarà sempre zero

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

    float header_height = Grid_size.y/4;

    Grid_pos = { 
        window_width - Grid_size.x - window_horizontal_displacement,
        (window_height - Grid_size.y - header_height)/2 + header_height 
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

Timer::Timer(sf::Vector2f header_pos, float cell_size, float pos_y, sf::Vector2f size){
    real_timer = 0; 
    acc = 0;
    isRunning = false;
    timer_size = {size};
    timer_pos= {header_pos.x + header_parameter_gap, pos_y};

    sf::Vector2f pos;
    for(int i = 0; i<3;i++){
        pos = { timer_pos.x+((timer_size.x/3)*i), timer_pos.y}; 
        timer_numbers.push_back(Number(pos, {timer_size.x/3, timer_size.y})); 
    }

}

Flag_Counter::Flag_Counter(sf::Vector2f header_pos, sf::Vector2f header_size, float cell_size, float pos_y, sf::Vector2f size, int mine_num){
    num_flag = mine_num;
    flag_size ={size}; 
    flag_pos =  {header_pos.x + header_size.x - (flag_size.x + header_parameter_gap), pos_y};

    sf::Vector2f pos;
    for(int i = 0; i<3;i++){
        pos = { flag_pos.x+((flag_size.x/3)*i), flag_pos.y}; 
        flag_numbers.push_back(Number(pos, {flag_size.x/3, flag_size.y})); 
    }

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

void Number::draw(sf::RenderWindow& window)
{
    sf::RectangleShape n (num_size); 
    n.setPosition(num_pos); 
    n.setTexture(num_texture); 
    window.draw(n);
}

void Timer::draw(sf::RenderWindow& window)
{
    for (auto& number : timer_numbers)
        number.draw (window);
}

void Flag_Counter::draw(sf::RenderWindow& window)
{
    for(auto& number : flag_numbers)
        number.draw(window);
}

void Face::draw(sf::RenderWindow& window)
{
    sf::RectangleShape f (face_size); 
    f.setPosition(face_pos); 
    f.setTexture(face_texture);
    window.draw(f);
}

void Header::draw(sf::RenderWindow& window)
{
    sf::RectangleShape h(h_size);
    h.setPosition(h_pos); 
    h.setFillColor(header_background_color); 
    h.setOutlineThickness(header_border_thickness);
    h.setOutlineColor(header_border_color);
    window.draw(h);
    timer.draw(window);
    f_counter.draw(window);
    face.draw(window); 
}

void Game_Panel::draw(sf::RenderWindow& window)
{
    header.draw(window);
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

    //AGGIUNTA: sotto 'Hai vinto/perso' ora verrà scritta la durata della partita in ore, minuti e secondi
    title.setString("Tempo impiegato fino ad ora: "+ to_string(time/3600) + (time/3600 == 1? " ora " : " ore ") + to_string((time%3600)/60) + ((time%3600)/60 == 1? " minuto " : " minuti ") + to_string((time%3600)%60) + ((time%3600)%60 == 1? " secondo " : " secondi "));
    title.setCharacterSize(stop_time_text_size); //sarà scritto leggermente più piccolo del resto
    title.setFillColor(stop_time_text_color); //impostazione di un colore diverso rispetto alle altre scritte
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x/2, b.position.y});
    title.setPosition({stop_text_pos_x,  title.getPosition().y + stop_title_size + stop_gap}); 
    window.draw(title);

    title.setString("Premere SPACE");
    title.setCharacterSize(stop_subtitle_size);
    title.setFillColor(text_color);  //AGGIUNTA: ritorno al colore nero per tutte le scritte a parte la durata della partita
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x/2, b.position.y});
        title.setPosition({stop_text_pos_x,title.getPosition().y + stop_time_text_size + stop_gap});  
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

//AGGIUNTA: funzione per rendere visibile e interattivo (si aggiorna in automatico al click destro) il contatore di bandierine
void Flag_Counter::set_number(bool flag_placed){ 
    //viene utilizzato un valore booleano per capire se vi è stata piazzata o tolta una bandierina
    if(flag_placed) num_flag --; 
    else num_flag ++;
    if(num_flag < 0) return; //il contatore non è stato implementato per rappresentare numeri negativi
    
    //aggiornamento dei valori del contatore
    flag_numbers[2].num_texture = &Clock_textures[num_flag%10];
    flag_numbers[1].num_texture = &Clock_textures[(num_flag/10)%10];
    flag_numbers[0].num_texture = &Clock_textures[(num_flag/100)%10];
}

//AGGIUNTA: funzione per l'aggiornamento del timer della partita in corso 
void Timer::update(float elapsed){
    if(!isRunning) return; //se la partita è finita o non è ancora iniziata il timer non è attivo
    acc += elapsed; //accumulo il tempo trascorso dall’ultimo frame
    while(acc >= 1){ //quando è trascorso almeno un secondo
        acc -= 1; //tolgo un secondo dall’accumulatore
        real_timer ++; //incremento il vero timer
        if(real_timer >= 999) return; //dopo 999 non vi sono abbastanza cifre per rappresentare il timer correttamente

        //aggiornamento del valore del timer
        timer_numbers[2].num_texture = &Clock_textures[real_timer%10];
        timer_numbers[1].num_texture = &Clock_textures[(real_timer/10)%10]; 
        timer_numbers[0].num_texture = &Clock_textures[(real_timer/100)%10];
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
    if(c.cell_state == cell_state::Flag) game_panel.header.f_counter.set_number(false); //AGGIUNTA: se è stata rivelata una cella che conteneva una bandierina si chiama la funzione per mantenimento corretto del contatore di bandierine
    c.cell_state = cell_state::Revealed;
    game_panel.grid.num_revealed++;

    if(c.cell_type == cell_type::Mine){
        c.cell_texture = &Exploded_Mine_texture;
        stop_p.stop_type = stop_type::Lose;
        ending_reveal(g,starting_index_cell); 
        game_panel.header.face.face_texture = &lost_face_texture; //AGGIUNTA: modifica della texture della faccina 
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
        game_panel.header.face.face_texture = &win_face_texture; //AGGIUNTA: modifica della texture della faccina
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
    game_panel.header.timer.isRunning = false; //AGGIUNTA: la partita è finita quindi il timer viene stoppato
    stop_p.time = game_panel.header.timer.real_timer; //AGGIUNTA:imposta il tempo di fine partita come quello corrente del timer
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

    //AGGIUNTA: imposta che il timer riparte solo nel caso in cui la partita è in corso
    if (!state.first_move && !state.game_ended) 
        state.game_panel.header.timer.isRunning = true;
}

void handle (const sf::Event::FocusLost&, State& state)
{
    state.focus = false;

    //AGGIUNTA: se vi era una partita in corso il timer viene bloccato
    if(!state.first_move && !state.game_ended) 
        state.game_panel.header.timer.isRunning = false;
}

void handle (const sf::Event::MouseButtonPressed& mouse, State& state)
{
    if(state.game_ended) return;

    if(state.mouse_cell <0 || state.mouse_cell >= state.game_panel.grid.cells.size()) return; 

    Cell& cur_cell = state.game_panel.grid.cells[state.mouse_cell]; 

    if(cur_cell.cell_state == cell_state::Revealed) return; 

    if( mouse.button == sf::Mouse::Button::Left){

        if(state.first_move){
            state.first_move = false;
            state.game_panel.grid.place_mines(state.mouse_cell);
            state.game_panel.grid.place_numbers();
            state.reveal(state.game_panel.grid, state.mouse_cell);
            state.game_panel.header.timer.isRunning = true; //AGGIUNTA: il timer parte solo dopo che è stata fatta la prima mossa
        }
        else{
            state.game_panel.header.face.face_texture = &Click_face_texture; //AGGIUNTA: ad ogni click sinistro la faccina in gioco diventerà sorpresa 
            state.reveal(state.game_panel.grid, state.mouse_cell);
        }
    }

    if(mouse.button == sf::Mouse::Button::Right){
        if(state.first_move) return;

        if(cur_cell.cell_state != cell_state::Flag){
            state.game_panel.header.f_counter.set_number(true); //AGGIUNTA:si chiama la funzione per mantenimento corretto del contatore di bandierine
            cur_cell.cell_state = cell_state::Flag; 
            cur_cell.cell_texture = &Flag_texture;
        }
        else{
            state.game_panel.header.f_counter.set_number(false); //AGGIUNTA:si chiama la funzione per mantenimento corretto del contatore di bandierine
            cur_cell.cell_state = cell_state::Covered; 
            cur_cell.cell_texture = &Covered_texture;
        }
    }
}

//AGGIUNTA: gestione dell'evento caratterizzato dal rilascio del click del mouse per reimpostare la faccina con la texture di default
void handle (const sf::Event::MouseButtonReleased& mouse, State& state)
{
    if(state.game_ended || state.game_panel.header.face.face_texture != &Click_face_texture) return; //non fare nulla se la partita è terminata o la faccina non è con la texture del click
    state.game_panel.header.face.face_texture = &smile_face_texture; 
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
    sf::Clock Clock; //AGGIUNTA: clock interno a SFML che misura il tempo trascorso tra un frame e l'altro
    float elapsed; 

    while (window.isOpen())
    {
        window.handleEvents (
                             [&window](const sf::Event::Closed&) { handle_close (window); },
                            [&window](const sf::Event::Resized& event) { handle_resize (event, window); }, 
                            [&window, &state](const sf::Event::MouseMoved& event) {handle_mouse_moved (event, window, state); },
                            [&state] (const auto& event) { handle (event, state); }
        );

        elapsed = Clock.restart().asSeconds(); //AGGIUNTA: calcola i secondi trascorsi dall'ultimo frame
        state.game_panel.header.timer.update(elapsed); //AGGIUNTA: aggiornamento del timer

        window.clear(window_background_color);
        state.draw(window); 
        window.draw(border); 
        window.display();
    }
}