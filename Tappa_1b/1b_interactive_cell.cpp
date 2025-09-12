#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib> 
#include <ctime> 
#include <cmath>
#include "../textures_fonts.hpp"

using namespace std; 

////////////////FINESTRA////////////////
const char* window_title = " Interactive cells";
const unsigned window_width = 1200;
const unsigned window_height = 900;
const float max_frame_rate = 60;

////////////////GRIGLIA////////////////

const float grid_horizontal_displacement = 100; 
const float grid_vertical_displacement = 100; 

////////////////BLOCCO////////////////

enum class cell_type{Mine,Empty, Number}; 
enum class cell_state{ Covered, Revealed, Flag}; 

////////////////STRUCT////////////////
struct Cell
{
    sf::Vector2f pos; 
    float size; 
    int row_index, column_index; //AGGIUNTA: indice riga e indice colonna della cella nella griglia 
    sf::FloatRect bounds; //AGGIUNTA: confini della cella 
    bool mouse_focus; //AGGIUNTA: indica se il mouse si trova o meno sopra alla cella 
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
    int mine_num; //AGGIUNTA: numero di mine 'nascoste' nella griglia 
    //AGGIUNTA: numero di bandierine piazzate nella griglia
    int num_flag = 0; //inizialmente il numero di badierina sarà sempre zero

    Grid (sf::Vector2i cell_num, int mine_num); //MODIFICA: aggiunto il numero di mine poichè anche quello varia in base alla difficoltà
    void place_mines(int starting_index_cell); //AGGIUNTA: funzione per il piazzamento casuale delle mine nella griglia post click di inizio partita
    void place_numbers(); //AGGIUNTA: funzione per dare la corretta texture alle celle vuote in base al numero di mine adiacenti 
    void draw (sf::RenderWindow& window);
};

struct State  
{
    Grid grid; 
    int mouse_cell; //AGGIUNTA: posizione del mouse nella 

    bool focus; //AGGIUNTA: indica se la finestra di gioco ha il focus o meno 
    bool pause; //AGGIUNTA: variabile che indica se il gioco è in pausa 
    bool first_move; //AGGIUNTA: indica se è stata o meno fatta la prima mossa di gioco 

    State () : 
            grid({9,9}, 15), //per ora il numero di minea sarà sempre 15 e di celle 9x9  
            focus(false), 
            pause(true), 
            first_move(true),
            mouse_cell(-1) {}  
    
    void reveal(Grid& g, int starting_index_cell); //AGGIUNTA: funzione per la rivelazione di una cella
    void flood_reveal(Grid& g, int starting_index_cell, Cell& start_c); //AGGIUNTA: rivelazione a cascata 
    void draw (sf::RenderWindow& window);
};

////////////////CREAZIONE////////////////

