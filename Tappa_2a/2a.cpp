#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib> 
#include <ctime> 
#include <cmath>
#include "../textures_fonts.hpp"

using namespace std; 

////////////////FINESTRA////////////////
const char* window_title = "clock, flags, faces and borders";
const unsigned window_width = 1200;
const unsigned window_height = 900;
const float max_frame_rate = 60;

////////////////GRIGLIA////////////////

const float wall_horizontal_displacement = 100; 
const float wall_vertical_displacement = 100; 

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
    sf::Texture texture;
    cell_type type; 
    int mine_adj;
    cell_state state;
    
    Cell (sf::Vector2f pos, float size, int row_index, int column_index) : pos (pos),
                                                  size (size),
                                                  row_index(row_index),
                                                  column_index(column_index),
                                                  bounds (pos, {size, size}),
                                                  mouse_focus(false),
                                                  texture (Covered_texture),
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
    int mine_num; 
    int num_flag; 
    int num_revealed; 

    Grid (sf::Vector2i cell_num, int mine_num); 
    void place_mines(int starting_index_cell); 
    void place_numbers(); 
    void draw (sf::RenderWindow& window);
};

struct Restart{ 
    sf::Text title{font}; 
    bool visible; 
    bool victory; 
    
    Restart (): 
                visible(false), 
                victory(false) {}
                
    void draw (sf::RenderWindow& window);
}; 

struct State  
{
    Grid grid;
    Restart res; 
    int mouse_cell; 
    bool focus; 
    bool pause; 
    bool first_move; 
    bool game_ended;  

    State (): 
                grid({9,9}, 15), 
                res(),
                focus(false), 
                pause(true), 
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

Grid::Grid (sf::Vector2i bs, int bn){
    cell_num = bs; 
    mine_num = bn; 
    num_flag = num_revealed = 0; 

    float cell_size = ((window_height - (wall_vertical_displacement * 2)) / cell_num.y) * 0.85f;
 
    Grid_size = {cell_size * cell_num.x, cell_size * cell_num.y};
 
    sf::Vector2f start_pos = {
        window_width - Grid_size.x - wall_horizontal_displacement,
        (window_height - Grid_size.y) / 2.0f
    };
    sf::Vector2f pos; 
    
    for (unsigned hb = 0; hb < cell_num.x; hb++) {
        for (unsigned vb = 0; vb < cell_num.y; vb++) {
            pos = {
                start_pos.x + hb * (cell_size + 2.f), 
                start_pos.y + vb * (cell_size + 2.f)
            };
            cells.push_back(Cell(pos, cell_size, hb, vb)); 
        }
    } 
}

////////////////DRAW////////////////

void Cell::draw (sf::RenderWindow& window)
{
    sf::RectangleShape c ({size,size});
    c.setTexture(&texture);
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


void Restart::draw(sf::RenderWindow& window){
    if(!visible) return; 

    sf::RectangleShape s({600.f, 400.f}); 
    s.setPosition({(window_width - s.getSize().x)/2.f, (window_height - s.getSize().y)/2.f}); 
    s.setFillColor(sf::Color(210,180,140)); 
    s.setOutlineThickness(20.f); 
    s.setOutlineColor(sf::Color(92,51,23));
    window.draw(s); 

    
    title.setString(victory ? "Hai vinto!" : "Hai perso!"); 
    title.setCharacterSize(140); 
    title.setFillColor(sf::Color::Black); 
    title.setOutlineThickness(2.f); 
    title.setOutlineColor(sf::Color::White); 
    auto b = title.getLocalBounds(); 
    title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y}); 
    title.setPosition({s.getPosition().x + s.getSize().x/2.f, s.getPosition().y + s.getSize().y/2.f - 160.f});                  
    window.draw(title);

    title.setString("Premere SPACE");
    title.setCharacterSize(40);
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
    title.setPosition({s.getPosition().x + s.getSize().x/2.f,s.getPosition().y + s.getSize().y/2.f + 20.f});
    window.draw(title);

    title.setString("per cominciare una nuova partita");
    title.setCharacterSize(40);
    b = title.getLocalBounds();
    title.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
    title.setPosition({s.getPosition().x + s.getSize().x/2.f,s.getPosition().y + s.getSize().y/2.f + 80.f});
    window.draw(title);
}

void State::draw (sf::RenderWindow& window)
{
    grid.draw (window);
    res.draw(window); 
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

    if(res.victory == false && i == starting_index_cell ) continue; 

            if(g.cells[i].state == cell_state::Flag && g.cells[i].type != cell_type::Mine){
                g.cells[i].state = cell_state::Revealed; 
                g.cells[i].texture = False_Mine_texture; 
            }

            if(g.cells[i].type == cell_type::Mine){
                g.cells[i].state = cell_state::Revealed; 
                g.cells[i].texture = Normal_Mine_texture; 
            }
    }

    game_ended = true; 

    res.visible = true; 
    
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
    if(c.state == cell_state::Flag) g.num_flag --; 
    c.state = cell_state::Revealed; 
    grid.num_revealed++; 

    if(c.type == cell_type::Mine){
        c.texture = Exploded_Mine_texture;
        res.victory = false;  
        ending_reveal(g,starting_index_cell); 
        return; 
    } 
    else if(c.type == cell_type::Number){
        c.texture = Number_textures[c.mine_adj-1]; 
    }
    else{
        c.texture = Empty_texture; 
        flood_reveal(g, starting_index_cell, c);
    }

    if (grid.num_revealed == static_cast<int>(g.cells.size()) - g.mine_num) {
        res.victory = true;
        ending_reveal(g, starting_index_cell); 
    }
}

void State::reset(){
    res = Restart(); 
    grid = Grid({9,9}, 15);  
    focus = game_ended= false; 
    pause = first_move = true; 
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
    state.pause = true; 
    state.focus = false; 
}

void handle (const sf::Event::MouseButtonPressed& mouse, State& state)
{
    if(state.game_ended) return; 

    if(state.mouse_cell <0 || state.mouse_cell >= state.grid.cells.size()) return; 

    if(state.grid.cells[state.mouse_cell].state == cell_state::Revealed) return; 

    if( mouse.button == sf::Mouse::Button::Left){

        if(state.first_move){
            state.first_move = false; 
            state.grid.place_mines(state.mouse_cell); 
            state.grid.place_numbers(); 
            state.reveal(state.grid, state.mouse_cell); 
        }
        else state.reveal(state.grid, state.mouse_cell); 
    }

    if(mouse.button == sf::Mouse::Button::Right){
        if(state.first_move) return; 
        state.grid.num_flag++;

        state.grid.cells[state.mouse_cell].state = cell_state::Flag; 
        state.grid.cells[state.mouse_cell].texture = Flag_texture;
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
    for (int i = 0; i < state.grid.cells.size(); ++i) {
        if (state.grid.cells[i].bounds.contains(mouse_float_pos)) { new_idx = i; break; }
    }

    if (new_idx == state.mouse_cell) return;
 
    if (state.mouse_cell >= 0)
        state.grid.cells[state.mouse_cell].mouse_focus = false;

    state.mouse_cell = new_idx;
    if (state.mouse_cell >= 0)
        state.grid.cells[state.mouse_cell].mouse_focus = true;
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