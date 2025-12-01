#include <SFML/Graphics.hpp>
#include <random> //EX  
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SFML/System/Clock.hpp>
#include "../risorse/textures_fonts.hpp"

using namespace std;

////////////////FINESTRA////////////////
const char* window_title = "Rules and Title";
const unsigned window_width = 800; //EX MODIFICA
const unsigned window_height = 600; //EX MODIFICA
const float max_frame_rate = 60;
const float window_border_thickness = 15; //EX
const float cell_proportion = 0.85; //EX

////////////////PANNELLI GENERALE////////////////
sf::Color panel_background_color = sf::Color(210,180,140); //EX
sf::Color panel_border_color = sf::Color(92,51,23); //EX
sf::Color focus_color = sf::Color::Red; //ex
const float panel_thickness = 15; //EX


////////////////TESTO////////////////
const float font_size_mine_title = 80; //EX
sf::Color text_color = sf::Color::Black;//EX
sf::Color text_border_color = sf::Color::White; //EX
const float text_thickness = 2; //ex

////////////////STATO////////////////

enum class Difficulty{easy, medium,hard}; 
const float title_gap = 30; //AGGIUNTA

////////////////START PANEL////////////////
const float start_width = (window_width/4)*3; //EX 
const float start_height = (window_height/4)*3;
const float start_pos_x = window_width/8; //ex 
const float start_pos_y = window_height/8; //EX
const float start_gap =25; //ex
const float start_size_subtitle = 30; //EX
const float start_size_text = 17; //EX
const float start_cb_width = start_width/5; 
const float start_cb_height = start_height/6; 

////////////////GAME PANEL////////////////

const float game_horizontal_displacement = 50; //EX MODIFICA
const float game_vertical_displacement = 100; //mai usato ex
const float starting_cell_size = (((window_width-(game_horizontal_displacement*2))/2)/(10))*cell_proportion;
const float gap_ratio = 2.f/starting_cell_size; //EX MODIFICA

////////////////STOP PANEL////////////////

enum class stop_type{None, Win, Lose, Pause, New_Game}; 
const float stop_width = (window_width/3)*2; //EX 
const float stop_height = (window_height/3)*2;
const float stop_pos_x = window_width/6; //ex 
const float stop_pos_y = window_height/6; //EX
const float stop_gap =20; //ex
const float stop_cb_width = stop_width/4; 
const float stop_cb_height = stop_height/6; 
const float stop_title_size = 90; //EX
const float stop_title2_size = 45; //EX
const float stop_subtitle2_size = 30; //ex
const float stop_subtitle_size = 20; //EX
const float stop_time_text_size = 15; //EX

////////////////HEADER////////////////

const float header_parameter_gap = 30; 
const float header_border_gap = 5;
const float header_grid_proportion = 2;

////////////////BLOCCO////////////////

enum class cell_type{Mine,Empty, Number}; 
enum class cell_state{ Covered, Revealed, Flag}; 

////////////////PULSANTE DI CONTROLLO////////////////

enum class button_type{new_game, pause, easy, medium, hard, exit};
sf::Color button_color = sf::Color(192, 192, 192); 
const float button_text_thickness = 1.5; 
const float button_text_proportion =3.5; 

////////////////PANNELLO DI CONTROLLO////////////////

const float control_button_horizontal_gap = 20; 
const float control_button_vertical_gap = 20; 
const float control_info_gap = 10; //EX MODIFICA: gap tra le scritte e i pulsanti 
const float info_size = 10; //AGGIUNTA: dimensione del testo nel control panel 

////////////////STRUCT////////////////
struct Cell
{
    sf::Vector2f cell_pos; //ex
    float cell_size; //ex
    int row_index, column_index;
    sf::FloatRect cell_bounds; //ex
    bool mouse_focus;
    sf::Texture* cell_texture; //ex
    cell_type cell_type; //ex
    int mine_adj;
    cell_state cell_state; //ex
    float gap; //ex
    
    Cell (sf::Vector2f pos, float size, int row_index, int column_index, float gap) :  cell_pos (pos),
                                                                            cell_size (size),
                                                                            row_index(row_index),
                                                                            column_index(column_index),
                                                                            cell_bounds (pos, {size, size}),mouse_focus(false),
                                                                            cell_texture (&Covered_texture),
                                                                            cell_type(cell_type::Empty),
                                                                            mine_adj(0),
                                                                            cell_state(cell_state::Covered),
                                                                            gap(gap) {}
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

