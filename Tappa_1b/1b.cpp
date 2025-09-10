#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib> 
#include <ctime> 
#include <cmath>
#include <queue>

using namespace std; 

////////////////FINESTRA////////////////
const char* window_title = "Cell starting texture";
const unsigned window_width = 1200;
const unsigned window_height = 900;
const float max_frame_rate = 60;

////////////////GRIGLIA////////////////

const float wall_horizontal_displacement = 100; 
const float wall_vertical_displacement = 100; 


////////////////BLOCCO////////////////

enum class cell_type{Mine,Empty, Number}; 
enum class cell_state{ Covered, Revealed, Flag}; 
// celle coperte
sf::Texture Covered_texture("../risorse/texture/cells/cellup.jpg");
// bandierina
sf::Texture Flag_texture("../risorse/texture/cells/cellflag.jpg");
// mina
sf::Texture Normal_Mine_texture("../risorse/texture/cells/cellmine.jpg");
//mina esplosa 
sf::Texture Exploded_Mine_texture("../risorse/texture/cells/blast.jpg");
//falsa mina 
sf::Texture False_Mine_texture("../risorse/texture/cells/falsemine.jpg");
// celle vuote (0 adiacenti)
sf::Texture Empty_texture("../risorse/texture/cells/celldown.jpg");
// numeri (1–8)
sf::Texture Number_textures[8] = {
    sf::Texture("../risorse/texture/cells/cell1.jpg"),
    sf::Texture("../risorse/texture/cells/cell2.jpg"),
    sf::Texture("../risorse/texture/cells/cell3.jpg"),
    sf::Texture("../risorse/texture/cells/cell4.jpg"),
    sf::Texture("../risorse/texture/cells/cell5.jpg"),
    sf::Texture("../risorse/texture/cells/cell6.jpg"),
    sf::Texture("../risorse/texture/cells/cell7.jpg"),
    sf::Texture("../risorse/texture/cells/cell8.jpg")
};

////////////////STRUCT////////////////
struct Cell
{
    sf::Vector2f pos; 
    float size; 
    int row_index, column_index; 
    sf::FloatRect bounds; //indica la dimensione della cella 
    bool mouse_focus; //indica se il mouse si trova o meno sopra alla cella 
    sf::Texture texture;
    cell_type type; 
    int bomb_number;
    cell_state state;

    
    Cell (sf::Vector2f pos, float size, int row_index, int column_index) : pos (pos),
                                                  size (size),
                                                  row_index(row_index),
                                                  column_index(column_index),
                                                  bounds (pos, {size, size}),
                                                  mouse_focus(false),
                                                  texture (Covered_texture),
                                                  type(cell_type::Empty), 
                                                  bomb_number(0),
                                                  state(cell_state::Covered) {}
    void draw (sf::RenderWindow& window);
};

struct Grid
{
    vector<Cell> cells;  
    sf::Vector2i cell_num;  
    sf::Vector2f Grid_size;
    int bomb_num; 

    Grid (sf::Vector2i cell_num, int bomb_num); 
    void place_mines(int starting_index_cell); 
    void place_numbers(); 
    void draw (sf::RenderWindow& window);
};

struct State  
{
    Grid grid; 
    int mouse_cell;
    int num_flag; 

    bool focus; //variabile che indica se la finestra di gioco ha il focus o meno 
    bool pause; //variabile che indica se il gioco è il pausa 
    bool first_move; //indica se bisogna o meno fare il primo click

    State () : 
            grid({9,9}, 15), //per ora il numero di bomba sarà sempre 10 
            num_flag(0),
            focus(false), 
            pause(true), 
            first_move(true),
            mouse_cell(-1) {}  
    
    void reveal(Grid& g, int starting_index_cell);
    void flood_reveal(Grid& g, int starting_index_cell, Cell& start_c);
    void draw (sf::RenderWindow& window);
};

////////////////CREAZIONE////////////////