Grid::Grid (sf::Vector2i bs, int bn){
    cell_num = bs; 
    mine_num = bn; //AGGIUNTA: imposto il numero di mine che saranno nella griglia 

    float cell_size = ((window_height - (grid_vertical_displacement * 2)) / cell_num.y) * 0.85f;
 
    Grid_size = {cell_size * cell_num.x, cell_size * cell_num.y};
 
    sf::Vector2f start_pos = {
        window_width - Grid_size.x - grid_horizontal_displacement,
        (window_height - Grid_size.y) / 2.0f
    };
    sf::Vector2f pos; 
    
    for (unsigned hb = 0; hb < cell_num.x; hb++) {
        for (unsigned vb = 0; vb < cell_num.y; vb++) {
            pos = {
                //MODIFICA: aggiunta di un leggero gap (2.f) per rendere visibile il bordo delle celle 
                start_pos.x + hb * (cell_size + 2.f), 
                start_pos.y + vb * (cell_size + 2.f)
            };
            //MODIFICA: aggiunta dei parametri di row_index e column_index
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

    //AGGIUNTA: aggiunta la caratteristica del bordo della cella che si illumina al passaggio del mouse 
    //se la cella ha il focus imposta un bordo rosso spesso 2.f
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

//AGGIUNTA: funzione per il piazzamento casuale delle mine nella griglia post click di inizio partita
void Grid::place_mines(int starting_cell_index){

    int x = cells[starting_cell_index].row_index; 
    int y = cells[starting_cell_index].column_index;
    int casual_index; 

    //per il piazzamento casuale delle mine
    srand(time(NULL)); 

    //ciclo per il piazzamento casuale nella griglia di esattamente mine_num mine
    int i=0; 
    while(i<mine_num){ 
        casual_index = (rand()% cells.size()+1)-1; //scelta cella casuale per la mina 

        //Per la regola del gioco, non vi posso essere mine intorno e nella cella scelta come inizio quindi in quel caso bisogna scegliere un'altra cella 
        if(abs(cells[casual_index].row_index -x)<=1 && abs(cells[casual_index].column_index - y)<=1) 
            continue;
        
        //nel caso sia stata scelta una cella con già una mina se ne deve scegliere un'altra 
        else if(cells[casual_index].type == cell_type::Mine) 
            continue;

         //sennò cambia il tipo della cella e incrementa il numero di mine inserito 
        else{
            cells[casual_index].type = cell_type::Mine; 
            i++; 
        }
    }

    //reset del numero di mine adiacenti a zero per ogni cella per sicurezza 
    for (Cell& c : cells) c.mine_adj = 0;

    //ciclo per il calcolo del numero di mine adiacenti ad ogni cella 
    for(int j=0; j<cells.size(); j ++){

        //se la cella considerata nasconde una mina
        if(cells[j].type == cell_type::Mine){
            //riuso x e y da prima in modo da avere i valori di row_index e column_index direttamente senza però utilizzare ulteriore nuova memoria 
            x = cells[j].row_index; 
            y = cells[j].column_index; 

            //se la cella considerata non è nel bordo in alto della griglia 
            if(x > 0){
                //incrementa mine_adj della cella sopra
                cells[j-cell_num.y].mine_adj++; 

                //se la cella non è nel bordo sinistro incrementa mine_adj della cella sopra a sinistra
                if(y > 0) cells[j-cell_num.y-1].mine_adj++; 
                //se la cella non è nel bordo destro incrementa mine_adj della cella sopra a destra
                if(y < (cell_num.y-1)) cells[j-cell_num.y+1].mine_adj++;
            }

            //lo stesso ragionamento di prima se la cella non è nel bordo in basso 
            if(x < (cell_num.x-1)){
                cells[j+cell_num.y].mine_adj++; 
                if(y > 0) cells[j+cell_num.y-1].mine_adj++;
                if(y < (cell_num.y-1)) cells[j+cell_num.y+1].mine_adj++; 
            }

            //se la cella non è nel bordo sinistro incrementa mine_adj della cella alla sua sinistra
            if(y > 0) cells[j-1].mine_adj++; 
            //se la cella non è nel bordo destro incrementa mine_adj della cella alla sua destra
            if(y < (cell_num.y-1)) cells[j+1].mine_adj++; 
        }
    }
}

//AGGIUNTA: impostazione del tipo corretto per ogni cella 
void Grid::place_numbers(){
    //per ogni cella 
    for(int i=0; i<cells.size();i++){
        //se ha come tipo Mine non fare nulla
        if(cells[i].type == cell_type::Mine) continue; 
        //se il numero di mine adiacenti alla cella considerata è zero metti il tipo Empty 
        if(cells[i].mine_adj == 0){
            cells[i].type = cell_type::Empty; 
        }
        // in caso contrario metti il tipo Number 
        else cells[i].type = cell_type::Number; 
    }
}

//AGGIUNTA: funzione per il rivelamento a cascata delle celle nel caso di una cella Empty
void State::flood_reveal(Grid& g, int starting_index_cell, Cell& start_c){

    //se la cella considerata non è nel bordo in alto della griglia
    if(start_c.row_index > 0){
        //rivela la cella sopra a quella considerata solo nel caso detta cella non sia di tipo Mine 
        if(g.cells[starting_index_cell-g.cell_num.y].type !=  cell_type::Mine) reveal(g, starting_index_cell-g.cell_num.y); 

        //se la cella considerata non è nemmeno nel bordo sinistro della griglia e la cella in alto a sinistra della cella considerata non è di tipo Mine, rivela la cella in alto a sinistra
        if((start_c.column_index > 0) && (g.cells[starting_index_cell-g.cell_num.y-1].type != cell_type::Mine) ) reveal(g, starting_index_cell-g.cell_num.y-1); 

        //se la cella considerata non è nemmeno nel bordo destro della griglia e la cella in alto a destra della cella considerata non è di tipo Mine, rivela la cella in alto a destra
        if((start_c.column_index < (g.cell_num.y-1)) && (g.cells[starting_index_cell-g.cell_num.y+1].type != cell_type::Mine)) reveal(g, starting_index_cell-g.cell_num.y+1);
    }

    /*fai lo stesso ragionamento dei bordi di prima ma per: 
    - bordo in basso 
    - bordo basso e sinistro 
    - bordo basso e destro
    */
    if(start_c.row_index < (g.cell_num.x-1)){
        if(g.cells[starting_index_cell+g.cell_num.y].type !=  cell_type::Mine) reveal(g, starting_index_cell+g.cell_num.y);

        if((start_c.column_index > 0) && (g.cells[starting_index_cell+g.cell_num.y-1].type != cell_type::Mine)) reveal(g, starting_index_cell+g.cell_num.y-1);

        if((start_c.column_index < (g.cell_num.y-1)) && (g.cells[starting_index_cell+g.cell_num.y+1].type != cell_type::Mine)) reveal(g, starting_index_cell+g.cell_num.y+1);        
    }

    //se la cella considerata non è nel bordo sinistro della griglia e la cella a sinistra della cella considerata non è di tipo Mine, rivela la cella a sinistra 
    if((start_c.column_index > 0) && (g.cells[starting_index_cell-1].type != cell_type::Mine)) reveal(g, starting_index_cell-1);

    //se la cella considerata non è  nel bordo destro della griglia e la cella a destra della cella considerata non è di tipo Mine, rivela la cella a destra
    if((start_c.column_index < (g.cell_num.y-1)) && (g.cells[starting_index_cell+1].type != cell_type::Mine)) reveal(g, starting_index_cell+1);
}

//AGGIUNTA: funzione per la rivelazione di una sola cella nella griglia
void State::reveal(Grid& g, int starting_index_cell){

    //salvataggio della cella
    Cell& c = g.cells[starting_index_cell]; 

    //se la cella aveva già lo stato Revealed non fare nulla 
    if(c.state == cell_state::Revealed) return; 
    //se la cella aveva lo stato Flag diminuisci il numero di bandierine presenti in gioco 
    if(c.state == cell_state::Flag) g.num_flag --; 
    //imposto la cella come Revealed essendo che viene rivelata 
    c.state = cell_state::Revealed; 

    //se la cella è di tipo Mine il gioco dovrebbe finire (per ora si imposta solo la texture della mina esplosa per la cella e si fa return)
    if(c.type == cell_type::Mine){
        c.texture = Exploded_Mine_texture;
        return; 
    } 
    //se la cella è di tipo Number imposto come sua texture quella corrispondente al numero di bombe adiacenti alla cella rivelata e faccio return
    else if(c.type == cell_type::Number){
        c.texture = Number_textures[c.mine_adj-1]; //-1 perchè l'array inizia con l'indice zero 
        return;
    }
    //se la cella è di tipo Empty imposto come sua texture quella della cella vuota e faccio una rivelazione a cascata 
    else{
        c.texture = Empty_texture; 
        flood_reveal(g, starting_index_cell, c);
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

//AGGIUNTA: versione base della funzione di gestione eventi in modo che se accade un evento non programmato non succeda nulla 
template <typename T>
void handle (T& event, State& state) {}

//AGGIUNTA: gestione dell'evento caratterizzato dalla finestra di gioco che prende il focus
void handle (const sf::Event::FocusGained&, State& state)
{
    state.focus = true; 
}

//AGGIUNTA: gestione dell'evento caratterizzato dalla finestra di gioco che perde il focus
void handle (const sf::Event::FocusLost&, State& state)
{
    state.pause = true; 
    state.focus = false; 
}

//AGGIUNTA: gestione dell'evento caratterizzato dal click di un pulsante del mouse 
void handle (const sf::Event::MouseButtonPressed& mouse, State& state)
{
    //se la posizione del click del mouse è fuori dalla griglia di gioco non fare nulla 
    if(state.mouse_cell <0 || state.mouse_cell >= state.grid.cells.size()) return; 

    //se la posizione del click del mouse è una cella scoperta non fare nulla 
    if(state.grid.cells[state.mouse_cell].state == cell_state::Revealed) return; 

    //se è stato cliccato il tasto sinistro del mouse 
    if( mouse.button == sf::Mouse::Button::Left){

        //se è la mossa di inizio partita 
        if(state.first_move){
            state.first_move = false; //metti che è stata fatta la prima mossa 
            state.grid.place_mines(state.mouse_cell); //piazza le mine in base alla prima cella cliccata 
            state.grid.place_numbers(); //imposta il tipo di ogni cella 
            state.reveal(state.grid, state.mouse_cell); //rivela la cella cliccata 
        }
        else state.reveal(state.grid, state.mouse_cell); // in caso non sia la prima mossa, rivela solo la cella cliccata 
    }

    //se è stato cliccato il tasto destro del mouse 
    if(mouse.button == sf::Mouse::Button::Right){
        if(state.first_move) return; //se non è ancora stata fatta la prima mossa non fare nulla 
        state.grid.num_flag++; //aumenta il numero di bandierine in gioco 

        //cambia la texture e lo stato della cella cliccata in quello della bandierina
        state.grid.cells[state.mouse_cell].state = cell_state::Flag; 
        state.grid.cells[state.mouse_cell].texture = Flag_texture;
    }

}

//AGGIUNTA: gestione dell'evento caratterizzato dal movimento del mouse 
void handle (const sf::Event::MouseMoved& ev, State& state)
{
    //la posizione del mouse viene salvata 
    const sf::Vector2f mouse_float_pos{
        static_cast<float>(ev.position.x),
        static_cast<float>(ev.position.y)
    };


    //ricerca e salvataggio dell'indice della cella in cui si trova il mouse nella griglia (se non si trova nella griglia la posizione sarà -1, un indice impossibile per la griglia)
    int new_idx =-1;
    for (int i = 0; i < state.grid.cells.size(); ++i) {
        if (state.grid.cells[i].bounds.contains(mouse_float_pos)) { new_idx = i; break; }
    }

    //se il muose è rimasto nella stessa nella pur spostandosi non fare nulla (ad esempio con un micro movimento)
    if (new_idx == state.mouse_cell) return;

    //togli il mouse_focus dalla cella in cui si trovava precedentemente il mouse 
    if (state.mouse_cell >= 0)
        state.grid.cells[state.mouse_cell].mouse_focus = false;

    //metti il mouse_focus alla cella in cui si trova ora il mouse 
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
                             [&state] (const auto& event) { handle (event, state); } //handling dei nuovi possibili eventi
        );

        window.clear(sf::Color(144, 238, 144));
        state.draw(window); 
        window.draw(border); 
        window.display();

    }
}