    Grid (sf::Vector2i cell_num, int mine_num, float& cell_size, float gap); //EX MODIFICA
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
                                                                                                        face_pos({(header_pos.x+ (header_size.x/2))- (size/2), pos_y}),
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

    Flag_Counter(sf::Vector2f header_pos, sf::Vector2f header_size, float cell_size, float pos_y, sf::Vector2f size, int mine_num);
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

    Header(Grid& grid, float gap, int mine_num): //EX MODIFICA 
                        h_size({ grid.Grid_size.x + (gap*(grid.cell_num.x-1)) - (2*header_border_gap), (starting_cell_size*header_grid_proportion) - (2*header_border_gap) }), 
                        h_pos({ grid.Grid_pos.x + header_border_gap, grid.Grid_pos.y - (starting_cell_size*header_grid_proportion + gap) + header_border_gap }), //ex (mandato a capo)
                        details_size({(h_size.y - (h_size.y/3))*3/2, h_size.y - (h_size.y/3)}), 
                        details_pos_y(h_pos.y + (h_size.y/6)),
                        timer(h_pos, starting_cell_size, details_pos_y, details_size), 
                        face(h_pos, h_size, starting_cell_size, details_pos_y, details_size.y), 
                        f_counter(h_pos, h_size, starting_cell_size, details_pos_y, details_size, mine_num) {} 
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

    Border(float cell_size, Grid& grid, Header& header, float gap); //EX MODIFICA 
    void draw (sf::RenderWindow& window);
}; 

struct Game_Panel
{
    float cell_size;
    float gap; //EX AGGIUNTA
    Grid grid;  
    Header header;
    Border border;  

    Game_Panel(sf::Vector2i cell_num, int mine_num):
                                                    cell_size((((window_width-(game_horizontal_displacement*2))/2)/(cell_num.x+1))*cell_proportion),
                                                    gap(cell_size*gap_ratio), //EX AGGIUNTA
                                                    grid(cell_num, mine_num, cell_size, gap), //EX MODIFICA
                                                    header(grid, gap, mine_num), //EX MODIFICA
                                                    border(cell_size, grid, header, gap) {} //EX MODIFICA
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
                                    cp_pos(game_horizontal_displacement, border.b_pos.y + border.thickness), 
                                    cp_size({border.b_size.x -(border.thickness *2), border.b_size.y -(border.thickness *2)}), 
                                    button_size({(cp_size.x-(control_button_horizontal_gap*2))/3, (cp_size.y-(control_button_vertical_gap*2))/8}),  
                                    new_game(button_type::new_game, {cp_pos.x + control_button_horizontal_gap, cp_pos.y + control_button_vertical_gap}, button_size) ,
                                    pause(button_type::pause, {cp_pos.x +cp_size.x - control_button_horizontal_gap - button_size.x, cp_pos.y + control_button_vertical_gap}, button_size), 
                                    exit(button_type::exit, {cp_pos.x +cp_size.x/2 - button_size.x/2, pause.cb_pos.y + pause.cb_size.y + control_button_vertical_gap + control_info_gap/2}, button_size), //EX MODIFICA
                                    info_mine(num_mines), //AGGIUNTA 
                                    info_diff(diff) //AGGIUNTA
                                    {}

    void draw (sf::RenderWindow& window);
};

struct Stop_Panel //EX
{ 
    sf::Text title{font}; 
    stop_type type; 
    sf::Vector2f stop_size; //ex
    sf::Vector2f stop_pos; //ex
    bool visible; 
    int time;
    Control_Button new_game_cb, easy_cb, medium_cb, hard_cb, exit_cb; 
    