Grid::Grid (sf::Vector2i bs, int bn){
    cell_num = bs; 
    bomb_num = bn; //numero di bonbe che saranno nascoste nelle celle della griglia

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
                //aggiunta di un leggero gap per far vedere il bordo delle celle 
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

    int start_x = cells[starting_cell_index].row_index; 
    int start_y = cells[starting_cell_index].column_index;
    int casual_index; 

    srand(time(NULL)); 

    int i=0; 
    while(i<bomb_num){
        casual_index = (rand()% cells.size()+1)-1; 
        if(abs(cells[casual_index].row_index -start_x)<=1 && abs(cells[casual_index].column_index -start_y)<=1)   
            continue;
        else if(cells[casual_index].type == cell_type::Mine) continue; 
        else{
            cells[casual_index].type = cell_type::Mine; 
            i++; 
        }
    }

    for (auto& c : cells) c.bomb_number = 0;

    for(int j=0; j<cells.size(); j ++){
        if(cells[j].type == cell_type::Mine){
            if(cells[j].row_index > 0){
                cells[j-cell_num.y].bomb_number++; 
                if(cells[j].column_index > 0) cells[j-cell_num.y-1].bomb_number++; 
                if(cells[j].column_index < (cell_num.y-1)) cells[j-cell_num.y+1].bomb_number++;
            }
            if(cells[j].row_index < (cell_num.x-1)){
                cells[j+cell_num.y].bomb_number++; 
                if(cells[j].column_index > 0) cells[j+cell_num.y-1].bomb_number++;
                if(cells[j].column_index < (cell_num.y-1)) cells[j+cell_num.y+1].bomb_number++; 
            }

            if(cells[j].column_index > 0) cells[j-1].bomb_number++; 
            if(cells[j].column_index < (cell_num.y-1)) cells[j+1].bomb_number++; 
        }
    }
}

void Grid::place_numbers(){
    for(int i=0; i<cells.size();i++){
        if(cells[i].type == cell_type::Mine) continue; 
        if(cells[i].bomb_number == 0){
            cells[i].type = cell_type::Empty; 
        }
        else cells[i].type = cell_type::Number; 
    }
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

    //la cella viene rivelata quindi ne devo cambaire lo stato se non è già Revealed 
    if(c.state == cell_state::Revealed) return; 
    if(c.state == cell_state::Flag) num_flag --; 
    c.state = cell_state::Revealed; 

    //se la cella è una mina il gioco finisce (per ora solo return)
    if(c.type == cell_type::Mine){
        c.texture = Exploded_Mine_texture;
        return; 
    } 
    else if(c.type == cell_type::Number){
        c.texture = Number_textures[c.bomb_number-1]; 
        return;
    }
    else{
        c.texture = Empty_texture; 
        flood_reveal(g,starting_index_cell, c);
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

//versione base della funzione di gestione eventi in modo che se accade un evento non gestito non succeda nulla 
template <typename T>
void handle (T& event, State& state) {}

//la finestra di gioco prende il focus
void handle (const sf::Event::FocusGained&, State& state)
{
    state.focus = true; 
}

//la finestra di gioco perde il focus 
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
            state.first_move = false; //il primo click è stato fatto 
            state.grid.place_mines(state.mouse_cell);
            state.grid.place_numbers();
            state.reveal(state.grid, state.mouse_cell);
        }
        else state.reveal(state.grid, state.mouse_cell); 
    }

    if(mouse.button == sf::Mouse::Button::Right){
        if(state.first_move) return; 
        state.num_flag++; 
        state.grid.cells[state.mouse_cell].state = cell_state::Flag; 
        state.grid.cells[state.mouse_cell].texture = Flag_texture;
    }

}

void handle (const sf::Event::MouseButtonReleased& mouse, State& state)
{
    
}

void handle (const sf::Event::MouseEntered& mouse, State& state)
{
    
}

void handle (const sf::Event::MouseLeft&, State& state)
{
    if (state.mouse_cell >= 0) {
        state.grid.cells[state.mouse_cell].mouse_focus = false;
        state.mouse_cell = -1;
    }
}

void handle (const sf::Event::MouseMoved& ev, State& state)
{

    //la posizione del mouse salvata in float
    const sf::Vector2f mouse_float_pos{
        static_cast<float>(ev.position.x),
        static_cast<float>(ev.position.y)
    };

    int new_idx =-1;
    for (int i = 0; i < static_cast<int>(state.grid.cells.size()); ++i) {
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