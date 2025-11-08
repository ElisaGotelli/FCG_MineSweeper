#include <SFML/Graphics.hpp>
#include <vector> 
#include <string>
#include <cstdlib> 
#include <ctime> 
#include <cmath>
#include <SFML/System/Clock.hpp>
#include "../textures_fonts.hpp"

using namespace std; 

////////////////FINESTRA////////////////
const char* window_title = "Rules and Title";
const unsigned window_width = 1200; 
const unsigned window_height = 900;
const float max_frame_rate = 60;

////////////////STATO////////////////

enum class Difficulty{easy, medium,hard}; 

////////////////SCHERMATA INIZIALE////////////////

const unsigned start_width = (window_width/3.f)*2.f;
const unsigned start_heigth = (window_height/3.f)*2.f;
const unsigned start_pos_x = window_width/6.f; 
const unsigned start_pos_y = window_height/6.f;
const float start_gap = 30.f;

////////////////PANNELLO DI GIOCO////////////////

const float panel_horizontal_displacement = 100;
const float panel_vertical_displacement = 100;
const float gap = 2.f;
const float start_cell_size = (((window_width-(panel_horizontal_displacement*2))/2.f)/9.f)*0.85; 

////////////////GAME STOP////////////////

const float stop_gap = 15.f; 
enum class stop_type{None, Win, Lose, Pause, New_Game}; 
const unsigned stop_height = window_height/2.f; 
const unsigned stop_width = window_width/2.f; 
const unsigned std_stop_button_width = stop_width/5.f; 
const unsigned std_stop_button_heigth = stop_height/7.5f; 

////////////////HEADER////////////////

const float header_parameter_gap = 30.f; 
const float header_border_gap = 5.f;

////////////////BLOCCO////////////////

enum class cell_type{Mine,Empty, Number}; 
enum class cell_state{ Covered, Revealed, Flag}; 

////////////////PULSANTE DI CONTROLLO////////////////

enum class button_type{new_game, pause, easy, medium, hard, exit}; 

////////////////PANNELLO DI CONTROLLO////////////////

const float control_horizontal_displacement = 30; 
const float control_vertical_displacement = 30; 
const float control_gap = 10.f; //EX MODIFICA: gap tra le scritte e i pulsanti 
const float info_size = 20.f; //AGGIUNTA: dimensione del testo nel control panel 

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
    
    Cell (sf::Vector2f pos, float size, int row_index, int column_index) :  pos (pos),
                                                                            size (size),
                                                                            row_index(row_index),
                                                                            column_index(column_index),
                                                                            bounds (pos, {size, size}),mouse_focus(false),
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
    void update(float elapsed); 
    void draw (sf::RenderWindow& window);
}; 

struct Face
{
    sf::Vector2f face_pos; 
    sf::Vector2f face_size; 
    sf::Texture* face_texture; 

    Face(sf::Vector2f header_pos, sf::Vector2f header_size, float cell_size, float pos_y, float size): 
                                                                                                        face_pos({(header_pos.x+ (header_size.x/2.f))- (size/2), pos_y}),
                                                                                                        face_size({size,size}), 
                                                                                                        face_texture(&smile_face_texture) {} 

    void draw (sf::RenderWindow& window);
};

struct Flag_Counter
{
    vector<Number> flag_numbers; 
    sf::Vector2f flag_pos; 
    sf::Vector2f flag_size; 
    int num_flag; 

    Flag_Counter(sf::Vector2f header_pos, sf::Vector2f header_size, float cell_size, float pos_y, sf::Vector2f size);
    void set_number(bool adding); 
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

    Header(Grid& grid): //EX MODIFICA 
                        h_size({ grid.Grid_size.x + (gap*(grid.cell_num.x-1)) - (2*header_border_gap), (start_cell_size * 9) / 4.f - (2*header_border_gap) }), h_pos({ grid.Grid_pos.x + header_border_gap, grid.Grid_pos.y - ((start_cell_size * 9) / 4.f + gap) + header_border_gap }),
                        details_size({(h_size.y - (h_size.y/3.f))*3.f/2.f, h_size.y - (h_size.y/3.f)}), 
                        details_pos_y(h_pos.y + (h_size.y/6.f)),
                        timer(h_pos, start_cell_size, details_pos_y, details_size), 
                        face(h_pos, h_size, start_cell_size, details_pos_y, details_size.y), 
                        f_counter(h_pos, h_size, start_cell_size, details_pos_y, details_size) {} 
    void draw (sf::RenderWindow& window);
};


struct Border_Cell
{
    sf::Vector2f border_cell_pos; 
    sf::Vector2f border_cell_size;  
    sf::Texture* border_cell_texture; 

    Border_Cell(sf::Vector2f pos, sf::Vector2f size, sf::Texture* texture): 
                                                                            border_cell_pos(pos),
                                                                            border_cell_size(size), 
                                                                            border_cell_texture(texture) {}
    void draw (sf::RenderWindow& window);
};


struct Border
{ 
    vector<Border_Cell> angle_cells;
    vector<Border_Cell> side_cells; 

    sf::Vector2f b_size;
    sf::Vector2f b_pos;
    float thickness; 