    Stop_Panel (): //EX
                visible(false), 
                type(stop_type::None), 
                stop_size({stop_width, stop_height}), 
                stop_pos({stop_pos_x,stop_pos_y}), //ex
                time(0), 
                new_game_cb(button_type::new_game,{stop_pos.x + stop_size.x/3 - stop_cb_width/2, stop_pos.y + stop_size.y - stop_gap - stop_cb_height}, {stop_cb_width, stop_cb_height}), 
                easy_cb(button_type::easy, {stop_pos.x + stop_gap*2, stop_pos.y + stop_size.y*3/4 -stop_cb_height/2}, {stop_cb_width, stop_cb_height}), //EX
                medium_cb(button_type::medium, {stop_pos.x+ stop_size.x/2 - stop_cb_width/2, stop_pos.y + stop_size.y*3/4 -stop_cb_height/2}, {stop_cb_width, stop_cb_height}), //EX
                hard_cb(button_type::hard, {stop_pos.x+ stop_size.x - stop_gap*2 - stop_cb_width, stop_pos.y + stop_size.y*3/4 -stop_cb_height/2}, {stop_cb_width, stop_cb_height}), //EX
                exit_cb(button_type::exit, {stop_pos.x + stop_size.x*2/3 - stop_cb_width/2, stop_pos.y + stop_size.y - stop_gap - stop_cb_height}, {stop_cb_width, stop_cb_height}) 
                {} 
                
    void draw (sf::RenderWindow& window);
}; 

struct Start_Panel
{
    sf::Vector2f start_size; 
    sf::Vector2f start_pos; 
    sf::Text start_title{font}; 
    Control_Button easy_cb, medium_cb, hard_cb; 
    bool visible; 

    Start_Panel (): 
                    start_size({start_width, start_height}), 
                    start_pos({start_pos_x, start_pos_y}), 
                    easy_cb(button_type::easy, {start_pos.x+start_cb_width/2, start_pos.y+start_size.y/2 + start_cb_height}, {start_cb_width, start_cb_height}), //EX
                    medium_cb(button_type::medium,{easy_cb.cb_pos.x+(start_cb_width*1.5f), easy_cb.cb_pos.y}, {start_cb_width, start_cb_height}), 
                    hard_cb(button_type::hard,{easy_cb.cb_pos.x+(start_cb_width*3.f), easy_cb.cb_pos.y}, {start_cb_width, start_cb_height}),  
                    visible(true) {} 
    void draw (sf::RenderWindow& window); 
};
struct State  
{
    Difficulty diff; //EX MODIFICA (SCAMBIO ORDINE CON SOTTO)
    Start_Panel sp; 
    Game_Panel game_panel;
    Stop_Panel stop_p; //EX
    Control_Panel cp; 
    int mouse_cell; 
    bool focus; 
    bool game_paused; 
    bool first_move; 
    bool game_ended;
    sf::Text title; //AGGIUNTA

    State (): 
                diff(Difficulty::easy), 
                sp(), 
                game_panel({9,9}, 15), 
                stop_p(), //ex
                cp(game_panel.border, 15, diff), //MODIFICA
                focus(false), 
                game_paused(false), 
                first_move(true),
                mouse_cell(-1), 
                game_ended(false),
                title(font) {}  //AGGIUNTA
    
