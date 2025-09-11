#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib> 
#include <ctime> 
#include <cmath>
#include "../textures.hpp"

using namespace std; 

////////////////FINESTRA////////////////
const char* window_title = "Other parameters";
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

    Grid (sf::Vector2i cell_num, int mine_num); 
    void place_mines(int starting_index_cell); 
    void place_numbers(); 
    void reveal(int starting_index_cell); 
    void flood_reveal(int starting_index_cell, Cell& start_c); 
    void draw (sf::RenderWindow& window);
};

struct State  
{
    Grid grid; 
    int mouse_cell; 

    bool focus; 
    bool pause; 
    bool first_move; 

    State () : 
            grid({9,9}, 15), 
            focus(false), 
            pause(true), 
            first_move(true),
            mouse_cell(-1) {}  
    
    void draw (sf::RenderWindow& window);
};

////////////////CREAZIONE////////////////

Grid::Grid (sf::Vector2i bs, int bn){
    cell_num = bs; 
    mine_num = bn; 
    num_flag = 0; 

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

void State::draw (sf::RenderWindow& window)
{
    grid.draw (window);
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

void Grid::flood_reveal(int starting_index_cell, Cell& start_c){

    if(start_c.row_index > 0){
        if(cells[starting_index_cell-cell_num.y].type !=  cell_type::Mine) reveal(starting_index_cell-cell_num.y); 

        if((start_c.column_index > 0) && (cells[starting_index_cell-cell_num.y-1].type != cell_type::Mine) ) reveal(starting_index_cell-cell_num.y-1); 

        if((start_c.column_index < (cell_num.y-1)) && (cells[starting_index_cell-cell_num.y+1].type != cell_type::Mine)) reveal(starting_index_cell-cell_num.y+1);
    }

    if(start_c.row_index < (cell_num.x-1)){
        if(cells[starting_index_cell+cell_num.y].type !=  cell_type::Mine) reveal(starting_index_cell+cell_num.y);

        if((start_c.column_index > 0) && (cells[starting_index_cell+cell_num.y-1].type != cell_type::Mine)) reveal(starting_index_cell+cell_num.y-1);

        if((start_c.column_index < (cell_num.y-1)) && (cells[starting_index_cell+cell_num.y+1].type != cell_type::Mine)) reveal(starting_index_cell+cell_num.y+1);        
    }

    if((start_c.column_index > 0) && (cells[starting_index_cell-1].type != cell_type::Mine)) reveal(starting_index_cell-1);

    if((start_c.column_index < (cell_num.y-1)) && (cells[starting_index_cell+1].type != cell_type::Mine)) reveal(starting_index_cell+1);
}

void Grid::reveal(int starting_index_cell){

    Cell& c = cells[starting_index_cell]; 

    if(c.state == cell_state::Revealed) return; 
    if(c.state == cell_state::Flag) num_flag --; 
    c.state = cell_state::Revealed; 

    if(c.type == cell_type::Mine){
        c.texture = Exploded_Mine_texture;
        return; 
    } 
    else if(c.type == cell_type::Number){
        c.texture = Number_textures[c.mine_adj-1]; 
        return;
    }
    else{
        c.texture = Empty_texture; 
        flood_reveal(starting_index_cell, c);
    }
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
    if(state.mouse_cell <0 || state.mouse_cell >= state.grid.cells.size()) return; 

    if(state.grid.cells[state.mouse_cell].state == cell_state::Revealed) return; 

    if( mouse.button == sf::Mouse::Button::Left){

        if(state.first_move){
            state.first_move = false; 
            state.grid.place_mines(state.mouse_cell); 
            state.grid.place_numbers(); 
            state.grid.reveal(state.mouse_cell); 
        }
        else state.grid.reveal(state.mouse_cell); 
    }

    if(mouse.button == sf::Mouse::Button::Right){
        if(state.first_move) return; 
        state.grid.num_flag++;

        state.grid.cells[state.mouse_cell].state = cell_state::Flag; 
        state.grid.cells[state.mouse_cell].texture = Flag_texture;
    }

}

void handle (const sf::Event::MouseButtonReleased& mouse, State& state)
{
    
}

void handle (const sf::Event::MouseMoved& ev, State& state)
{
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
    load_textures();
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
                             [&state] (const auto& event) { handle (event, state); } //handling dei nuovi possibili eventi
        );

        window.clear(sf::Color(144, 238, 144));
        state.draw(window); 
        window.draw(border); 
        window.display();

    }
}