    Border(float cell_size, Grid& grid, Header& header); 
    void draw (sf::RenderWindow& window);
}; 

struct Game_Panel
{
    float cell_size;
    Grid grid;  
    Header header;
    Border border;  

    Game_Panel(sf::Vector2i cell_num, int mine_num):
                                                    cell_size((((window_width-(panel_horizontal_displacement*2))/2.f)/(cell_num.x+1))*0.85), 
                                                    grid(cell_num, mine_num, cell_size), 
                                                    header(grid), 
                                                    border(cell_size, grid, header) {} 
    void draw (sf::RenderWindow& window);
}; 

struct Control_Button
{
    sf::Vector2f cb_pos; 
    sf::Vector2f cb_size; 
    sf::Text cb_text; 
    sf::FloatRect cb_bounds; 
    button_type cb_type;
    //tolto: bool clicked;
    bool mouse_focus; 

   Control_Button(button_type type, sf::Vector2f pos, sf::Vector2f size) : 
                                                                            cb_pos(pos), 
                                                                            cb_size(size), 
                                                                            cb_text{font},
                                                                            cb_bounds(cb_pos, cb_size), 
                                                                            cb_type(type),
                                                                            mouse_focus(false) {} 
    void draw (sf::RenderWindow& window);
}; 

struct Control_Panel 
{
    sf::Vector2f cp_pos; 
    sf::Vector2f cp_size;  
    sf::Vector2f button_size; 
    Control_Button pause, new_game, exit; 
    sf::Text info{font}; //AGGIUNTA 
    int info_mine; //AGGIUNTA
    Difficulty info_diff; //AGGIUNTA

    Control_Panel(Border border, int num_mines, Difficulty diff): 
                                    cp_pos(panel_horizontal_displacement, border.b_pos.y + border.thickness), 
                                    cp_size({border.b_size.x -(border.thickness *2.f), border.b_size.y -(border.thickness *2.f)}), 
                                    button_size({(cp_size.x-(control_vertical_displacement*2))/3.f, (cp_size.x-(control_vertical_displacement*2.f))/6.f}),  
                                    new_game(button_type::new_game, {cp_pos.x + control_horizontal_displacement, cp_pos.y + control_vertical_displacement}, button_size) ,
                                    pause(button_type::pause, {cp_pos.x +cp_size.x - control_horizontal_displacement - button_size.x, cp_pos.y + control_vertical_displacement}, button_size), 
                                    exit(button_type::exit, {cp_pos.x +cp_size.x/2.f - button_size.x/2.f, pause.cb_pos.y + pause.cb_size.y + control_vertical_displacement + control_gap/2.f}, button_size), //EX MODIFICA
                                    info_mine(num_mines), //AGGIUNTA 
                                    info_diff(diff) //AGGIUNTA
                                    {}

    void draw (sf::RenderWindow& window);
};

struct Game_Stop 
{ 
    sf::Text title{font}; 
    stop_type type; 
    sf::Vector2f gs_size; 
    sf::Vector2f gs_pos;
    bool visible; 
    int time;
    Control_Button new_game_cb, easy_cb, medium_cb, hard_cb, exit_cb; 
    
    Game_Stop ():
                visible(false), 
                type(stop_type::None), 
                gs_size({stop_width, stop_height}), 
                gs_pos({(window_width - stop_width)/2.f, (window_height - stop_height)/2.f}), 
                time(0), 
                new_game_cb(button_type::new_game,{gs_pos.x + gs_size.x/3.f - std_stop_button_width/2.f, gs_pos.y + gs_size.y - stop_gap - std_stop_button_heigth}, {std_stop_button_width, std_stop_button_heigth}), 
                easy_cb(button_type::easy, {gs_pos.x + stop_gap*2.f, gs_pos.y + gs_size.y*3.f/4.f -std_stop_button_heigth/2.f}, {std_stop_button_width, std_stop_button_heigth}), 
                medium_cb(button_type::medium, {gs_pos.x+ gs_size.x/2.f - std_stop_button_width/2.f, gs_pos.y + gs_size.y*3.f/4.f -std_stop_button_heigth/2.f}, {std_stop_button_width, std_stop_button_heigth}), 
                hard_cb(button_type::hard, {gs_pos.x+ gs_size.x - stop_gap*2.f - std_stop_button_width, gs_pos.y + gs_size.y*3.f/4.f -std_stop_button_heigth/2.f}, {std_stop_button_width, std_stop_button_heigth}), 
                exit_cb(button_type::exit, {gs_pos.x + gs_size.x*2.f/3.f - std_stop_button_width/2.f, gs_pos.y + gs_size.y - stop_gap - std_stop_button_heigth}, {std_stop_button_width, std_stop_button_heigth}) 
                {} 
                
    void draw (sf::RenderWindow& window);
}; 

struct Start_Panel
{
    sf::Vector2f start_size; 
    sf::Vector2f start_pos; 
    sf::Text start_title{font}; 
    sf::Vector2f start_cb_size;
    Control_Button easy_cb, medium_cb, hard_cb; 
    bool visible; 