    //void AdjustView(sf::RenderWindow& window); 
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

Grid::Grid (sf::Vector2i bs, int bn, float& cell_size, float gap){ //EX MODIFICA (sono state fatte due modifica ma la principale è che cell_size è per riferimento in modo da cambiarla ovunque nel codice in base alla difficoltà)
    cell_num = bs;
    mine_num = bn;
    num_revealed = 0;

    cell_size = (mine_num==99? cell_size/cell_proportion : cell_size);  //EX AGGIUNTA: toglio la proporzione per renderle leggermente più grandi 

    Grid_size = {cell_size * cell_num.x, cell_size * cell_num.y};

    Grid_pos = { 
        window_width - Grid_size.x - game_horizontal_displacement - (gap * cell_num.x),
        (window_height - Grid_size.y - (gap * cell_num.y) + (Grid_size.y/4.f)) / 2.f
    };
    sf::Vector2f pos; 
    
    for (float hb = 0; hb < cell_num.x; hb++) {
        for (float vb = 0; vb < cell_num.y; vb++) {
            pos = {
                Grid_pos.x + hb * (cell_size + gap), 
                Grid_pos.y + vb * (cell_size + gap)
            };
            cells.push_back(Cell(pos, cell_size, hb, vb, gap)); 
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
        pos = {
            flag_pos.x+((flag_size.x/3)*i), 
            flag_pos.y
        }; 

        flag_numbers.push_back(Number(pos, {flag_size.x/3, flag_size.y})); 
    }

    flag_numbers[2].num_texture = &Clock_textures[num_flag%10];
    flag_numbers[1].num_texture = &Clock_textures[(num_flag/10)%10];
    flag_numbers[0].num_texture = &Clock_textures[(num_flag/100)%10];
}


Border::Border(float cell_size, Grid& grid, Header& header, float gap){//EX MODIFICA 

    thickness = cell_size/2; //EX MODIFICA

    
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
    sf::RectangleShape c ({cell_size,cell_size});
    c.setTexture(cell_texture);
    c.setPosition(cell_pos);

    if(mouse_focus){
        c.setOutlineThickness(gap); //ex 
        c.setOutlineColor(focus_color); //ex 
    }

    window.draw(c);

}

void Grid::draw (sf::RenderWindow& window)
{
    for (auto& cell : cells)
        cell.draw (window);
}


void Stop_Panel::draw(sf::RenderWindow& window){ 
    if(!visible) return; 

    sf::RectangleShape s(stop_size);
    s.setPosition(stop_pos); 
    s.setFillColor(panel_background_color);//EX
    s.setOutlineThickness(panel_thickness); //EX
    s.setOutlineColor(panel_border_color); //EX
    window.draw(s);
    float stop_text_pos_x = stop_pos.x + stop_size.x/2; //EX

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

    title.setCharacterSize((type == stop_type::New_Game)? stop_title2_size : stop_title_size); //EX
    title.setFillColor(text_color); //EX
    title.setOutlineThickness(text_thickness); //EX
    title.setOutlineColor(text_border_color); //EX
    auto b = title.getLocalBounds(); 
    title.setOrigin({b.position.x + b.size.x/2, b.position.y}); 
    if(type == stop_type::New_Game)
        title.setPosition({stop_text_pos_x, stop_pos.y + stop_size.y/2 - stop_gap*2 - title.getCharacterSize()*2}); //EX
    else 
        title.setPosition({stop_text_pos_x, stop_pos.y + stop_gap}); //EX
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
    title.setCharacterSize((type == stop_type::New_Game)? stop_title2_size : stop_time_text_size); 
    title.setFillColor(focus_color); //ex
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
    if(type == stop_type::New_Game){ 
        title.setPosition({stop_text_pos_x,  title.getPosition().y + stop_title2_size + stop_gap});
        title.setFillColor(text_color);
    }
    else 
        title.setPosition({stop_text_pos_x,  title.getPosition().y + stop_title_size + stop_gap}); 

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
        title.setCharacterSize(stop_subtitle_size); 
        title.setFillColor(text_color);  
        b = title.getLocalBounds();
        title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        title.setPosition({stop_text_pos_x,title.getPosition().y + stop_subtitle_size + stop_gap}); 
        window.draw(title);

        title.setString("oppure"); 
        b = title.getLocalBounds();
        title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        title.setPosition({stop_text_pos_x,title.getPosition().y + stop_subtitle_size + stop_gap});
        window.draw(title);

        title.setString("scegliere uno dei pulsanti qua sottostanti"); 
        b = title.getLocalBounds();
        title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        title.setPosition({stop_text_pos_x,title.getPosition().y + stop_subtitle_size + stop_gap});
        window.draw(title);
        //TOLTO new_game_cb.draw(window); 
    }
    else 
    { 
        title.setString("Scegliere uno dei pulsanti"); 
        title.setCharacterSize(stop_subtitle2_size);
        title.setFillColor(text_color);  
        b = title.getLocalBounds();
        title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        title.setPosition({stop_text_pos_x,title.getPosition().y + stop_time_text_size + stop_gap}); 
        window.draw(title);

        title.setString("qua sottostanti"); 
        b = title.getLocalBounds();
        title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
        title.setPosition({stop_text_pos_x,title.getPosition().y + stop_subtitle2_size + stop_gap}); 
        window.draw(title);
        /*
        new_game_cb.cb_size = exit_cb.cb_size =  {stop_cb_width*1.5f, stop_cb_height*1.5f};  
        new_game_cb.cb_pos = {stop_pos.x + stop_size.x/3 - exit_cb.cb_size.x/2, stop_pos.y + stop_size.y - stop_gap - stop_cb_height*1.5f}; 
        new_game_cb.cb_bounds = {new_game_cb.cb_pos, new_game_cb.cb_size};
        exit_cb.cb_pos = {stop_pos.x + stop_size.x*2/3 - exit_cb.cb_size.x/2, new_game_cb.cb_pos.y};
        exit_cb.cb_bounds = {exit_cb.cb_pos, exit_cb.cb_size}; */
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
    cb.setFillColor(button_color); 
    cb.setOutlineThickness(header_border_gap);  
    if(mouse_focus) 
        cb.setOutlineColor(focus_color);
    else 
        cb.setOutlineColor(text_color); 
    window.draw(cb);

    cb_text.setFont(font);
    cb_text.setCharacterSize(cb.getSize().y/button_text_proportion); //EX
    cb_text.setFillColor(text_color); 
    cb_text.setOutlineThickness(button_text_thickness); //ex
    cb_text.setOutlineColor(text_border_color); 
    
    switch(cb_type){
        case button_type::pause: 
            cb_text.setString("PAUSA");
            break;
        
        case button_type::new_game: 
            cb_text.setString(" NUOVA\nPARTITA"); //EX 
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
    cb_text.setOrigin(sf::Vector2f(b.position.x + b.size.x/2, b.position.y + b.size.y/2));
    cb_text.setPosition(sf::Vector2f(cb_pos.x + cb_size.x/2, cb_pos.y + cb_size.y/2));

    window.draw(cb_text);
}

void Control_Panel::draw (sf::RenderWindow& window){
    sf::RectangleShape cp(cp_size); 
    cp.setPosition(cp_pos); 
    cp.setFillColor(panel_background_color); //EX 
    cp.setOutlineThickness(panel_thickness); //EX 
    cp.setOutlineColor(panel_border_color); //EX 
    window.draw(cp); 
    new_game.draw(window); 
    pause.draw(window); 
    exit.draw(window);

    //AGGIUNTA
    info.setString("INFO GIOCO");
    info.setCharacterSize(info_size*2);
    info.setFillColor(focus_color); //EX
    auto b = info.getLocalBounds();
    info.setOrigin({b.position.x + b.size.x/2, b.position.y});
    info.setPosition({cp_pos.x + cp_size.x/2, exit.cb_pos.y + exit.cb_size.y + control_info_gap*2});
    window.draw(info);

    float info_text_pos_x = cp_pos.x + control_info_gap; //AGGIUNTA: salvo il valore della posizione x del testo successivo poichè verrà riutilizzata più volte
    info.setCharacterSize(info_size);
    info.setFillColor(text_color); //EX
    info.setOrigin({0, 0});
    info.setPosition({info_text_pos_x, info.getPosition().y + info_size*2 + control_info_gap}); 
    switch(info_diff){
        case Difficulty::easy: 
            info.setString("Difficolta' scelta :\tFACILE");
            break; 

        case Difficulty::medium: 
            info.setString("Difficolta' scelta :\tMEDIA");
            break; 

        case Difficulty::hard: 
            info.setString("Difficolta' scelta :\tDIFFICILE");
            break; 
        default: 
            return; 
    }
    window.draw(info);

    //AGGIUNTA
    info.setString("Totale mine nella griglia :\t" + to_string(info_mine));
    info.setOrigin({0, 0}); 
    info.setPosition({info_text_pos_x, info.getPosition().y + info_size + control_info_gap}); 
    window.draw(info);

    //AGGIUNTA
    info.setString("Obbiettivo del gioco :"); 
    info.setOrigin({0, 0});
    info.setPosition({info_text_pos_x, info.getPosition().y + info_size + control_info_gap*1.5f}); 
    window.draw(info);

    //AGGIUNTA
    info.setString("\t- Scoprire tutte le celle che nascondono\n\t  una mina. \n\t- La partita e' persa alla prima mina\n\t  scoperta."); 
    info.setOrigin({0, 0});
    info.setPosition({info_text_pos_x, info.getPosition().y + info_size + control_info_gap}); 
    window.draw(info);

    //AGGIUNTA
    info.setString("Istruzioni :"); 
    info.setOrigin({0, 0});
    info.setPosition({info_text_pos_x, info.getPosition().y + info_size*4 + control_info_gap*1.5f}); 
    window.draw(info);

    //AGGIUNTA
    info.setString("\t- Click Sinistro : Scopre cella\n\t- Click Destro : Mette/toglie bandiera"); 
    info.setOrigin({0, 0});
    info.setPosition({info_text_pos_x, info.getPosition().y + info_size + control_info_gap}); 
    window.draw(info);

}


void Start_Panel::draw(sf::RenderWindow& window){
    sf::RectangleShape s (start_size); 
    s.setPosition (start_pos);
    s.setFillColor(panel_background_color); //EX
    s.setOutlineThickness(panel_thickness); //EX
    s.setOutlineColor(panel_border_color); //EX
    window.draw (s); 
    float start_text_pos_x = start_pos.x + start_size.x/2; //EX

    start_title.setString("Benvenuto su"); 
    start_title.setCharacterSize(start_size_subtitle); //EX
    start_title.setFillColor(text_color); //EX
    start_title.setOutlineThickness(text_thickness); //EX
    start_title.setOutlineColor(text_border_color); //EX
    auto b = start_title.getLocalBounds(); 
    start_title.setOrigin({b.position.x + b.size.x/2, b.position.y}); 
    start_title.setPosition({start_text_pos_x, start_pos.y + start_gap});             
    window.draw(start_title);

    start_title.setString("MINESWEEPER"); 
    start_title.setCharacterSize(font_size_mine_title); //EX 
    b = start_title.getLocalBounds(); 
    start_title.setOrigin({b.position.x + b.size.x/2, b.position.y}); 
    start_title.setPosition({start_text_pos_x,start_title.getPosition().y + start_size_subtitle + start_gap}); //ex         
    window.draw(start_title);

    start_title.setString("Versione E.0"); 
    start_title.setCharacterSize(start_size_text); //EX
    start_title.setFillColor(focus_color);
    b = start_title.getLocalBounds(); 
    start_title.setOrigin({b.position.x + b.size.x/2, b.position.y}); 
    start_title.setPosition({start_text_pos_x,start_title.getPosition().y + font_size_mine_title + start_gap}); //ex         
    window.draw(start_title);

    start_title.setString("Scegliere la difficolta' di gioco"); 
    start_title.setFillColor(text_color);
    start_title.setCharacterSize(start_size_subtitle); //ex
    b = start_title.getLocalBounds(); 
    start_title.setOrigin({b.position.x + b.size.x/2, b.position.y}); 
    start_title.setPosition({start_text_pos_x,start_title.getPosition().y + start_size_text + start_gap}); //EX
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
        //AGGIUNTA
        title.setString("MINESWEEPER"); 
        title.setCharacterSize(font_size_mine_title);
        title.setFillColor(text_color); 
        title.setOutlineThickness(text_thickness);
        title.setOutlineColor(text_border_color); 
        auto b = title.getLocalBounds(); 
        title.setOrigin({b.position.x + b.size.x/2, b.position.y}); 
        title.setPosition({window_width/2, title_gap});
        window.draw(title);

        stop_p.draw(window); 
    }
}

////////////////ALTRE FUNZIONI////////////////

void Flag_Counter::set_number(bool flag_placed){ 
    if(num_flag == 0) return; //ex 
    if(flag_placed)  //EX MODIFICA 
        num_flag--; 
    else 
        num_flag++;  
    flag_numbers[2].num_texture = &Clock_textures[num_flag%10];
    flag_numbers[1].num_texture = &Clock_textures[(num_flag/10)%10];
    flag_numbers[0].num_texture = &Clock_textures[(num_flag/100)%10];
}

void Timer::update(float elapsed){
    if(!isRunning) return; 
    acc += elapsed; 
    while(acc >= 1){ 
        acc -= 1; 
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
    int casual_index;//EX

    int i=0; 
    while(i<mine_num){ 
        casual_index = (rand()% cells.size()+1)-1;

        if(abs(cells[casual_index].row_index - x)<=1 && abs(cells[casual_index].column_index - y)<=1) 
            continue;
        
        else if(cells[casual_index].cell_type == cell_type::Mine) 
            continue;

        else{
            cells[casual_index].cell_type = cell_type::Mine; 
            i++; 
        }
    }

    for (Cell& c : cells) c.mine_adj = 0;

    int num_y; //EX

    for(int j=0; j<cells.size(); j ++){

        if(cells[j].cell_type == cell_type::Mine){
            x = cells[j].row_index; 
            y = cells[j].column_index; 
            num_y = cell_num.y; //EX

            if(x > 0){
                cells[j-num_y].mine_adj++; 
                if(y > 0) cells[j-num_y-1].mine_adj++; 
                if(y < (num_y-1)) cells[j-num_y+1].mine_adj++;
            }

            if(x < (cell_num.x-1)){
                cells[j+num_y].mine_adj++; 
                if(y > 0) cells[j+num_y-1].mine_adj++;
                if(y < (num_y-1)) cells[j+num_y+1].mine_adj++; 
            }

            if(y > 0) cells[j-1].mine_adj++; 
            if(y < (num_y-1)) cells[j+1].mine_adj++; 
        }
    }
}

void Grid::place_numbers(){
    for(int i=0; i<cells.size();i++){
        if(cells[i].cell_type == cell_type::Mine) continue; 
        else if(cells[i].mine_adj == 0) cells[i].cell_type = cell_type::Empty; 
        else cells[i].cell_type = cell_type::Number; 
    }
}

void State::ending_reveal(Grid& g, int starting_index_cell){

    for(int i = 0; i < g.cells.size(); i++){
        if(stop_p.type == stop_type::Lose && i == starting_index_cell) continue; 

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
    game_paused = false; 
    game_panel.header.timer.isRunning = false; 
    stop_p.time = game_panel.header.timer.real_timer; 
    stop_p.visible = true; 
    
}

void State::flood_reveal(Grid& g, int starting_index_cell, Cell& start_c){

    int cols = g.cell_num.y; 
    int rows = g.cell_num.x; 
    int up_index = starting_index_cell - cols; //EX
    int down_index = starting_index_cell + cols; 
    int start_col = start_c.column_index; 
    int start_row = start_c.row_index; 

    if(start_row > 0){
        if(g.cells[up_index].cell_type != cell_type::Mine) reveal(g, up_index); 

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
    if(c.cell_state == cell_state::Flag) game_panel.header.f_counter.set_number(false);
    c.cell_state = cell_state::Revealed; 
    game_panel.grid.num_revealed++; 

    if(c.cell_type == cell_type::Mine){
        c.cell_texture = &Exploded_Mine_texture;
        stop_p.type = stop_type::Lose; 
        ending_reveal(g,starting_index_cell); 
        game_panel.header.face.face_texture = &lost_face_texture; 
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
        stop_p.type = stop_type::Win;
        ending_reveal(g, starting_index_cell); 
        game_panel.header.face.face_texture = &win_face_texture; 
    }
}

void State::reset(){
    game_panel = Game_Panel(game_panel.grid.cell_num, game_panel.grid.mine_num);  
    stop_p = Stop_Panel(); 
    game_paused = focus = game_ended= false; 
    first_move = true; 
    mouse_cell = -1; 
}


void State::pause(){
    if(!first_move) game_panel.header.timer.isRunning = false; 
    stop_p.time = game_panel.header.timer.real_timer;
    game_paused = true; 
    if(stop_p.type != stop_type::New_Game) stop_p.type = stop_type::Pause; 
    stop_p.visible = true; 
}

void State::restart(){
    stop_p.visible = false;
    stop_p.type = stop_type::None; 
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

void handle_resize(const sf::Event::Resized& resized, sf::RenderWindow& window, State& state)
{
    float aspect = static_cast<float>(window_width) / static_cast<float>(window_height);
    sf::Vector2u ws = resized.size;
    float new_aspect = static_cast<float>(ws.x) / static_cast<float>(ws.y);
    if (new_aspect < aspect)
        ws = {ws.x,static_cast<unsigned>(ws.x/aspect)};
    else
        ws = {static_cast<unsigned>(ws.y*aspect),ws.y};
    window.setSize(ws);

    sf::View view(sf::FloatRect({0.f, 0.f}, {static_cast<float>(window_width), static_cast<float>(window_height)}));
    window.setView(view);
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

void handle_mouse_pressed (const sf::Event::MouseButtonPressed& mouse, sf::RenderWindow& window, State& state)
{
    sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Vector2i(mouse.position.x, mouse.position.y));

    if(state.game_ended || state.game_paused ||state.sp.visible)
    {
        if (mouse.button != sf::Mouse::Button::Left) return;

        if(state.sp.visible){
            if (state.sp.easy_cb.cb_bounds.contains(mouse_pos)){
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
        //EX MODIFCA 
        else if (state.stop_p.type == stop_type::New_Game){
            if (state.stop_p.easy_cb.cb_bounds.contains(mouse_pos)) 
                state.set_difficulty(Difficulty::easy); 

            else if (state.stop_p.medium_cb.cb_bounds.contains(mouse_pos))
                state.set_difficulty(Difficulty::medium);

            else if (state.stop_p.hard_cb.cb_bounds.contains(mouse_pos))
                state.set_difficulty(Difficulty::hard);
        }

        else if(state.stop_p.new_game_cb.cb_bounds.contains(mouse_pos)) 
            state.stop_p.type = stop_type::New_Game; 

        else if(state.stop_p.exit_cb.cb_bounds.contains(mouse_pos)) 
            state.exit(); 
        
        return;        
    }

//PARTE ATTIVA DEL GIOCO 
    if( mouse.button == sf::Mouse::Button::Left){
        
        if(state.cp.new_game.cb_bounds.contains(mouse_pos)){
            state.stop_p.type = stop_type::New_Game; 
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

        if(state.mouse_cell <0 || state.mouse_cell >= state.game_panel.grid.cells.size()) return; 

        if(state.game_panel.grid.cells[state.mouse_cell].cell_state == cell_state::Revealed) return; 

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

        if(state.mouse_cell <0 || state.mouse_cell >= state.game_panel.grid.cells.size()) return;
        
        Cell& cur_cell = state.game_panel.grid.cells[state.mouse_cell];

        if(cur_cell.cell_state == cell_state::Revealed) return;

        if(cur_cell.cell_state != cell_state::Flag){
            state.game_panel.header.f_counter.set_number(true); 
            cur_cell.cell_state = cell_state::Flag; 
            cur_cell.cell_texture = &Flag_texture;
        }
        else{
            state.game_panel.header.f_counter.set_number(false); 
            cur_cell.cell_state = cell_state::Covered; 
            cur_cell.cell_texture = &Covered_texture;
        }       
    }
}

void handle (const sf::Event::MouseButtonReleased& mouse, State& state)
{
    if(state.game_ended || state.game_paused || state.game_panel.header.face.face_texture != &Click_face_texture) return; 
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
        state.sp.easy_cb.mouse_focus = state.sp.easy_cb.cb_bounds.contains(mouse_float_pos); 
        state.sp.medium_cb.mouse_focus = state.sp.medium_cb.cb_bounds.contains(mouse_float_pos); 
        state.sp.hard_cb.mouse_focus = state.sp.hard_cb.cb_bounds.contains(mouse_float_pos);
        return;
    }

    if(state.game_ended || state.game_paused){
        state.stop_p.new_game_cb.mouse_focus = state.stop_p.new_game_cb.cb_bounds.contains(mouse_float_pos); 
        state.stop_p.easy_cb.mouse_focus = state.stop_p.easy_cb.cb_bounds.contains(mouse_float_pos); 
        state.stop_p.medium_cb.mouse_focus = state.stop_p.medium_cb.cb_bounds.contains(mouse_float_pos); 
        state.stop_p.hard_cb.mouse_focus = state.stop_p.hard_cb.cb_bounds.contains(mouse_float_pos); 
        state.stop_p.exit_cb.mouse_focus = state.stop_p.exit_cb.cb_bounds.contains(mouse_float_pos);  
        return; 
    }

    state.cp.new_game.mouse_focus = state.cp.new_game.cb_bounds.contains(mouse_float_pos);  
    state.cp.pause.mouse_focus = state.cp.pause.cb_bounds.contains(mouse_float_pos); 
    state.cp.exit.mouse_focus = state.cp.exit.cb_bounds.contains(mouse_float_pos); 

    int new_idx =-1; 
    for (int i = 0; i < state.game_panel.grid.cells.size(); ++i) {
        if (state.game_panel.grid.cells[i].cell_bounds.contains(mouse_float_pos)){ 
            new_idx = i; 
            break; 
        }
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

    sf::RenderWindow window (sf::VideoMode ({window_width, window_height}), window_title, sf::Style::Default, sf::State::Windowed); //EX MODIFICA 
    window.setFramerateLimit (max_frame_rate);

    sf::RectangleShape border({(window_width - window_border_thickness*2),(window_height- window_border_thickness*2)}); //EX
    border.setPosition({window_border_thickness, window_border_thickness}); //EX 
    border.setFillColor(sf::Color::Transparent);  
    border.setOutlineThickness(window_border_thickness); //EX MODIFICA
    border.setOutlineColor(sf::Color(0, 100, 0));

    State state;
    sf::Clock Clock; 
    float elapsed;

    while (window.isOpen()) 
    {
        window.handleEvents (
                            [&window](const sf::Event::Closed&) { handle_close (window); },
                            [&window, &state](const sf::Event::Resized& event) { handle_resize (event, window, state); }, 
                            [&window, &state](const sf::Event::MouseButtonPressed& event) { handle_mouse_pressed (event, window, state); },
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