    Start_Panel (): 
                    start_size({start_width, start_heigth}), 
                    start_pos({start_pos_x, start_pos_y}), 
                    start_cb_size({start_size.x/5.f,start_size.y/6.f}), 
                    easy_cb(button_type::easy, {start_pos.x+start_cb_size.x/2.f, start_pos.y+start_size.y/2.f + start_gap + start_cb_size.y}, start_cb_size), 
                    medium_cb(button_type::medium,{easy_cb.cb_pos.x+(start_cb_size.x*1.5f), easy_cb.cb_pos.y}, start_cb_size), 
                    hard_cb(button_type::hard,{easy_cb.cb_pos.x+(start_cb_size.x*3.f), easy_cb.cb_pos.y}, start_cb_size),  
                    visible(true) {} 
    void draw (sf::RenderWindow& window); 
};
struct State  
{
    Start_Panel sp; 
    Game_Panel game_panel;
    Game_Stop gs; 
    Control_Panel cp; 
    Difficulty diff;
    int mouse_cell; 
    bool focus; 
    bool game_paused; 
    bool first_move; 
    bool game_ended;  

    State (): 
                sp(), 
                game_panel({9,9}, 15), 
                gs(),
                cp(game_panel.border, 15, diff), //MODIFICA
                diff(Difficulty::easy), 
                focus(false), 
                game_paused(false), 
                first_move(true),
                mouse_cell(-1), 
                game_ended(false) {} 
    
    void AdjustView(sf::RenderWindow& window); 
    void reveal(Grid& g, int starting_index_cell); 
    void flood_reveal(Grid& g, int starting_index_cell, Cell& start_c); 
    void ending_reveal(Grid& g, int starting_index_cell); 
    void reset(); 
    void pause(); 
    void restart(); 
    void exit(); 
    void set_difficulty(Difficulty diff);
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
        (window_height - Grid_size.y - (gap * cell_num.y) + (Grid_size.y/4.f)) / 2.0f 
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

Timer::Timer(sf::Vector2f header_pos, float cell_size, float pos_y, sf::Vector2f size){
    real_timer = 0; 
    acc = 0.f; 
    isRunning = false; 
    timer_size = {size}; 
    timer_pos= {header_pos.x + header_parameter_gap, pos_y}; 

    sf::Vector2f pos;
    for(int i = 0; i<3;i++){
        pos = {
            timer_pos.x+((timer_size.x/3)*i), 
            timer_pos.y
        }; 

        timer_numbers.push_back(Number(pos, {timer_size.x/3, timer_size.y})); 
    }

}

Flag_Counter::Flag_Counter(sf::Vector2f header_pos, sf::Vector2f header_size, float cell_size, float pos_y, sf::Vector2f size){
    num_flag = 0; 
    flag_size ={size}; 
    flag_pos =  {header_pos.x + header_size.x - (flag_size.x + header_parameter_gap), pos_y}; 

    sf::Vector2f pos;
    for(int i = 0; i<3;i++){
        pos = {
            flag_pos.x+((flag_size.x/3)*i), 
            flag_pos.y
        }; 

        flag_numbers.push_back(Number(pos, {flag_size.x/3, flag_size.y})); 
    }
}


Border::Border(float cell_size, Grid& grid, Header& header){ 

    thickness = start_cell_size/2.f; 

    
    b_pos = {   header.h_pos.x - header_border_gap - thickness, 
                header.h_pos.y - header_border_gap - thickness
            }; 
    b_size = {  header.h_size.x + (header_border_gap*2) + (thickness * 2), 
                header.h_size.y + (header_border_gap*2) + (thickness*2) + grid.Grid_size.y + (gap * grid.cell_num.y)
            }; 

    angle_cells.clear();
    side_cells.clear();

    angle_cells.push_back(Border_Cell({b_pos}, {thickness, thickness}, &border_textures[2])); 
    angle_cells.push_back(Border_Cell({b_pos.x + b_size.x - thickness, b_pos.y}, {thickness, thickness}, &border_textures[3]));
    angle_cells.push_back(Border_Cell({b_pos.x, b_pos.y + b_size.y - thickness}, {thickness, thickness}, &border_textures[4])); 
    angle_cells.push_back(Border_Cell({b_pos.x + b_size.x - thickness, b_pos.y + b_size.y - thickness}, {thickness, thickness}, &border_textures[5])); 

    sf::Vector2f up_down_cell_size = {b_size.x -(thickness*2), thickness}; 
    sf::Vector2f left_right_cell_size = {thickness, b_size.y -(thickness*2)};  

    side_cells.push_back(Border_Cell({b_pos.x + thickness, b_pos.y}, up_down_cell_size, &border_textures[0])); 
    side_cells.push_back(Border_Cell({b_pos.x + thickness, b_pos.y + b_size.y - thickness}, up_down_cell_size, &border_textures[0])); 
    side_cells.push_back(Border_Cell({b_pos.x, b_pos.y + thickness}, left_right_cell_size, &border_textures[1])); 
    side_cells.push_back(Border_Cell({b_pos.x + b_size.x - thickness, b_pos.y + thickness}, left_right_cell_size, &border_textures[1])); 

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
        case stop_type::Pause:
            title.setString("Pausa!"); 
            break; 
        
        case stop_type::Win: 
            title.setString("Hai vinto!"); 
            break; 

        case stop_type::Lose: 
            title.setString("Hai perso!"); 
            break; 

        case stop_type::New_Game: 
            title.setString("Scegliere"); 
            break;

        default: 
            return; 
    }

    title.setCharacterSize((type == stop_type::New_Game)? 70.f : 140.f);
    title.setFillColor(sf::Color::Black); 
    title.setOutlineThickness(2.f); 
    title.setOutlineColor(sf::Color::White); 
    auto b = title.getLocalBounds(); 
    title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y}); 
    if(type == stop_type::New_Game)
        title.setPosition({gs_pos.x + gs_size.x/2.f, gs_pos.y + gs_size.y/2.f - stop_gap*2.f - title.getCharacterSize()*2.f}); 
    else 
        title.setPosition({gs_pos.x + gs_size.x/2.f, gs_pos.y + stop_gap});     

    window.draw(title);

    switch(type){
        case stop_type::New_Game: 
            title.setString("la nuova modalita'"); 
            break; 

        case stop_type::Pause: 
            title.setString("Tempo impiegato fino ad ora: "+ to_string(time/3600) + (time/3600 == 1? " ora " : " ore ") + to_string((time%3600)/60) + ((time%3600)/60 == 1? " minuto " : " minuti ") + to_string((time%3600)%60) + ((time%3600)%60 == 1? " secondo " : " secondi "));
            break; 
            
        default: 
            title.setString("Tempo impiegato: "+ to_string(time/3600) + (time/3600 == 1? " ora " : " ore ") + to_string((time%3600)/60) + ((time%3600)/60 == 1? " minuto " : " minuti ") + to_string((time%3600)%60) + ((time%3600)%60 == 1? " secondo " : " secondi ")); 
    }
    title.setCharacterSize((type == stop_type::New_Game)? 70.f : 20.f); 
    title.setFillColor(sf::Color::Red);
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
    if(type == stop_type::New_Game){ 
        title.setPosition({title.getPosition().x,  title.getPosition().y + 70.f + stop_gap});
        title.setFillColor(sf::Color::Black);
    }
    else 
        title.setPosition({title.getPosition().x,  title.getPosition().y + 140.f + stop_gap}); 
    window.draw(title);

    if(type ==stop_type::New_Game){
        easy_cb.draw(window);
        medium_cb.draw(window); 
        hard_cb.draw(window);
        return; 
    }

    if(type == stop_type::Pause)
    { 
        title.setString("Premere SPACE per riprendere la partita"); 
        title.setCharacterSize(30); 
        title.setFillColor(sf::Color::Black);  
        b = title.getLocalBounds();
        title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        title.setPosition({title.getPosition().x,title.getPosition().y + 20.f + stop_gap}); 
        window.draw(title);

        title.setString("oppure"); 
        title.setCharacterSize(30); 
        b = title.getLocalBounds();
        title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        title.setPosition({title.getPosition().x,title.getPosition().y + title.getCharacterSize() + stop_gap});
        window.draw(title);

        title.setString("scegliere uno dei pulsanti qua sottostanti"); 
        title.setCharacterSize(30); 
        b = title.getLocalBounds();
        title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        title.setPosition({title.getPosition().x,title.getPosition().y + title.getCharacterSize() + stop_gap});
        window.draw(title);
        //TOLTO new_game_cb.draw(window); 
    }
    else 
    { 
        title.setString("Scegliere uno dei pulsanti"); 
        title.setCharacterSize(40);
        title.setFillColor(sf::Color::Black);  
        b = title.getLocalBounds();
        title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        title.setPosition({title.getPosition().x,title.getPosition().y + 20.f + stop_gap}); 
        window.draw(title);

        title.setString("qua sottostanti"); 
        title.setCharacterSize(40); 
        b = title.getLocalBounds();
        title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        title.setPosition({title.getPosition().x,title.getPosition().y + title.getCharacterSize() + stop_gap}); 
        window.draw(title);

        new_game_cb.cb_size = exit_cb.cb_size =  {std_stop_button_width*1.5f, std_stop_button_heigth*1.5f};  
        new_game_cb.cb_pos = {gs_pos.x + gs_size.x/3.f - exit_cb.cb_size.x/2.f, gs_pos.y + gs_size.y - stop_gap - std_stop_button_heigth*1.5f}; 
        new_game_cb.cb_bounds = {new_game_cb.cb_pos, new_game_cb.cb_size};
        exit_cb.cb_pos = {gs_pos.x + gs_size.x*2.f/3.f - exit_cb.cb_size.x/2.f, new_game_cb.cb_pos.y};
        exit_cb.cb_bounds = {exit_cb.cb_pos, exit_cb.cb_size}; 
    }  
    new_game_cb.draw(window);
    exit_cb.draw(window); 
}

void Header::draw(sf::RenderWindow& window)
{
    sf::RectangleShape h(h_size); 
    h.setPosition(h_pos); 
    h.setFillColor(sf::Color(192, 192, 192)); 
    h.setOutlineThickness(header_border_gap); 
    h.setOutlineColor(sf::Color::Black); 
    window.draw(h);
    timer.draw(window);
    f_counter.draw(window);
    face.draw(window); 
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


void Border_Cell::draw(sf::RenderWindow& window)
{
    sf::RectangleShape b (border_cell_size);
    b.setPosition(border_cell_pos);
    b.setTexture(border_cell_texture);
    window.draw(b);
}

void Border::draw(sf::RenderWindow& window)
{
    for(auto& angle : angle_cells)
        angle.draw(window);
    
    for(auto& side : side_cells)
        side.draw(window);
}

void Game_Panel::draw(sf::RenderWindow& window)
{
    border.draw(window); 
    grid.draw(window); 
    header.draw(window);
}


void Control_Button::draw (sf::RenderWindow& window){
    sf::RectangleShape cb (cb_size);
    cb.setPosition(cb_pos); 
    cb.setFillColor(sf::Color(192, 192, 192)); 
    cb.setOutlineThickness(header_border_gap);  
    if(mouse_focus) 
        cb.setOutlineColor(sf::Color::Red);
    else 
        cb.setOutlineColor(sf::Color::Black); 
    window.draw(cb);

    cb_text.setFont(font);
    cb_text.setCharacterSize(cb.getSize().y/3.f); 
    cb_text.setFillColor(sf::Color::Black); 
    cb_text.setOutlineThickness(2.f); 
    cb_text.setOutlineColor(sf::Color::White); 
    
    switch(cb_type){
        case button_type::pause: 
            cb_text.setString("PAUSA");
            break;
        
        case button_type::new_game: 
            cb_text.setString("NUOVA\nPARTITA");
            break;

        case button_type::easy: 
            cb_text.setString("FACILE");
            break;

        case button_type::medium: 
            cb_text.setString("MEDIO");
            break;

        case button_type::hard: 
            cb_text.setString("DIFFICILE");
            break;

        case button_type::exit: 
            cb_text.setString("ESCI"); 
            break; 

        default: 
            break; 
    }
    sf::FloatRect b = cb_text.getLocalBounds();
    cb_text.setOrigin(sf::Vector2f(b.position.x + b.size.x/2.f, b.position.y + b.size.y/ 2.f));
    cb_text.setPosition(sf::Vector2f(cb_pos.x + cb_size.x/2.f, cb_pos.y + cb_size.y /2.f));

    window.draw(cb_text);
}

void Control_Panel::draw (sf::RenderWindow& window){
    sf::RectangleShape cp(cp_size); 
    cp.setPosition(cp_pos); 
    cp.setFillColor(sf::Color(210,180,140)); 
    cp.setOutlineThickness(20.f); 
    cp.setOutlineColor(sf::Color(92,51,23));
    window.draw(cp); 
    new_game.draw(window); 
    pause.draw(window); 
    exit.draw(window);

    //AGGIUNTA
    info.setString("INFO GIOCO");
    info.setCharacterSize(info_size);
    info.setFillColor(sf::Color::Black);
    info.setOutlineThickness(2.f);
    info.setOutlineColor(sf::Color::White);
    auto b = info.getLocalBounds();
    info.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
    info.setPosition({cp_pos.x + cp_size.x/2.f, exit.cb_pos.y + exit.cb_size.y + control_gap*2.f});
    window.draw(info);

    //AGGIUNTA
    info.setOrigin({0.f, 0.f});
    info.setPosition({cp_pos.x + control_gap, info.getPosition().y + info_size + control_gap}); 
    switch(info_diff){
        case Difficulty::easy: 
            info.setString("Difficolta' scelta : \tFACILE");
            break; 

        case Difficulty::medium: 
            info.setString("Difficolta' scelta : \tMEDIA");
            break; 

        case Difficulty::hard: 
            info.setString("Difficolta' scelta : \tDIFFICILE");
            break; 
        default: 
            return; 
    }
    window.draw(info);

    //AGGIUNTA
    info.setString("Totale mine nella griglia : \t" + to_string(info_mine));
    info.setOrigin({0.f, 0.f}); 
    info.setPosition({cp_pos.x + control_gap, info.getPosition().y + info_size + control_gap}); 
    window.draw(info);

    //AGGIUNTA
    info.setString("Obbiettivo del gioco : "); 
    info.setOrigin({0.f, 0.f});
    info.setPosition({cp_pos.x + control_gap, info.getPosition().y + info_size + control_gap*1.5f}); 
    window.draw(info);

    //AGGIUNTA
    info.setString("Scoprire tutte le celle che nascondono una \nmina. \nLa partita e' persa alla prima mina scoperta."); 
    info.setOrigin({0.f, 0.f});
    info.setPosition({cp_pos.x + control_gap, info.getPosition().y + info_size + control_gap}); 
    window.draw(info);

    //AGGIUNTA
    info.setString("Istruzioni :\n\t- Click Sinistro : Scopre cella\n\t- Click Destro : Mette/toglie bandiera"); 
    info.setOrigin({0.f, 0.f});
    info.setPosition({cp_pos.x + control_gap, info.getPosition().y + info_size*3.f + control_gap*1.5f}); 
    window.draw(info);

}


void Start_Panel::draw(sf::RenderWindow& window){
    sf::RectangleShape s (start_size); 
    s.setPosition (start_pos);
    s.setFillColor(sf::Color(210,180,140)); 
    s.setOutlineThickness(20.f); 
    s.setOutlineColor(sf::Color(92,51,23)); 
    window.draw (s); 

    start_title.setString("Benvenuto su"); 
    start_title.setCharacterSize(60);
    start_title.setFillColor(sf::Color::Black); 
    start_title.setOutlineThickness(2.f);
    start_title.setOutlineColor(sf::Color::White); 
    auto b = start_title.getLocalBounds(); 
    start_title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y}); 
    start_title.setPosition({start_pos.x + start_size.x/2.f, start_pos.y + start_gap});             
    window.draw(start_title);

    start_title.setString("MINESWEEPER"); 
    start_title.setCharacterSize(120);
    b = start_title.getLocalBounds(); 
    start_title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y}); 
    start_title.setPosition({start_title.getPosition().x,start_title.getPosition().y + 60.f + start_gap});           
    window.draw(start_title);

    start_title.setString("Versione E.0"); 
    start_title.setCharacterSize(40);
    start_title.setFillColor(sf::Color::Red);
    b = start_title.getLocalBounds(); 
    start_title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y}); 
    start_title.setPosition({start_title.getPosition().x,start_title.getPosition().y + 120.f + start_gap});            
    window.draw(start_title);

    start_title.setString("Scegliere la difficolta' di gioco"); 
    start_title.setFillColor(sf::Color::Black);
    start_title.setCharacterSize(60);
    b = start_title.getLocalBounds(); 
    start_title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y}); 
    start_title.setPosition({start_title.getPosition().x,start_title.getPosition().y + 40.f + start_gap});            
    window.draw(start_title);

    easy_cb.draw(window); 
    medium_cb.draw(window); 
    hard_cb.draw(window);

}

void State::draw (sf::RenderWindow& window){
    if(sp.visible) 
        sp.draw(window); 
    else{
        game_panel.draw (window);
        cp.draw(window); 
        gs.draw(window); 
    }
}

////////////////ALTRE FUNZIONI////////////////

void Flag_Counter::set_number(bool adding){ 
    if(num_flag == 999) return; 
    if(adding? num_flag++ : num_flag--);  
    flag_numbers[2].num_texture = &Clock_textures[num_flag%10];
    flag_numbers[1].num_texture = &Clock_textures[(num_flag/10)%10];
    flag_numbers[0].num_texture = &Clock_textures[(num_flag/100)%10];
}

void Timer::update(float elapsed){
    if(!isRunning) return; 
    acc += elapsed; 
    while(acc >= 1.f){ 
        acc -= 1.f; 
        real_timer ++;
        if(real_timer >= 999) return; 
        timer_numbers[2].num_texture = &Clock_textures[real_timer%10];
        timer_numbers[1].num_texture = &Clock_textures[(real_timer/10)%10];
        timer_numbers[0].num_texture = &Clock_textures[(real_timer/100)%10];  
    }

}

void Grid::place_mines(int starting_cell_index){

    int x = cells[starting_cell_index].row_index; 
    int y = cells[starting_cell_index].column_index;
    int casual_index; 

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

void State::AdjustView(sf::RenderWindow& window){
    float original_aspect = static_cast<float>(window_width) / static_cast<float>(window_height);

    sf::Vector2u window_size = window.getSize();
    float new_width = static_cast<float>(window_size.x);
    float new_height = static_cast<float>(window_size.y);
    float new_aspect = new_width / new_height;

    float viewport_x = 0.f;
    float viewport_y = 0.f;
    float viewport_width = 1.f;
    float viewport_height = 1.f;

    if (new_aspect > original_aspect)
    {
        viewport_width = original_aspect / new_aspect;
        viewport_x = (1.f - viewport_width) / 2.f;
    }
    else if (new_aspect < original_aspect)
    {
        viewport_height = new_aspect / original_aspect;
        viewport_y = (1.f - viewport_height) / 2.f; 
    }

    sf::Vector2f center(static_cast<float>(window_width) / 2.f, static_cast<float>(window_height) / 2.f);
    sf::Vector2f size(static_cast<float>(window_width), static_cast<float>(window_height));
    sf::View view(center, size);

    sf::Vector2f viewport_pos(viewport_x, viewport_y);
    sf::Vector2f viewport_size(viewport_width, viewport_height);
    view.setViewport(sf::FloatRect(viewport_pos, viewport_size));

    window.setView(view);
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
    game_panel.header.timer.isRunning = false; 
    gs.time = game_panel.header.timer.real_timer; 
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
    if(c.state == cell_state::Flag) game_panel.header.f_counter.set_number(false);
    c.state = cell_state::Revealed; 
    game_panel.grid.num_revealed++; 

    if(c.type == cell_type::Mine){
        c.texture = &Exploded_Mine_texture;
        gs.type = stop_type::Lose; 
        ending_reveal(g,starting_index_cell); 
        game_panel.header.face.face_texture = &lost_face_texture; 
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
        game_panel.header.face.face_texture = &win_face_texture; 
    }
}

void State::reset(){
    game_panel = Game_Panel(game_panel.grid.cell_num, game_panel.grid.mine_num);  
    gs = Game_Stop(); 
    game_paused = focus = game_ended= false; 
    first_move = true; 
    mouse_cell = -1; 
}


void State::pause(){
    if(!first_move) 
        game_panel.header.timer.isRunning = false; 
    gs.time = game_panel.header.timer.real_timer;
    game_paused = true; 
    if(gs.type != stop_type::New_Game) gs.type = stop_type::Pause; 
    gs.visible = true; 
}

void State::restart(){
    gs.visible = false;
    gs.type = stop_type::None; 
    game_paused = false; 
    if(!first_move) 
        game_panel.header.timer.isRunning = true;  
}

void State::set_difficulty(Difficulty diff){
    switch(diff){
        case Difficulty::easy: 
            game_panel.grid.cell_num = {9,9};
            game_panel.grid.mine_num = cp.info_mine = 15; //MODIFICA  
            cp.info_diff = Difficulty::easy; //AGGIUNTA   
            break; 

        case Difficulty::medium: 
            game_panel.grid.cell_num = {16,16};
            game_panel.grid.mine_num = cp.info_mine = 40; //MODIFICA  
            cp.info_diff = Difficulty::medium; //AGGIUNTA  
            break;

        case Difficulty::hard: 
            game_panel.grid.cell_num = {30,20};
            game_panel.grid.mine_num = cp.info_mine = 99; //MODIFICA  
            cp.info_diff = Difficulty::hard; //AGGIUNTA    
            break; 

        default: 
            break;
    }

    reset();           
}

void State::exit(){
    reset(); 
    sp.visible = true; 
}

////////////////EVENTI////////////////

void handle_close (sf::RenderWindow& window)
{
    window.close();
}

void handle_resize (const sf::Event::Resized& resized, sf::RenderWindow& window, State& state)
{
    float aspect = static_cast<float>(window_width)/static_cast<float>(window_height);
    sf::Vector2u ws = resized.size;
    float new_aspect = static_cast<float>(ws.x)/static_cast<float>(ws.y);
    if (new_aspect < aspect)
        ws = {ws.x,static_cast<unsigned>(ws.x/aspect)};
        else
        ws = {static_cast<unsigned>(ws.y*aspect),ws.y};
    window.setSize(ws);
    state.AdjustView(window); 
}

template <typename T>
void handle (T& event, State& state) {}

void handle (const sf::Event::FocusGained&, State& state)
{
    state.focus = true; 
    if (!state.first_move && !state.game_ended && !state.game_paused) 
        state.game_panel.header.timer.isRunning = true;
}

void handle (const sf::Event::FocusLost&, State& state)
{
    if(state.sp.visible) return; //EX MODIFICA: se la schermata iniziale è ancora visibile non fare nulla in quanto il gioco non è ancora iniziato
    state.pause(); 
    state.focus = false; 
    if(!state.first_move && !state.game_ended) 
        state.game_panel.header.timer.isRunning = false; 
}

void handle_mouse_pressed (const sf::Event::MouseButtonPressed& mouse, State& state, sf::RenderWindow& window)
{
    const sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Vector2i(mouse.position.x, mouse.position.y));

    if(state.game_ended || state.game_paused)
    {
        if(mouse.button == sf::Mouse::Button::Left){ //EX MODIFCA 
            if (state.gs.type == stop_type::New_Game){
                if (state.gs.easy_cb.cb_bounds.contains(mouse_pos)) state.set_difficulty(Difficulty::easy); 
                else if (state.gs.medium_cb.cb_bounds.contains(mouse_pos)) state.set_difficulty(Difficulty::medium);
                else if (state.gs.hard_cb.cb_bounds.contains(mouse_pos)) state.set_difficulty(Difficulty::hard);
            }
            else if(state.gs.new_game_cb.cb_bounds.contains(mouse_pos)) state.gs.type = stop_type::New_Game; 
            else if(state.gs.exit_cb.cb_bounds.contains(mouse_pos)) state.exit(); 
        }
        return; 
    }

    if( mouse.button == sf::Mouse::Button::Left){
        if(state.sp.visible){ 
            if (state.sp.easy_cb.cb_bounds.contains(mouse_pos))
            {
                state.set_difficulty(Difficulty::easy); 
                state.sp.visible = false; 
            }
            else if(state.sp.medium_cb.cb_bounds.contains(mouse_pos)){ 
                state.set_difficulty(Difficulty::medium);
                state.sp.visible = false; 
            }
            else if(state.sp.hard_cb.cb_bounds.contains(mouse_pos)){ 
                state.set_difficulty(Difficulty::hard);
                state.sp.visible = false; 
            }

            return; 
        }
        
        if(state.cp.new_game.cb_bounds.contains(mouse_pos)){
            state.gs.type = stop_type::New_Game; 
            state.pause();
            return; 
        }

        if(state.cp.pause.cb_bounds.contains(mouse_pos)){
            state.pause(); 
            return; 
        }

        if(state.cp.exit.cb_bounds.contains(mouse_pos)){
            state.exit(); 
            return; 
        }

        if(state.mouse_cell <0 || state.mouse_cell >= state.game_panel.grid.cells.size() || state.game_panel.grid.cells[state.mouse_cell].state == cell_state::Revealed) return; 

        if(state.first_move){
            state.first_move = false; 
            state.game_panel.grid.place_mines(state.mouse_cell); 
            state.game_panel.grid.place_numbers(); 
            state.reveal(state.game_panel.grid, state.mouse_cell); 
            state.game_panel.header.timer.isRunning = true; 
        }
        else{
            state.game_panel.header.face.face_texture = &Click_face_texture;  
            state.reveal(state.game_panel.grid, state.mouse_cell); 
        }
    }
    else if(mouse.button == sf::Mouse::Button::Right){ //EX MODIFICA
        if(state.first_move) return; 

        if(state.mouse_cell <0 || state.mouse_cell >= state.game_panel.grid.cells.size() || state.game_panel.grid.cells[state.mouse_cell].state == cell_state::Revealed) return; 

        if(state.game_panel.grid.cells[state.mouse_cell].state != cell_state::Flag){
            state.game_panel.header.f_counter.set_number(true); 
            state.game_panel.grid.cells[state.mouse_cell].state = cell_state::Flag; 
            state.game_panel.grid.cells[state.mouse_cell].texture = &Flag_texture;
        }
        else{
            state.game_panel.header.f_counter.set_number(false); 
            state.game_panel.grid.cells[state.mouse_cell].state = cell_state::Covered; 
            state.game_panel.grid.cells[state.mouse_cell].texture = &Covered_texture;
        }
        
    }

}


void handle (const sf::Event::MouseButtonReleased& mouse, State& state)
{
    if(state.game_ended || state.game_panel.header.face.face_texture != &Click_face_texture) return; 
    state.game_panel.header.face.face_texture = &smile_face_texture; 
}

void handle (const sf::Event::KeyPressed& key, State& state) 
{
    if (state.game_paused && key.scancode == sf::Keyboard::Scancode::Space) state.restart(); 
}


void handle_mouse_moved (const sf::Event::MouseMoved& ev, sf::RenderWindow& window, State& state)
{
    const sf::Vector2f mouse_float_pos = window.mapPixelToCoords(sf::Vector2i(ev.position.x, ev.position.y));

    if(state.sp.visible){
        state.sp.easy_cb.mouse_focus = (state.sp.easy_cb.cb_bounds.contains(mouse_float_pos)? true : false); 
        state.sp.medium_cb.mouse_focus = (state.sp.medium_cb.cb_bounds.contains(mouse_float_pos)? true : false); 
        state.sp.hard_cb.mouse_focus = (state.sp.hard_cb.cb_bounds.contains(mouse_float_pos)? true : false); 
        return;
    }

    if(state.game_ended || state.game_paused){
        state.gs.new_game_cb.mouse_focus = (state.gs.new_game_cb.cb_bounds.contains(mouse_float_pos)? true : false); 
        state.gs.easy_cb.mouse_focus = (state.gs.easy_cb.cb_bounds.contains(mouse_float_pos)? true : false); 
        state.gs.medium_cb.mouse_focus = (state.gs.medium_cb.cb_bounds.contains(mouse_float_pos)? true : false); 
        state.gs.hard_cb.mouse_focus = (state.gs.hard_cb.cb_bounds.contains(mouse_float_pos)? true : false); 
        state.gs.exit_cb.mouse_focus = (state.gs.exit_cb.cb_bounds.contains(mouse_float_pos)? true : false);  
        return; 
    }

    state.cp.new_game.mouse_focus = (state.cp.new_game.cb_bounds.contains(mouse_float_pos)? true : false);  
    state.cp.pause.mouse_focus = (state.cp.pause.cb_bounds.contains(mouse_float_pos)? true : false); 
    state.cp.exit.mouse_focus = (state.cp.exit.cb_bounds.contains(mouse_float_pos)? true : false); 

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
    srand(time(NULL));
    load_textures_fonts();
    sf::RenderWindow window (sf::VideoMode ({window_width, window_height}), window_title);
    window.setFramerateLimit (max_frame_rate);

    sf::RectangleShape border({(window_width - 40.f),(window_height-40.f)}); 
    border.setPosition({20.f, 20.f}); 
    border.setFillColor(sf::Color::Transparent);  
    border.setOutlineThickness(20.f);
    border.setOutlineColor(sf::Color(0, 100, 0));

    State state;
    sf::Clock Clock; 
    float elapsed;

    state.AdjustView(window);

    while (window.isOpen()) 
    {
        window.handleEvents (
                            [&window](const sf::Event::Closed&) { handle_close (window); },
                            [&window, &state](const sf::Event::Resized& event) { handle_resize (event, window, state); }, 
                            [&window, &state](const sf::Event::MouseButtonPressed& event) { handle_mouse_pressed (event, state, window); },
                            [&window, &state](const sf::Event::MouseMoved& event) {handle_mouse_moved (event, window, state); },
                            [&state] (const auto& event) { handle (event, state); } 
        );


        elapsed = Clock.restart().asSeconds(); 
        state.game_panel.header.timer.update(elapsed); 

        window.clear(sf::Color(144, 238, 144));
        state.draw(window); 
        window.draw(border); 
        window.display();

    }
}