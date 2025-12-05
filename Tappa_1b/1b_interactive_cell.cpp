#include <SFML/Graphics.hpp>
#include <vector> 
#include <string> 
#include "../risorse/textures_fonts.hpp"//file per le varie texture 

using namespace std; 

////////////////FINESTRA////////////////

const char* window_title = "Interactive Cells"; 
const unsigned window_width = 800; 
const unsigned window_height = 600;  
const float max_frame_rate = 60; 
const float window_horizontal_displacement = 100; 
const float window_vertical_displacement = 100;
sf::Color window_background_color = sf::Color(144, 238, 144); 

////////////////BORDO FINESTRA////////////////

const float window_border_thickness = 15; 
sf::Color window_border_color = sf::Color(0, 100, 0);

////////////////CELLA////////////////

enum class cell_type{Mine,Empty, Number}; 
enum class cell_state{ Covered, Revealed, Flag}; 
sf::Color focus_color = sf::Color::Red; //AGGIUNTA: colore di focus degli elementi al passaggio del mouse (per ora utilizzato solo per rendere il bordo delle celle rosso) al passaggio del mouse

////////////////STRUCT////////////////


struct Cell
{
    sf::Vector2f cell_pos; 
    float cell_size; 
    int row_index, column_index; //AGGIUNTA: indice riga e indice colonna della cella nella griglia 
    sf::FloatRect cell_bounds; //AGGIUNTA: confini della cella (posizione e dimensione)
    bool cell_mouse_focus; //AGGIUNTA: una cella prende il focus se il mouse si trova sopra di essa (cioè dentro i suoi confini)
    sf::Texture* cell_texture;
    cell_type cell_type; 
    int mine_adj; 
    cell_state cell_state;
    float cell_gap; //AGGIUNTA: spazio tra le varie celle per rendere ben visibile sia la texture delle celle sia il bordo che diventa rosso in caso di passaggio del mouse sulla cella


    Cell (sf::Vector2f pos, float size, int column_index, int row_index, float gap) : 
                                            cell_pos (pos),
                                            cell_size (size),
                                            row_index(row_index),
                                            column_index(column_index),
                                            cell_bounds (cell_pos, {cell_size, cell_size}),
                                            cell_mouse_focus(false), //inizialmente la cella non ha mai il focus quindi il valore di default è FALSE
                                            cell_texture (&Covered_texture), 
                                            cell_type(cell_type::Empty), 
                                            mine_adj(0), 
                                            cell_state(cell_state::Covered),
                                            cell_gap(gap) {}
    void draw (sf::RenderWindow& window); 
};

struct Grid
{
    std::vector<Cell> cells; 
    sf::Vector2i cell_num; 
    sf::Vector2f Grid_pos; 
    sf::Vector2f Grid_size; 
    int mine_num; //AGGIUNTA: numero di mine 'nascoste' nella griglia 

    Grid (sf::Vector2i cell_num, int mine_num, float& cell_size, float gap); //MODIFICATO: aggiunto il numero di mine poichè anche quello varia in base alla difficoltà. Viene inoltre passato il gap in modo da poterlo usare per il calcolo della size e per la funzione Draw
    void place_mines(int starting_index_cell); //AGGIUNTA: funzione per il piazzamento casuale delle mine nella griglia post click di inizio partita
    void place_numbers(); //AGGIUNTA: funzione per l'impostazione dello stato della cella nascosto corretto per ogni cella 
    void draw (sf::RenderWindow& window);
};


struct Game_Panel
{
    float gap; //AGGIUNTA: spazio tra le varie celle per rendere ben visibile sia la texture delle celle sia il bordo che diventa rosso in caso di passaggio del mouse sulla cella
    float cell_size; 
    Grid grid;  

    //MODIFICATO: aggiunto il numero di mine poichè anche quello varia in base alla difficoltà
    Game_Panel(sf::Vector2i cell_num, int mine_num):
                                        gap(2), //per ora il gap tra le celle sarà sempre 2
                                        cell_size((window_height - (window_vertical_displacement * 2) - (gap * (cell_num.x - 1))) / cell_num.y),
                                        grid(cell_num, mine_num, cell_size, gap) 
                                        {} 
    void draw (sf::RenderWindow& window);
};

struct State 
{
    Game_Panel game_panel;
    int mouse_cell; //AGGIUNTA: posizione del mouse nella griglia
    bool focus; //AGGIUNTA: indica se la finestra di gioco ha il focus o meno 
    bool first_move; //AGGIUNTA: indica se è stata o meno fatta la prima mossa in gioco 

    State () : 
            game_panel({9,9}, 15), //per ora il numero di mine sarà sempre 15 e di celle 9x9 poichè viene considerata solo la modalità FACILE 
            focus(false), 
            first_move(true),
            mouse_cell(-1) {}  

    void reveal(Grid& g, int starting_index_cell); //AGGIUNTA: funzione per la rivelazione di una cella qando essa viene cliccata
    void flood_reveal(Grid& g, int starting_index_cell, Cell& start_c); //AGGIUNTA: funzione per il rivelamento a cascata delle celle nel caso di una cella Empty o della prima cella
    void draw (sf::RenderWindow& window);
};

////////////////CREAZIONE////////////////

Grid::Grid (sf::Vector2i bs, int bn, float& cell_size, float gap){
    cell_num = bs; 
    mine_num = bn; //AGGIUNTA: imposto il numero di mine che saranno nella griglia 

    //MODIFICATO: per la dimensione della griglia vengono contati anche i gap tra le celle
    Grid_size = {
        (cell_size * cell_num.x) + (gap * (cell_num.x - 1)),
        (cell_size * cell_num.y) + (gap * (cell_num.y - 1))
    };

    Grid_pos = { 
        window_width - Grid_size.x - window_horizontal_displacement,
        (window_height - Grid_size.y) / 2
    };

    sf::Vector2f pos; 

    for (int row = 0; row < cell_num.y; row++) {
        for (int col = 0; col < cell_num.x; col++) {
            pos = {
                //MODIFICATO: viene contato il gap anche per la disposizione delle celle
                Grid_pos.x + col * (cell_size + gap), 
                Grid_pos.y + row * (cell_size + gap)
            };
            //MODIFICATO: aggiunta dei parametri di row_index e column_index calcolati nei for precedenti
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

    //AGGIUNTA: aggiunta la caratteristica del bordo della cella che diventa del colore specificato nella costante globale focus_color al passaggio del mouse 
    if(cell_mouse_focus){
        c.setOutlineThickness(cell_gap); //lo spessore del bordo sarà uguale al gap tra le celle
        c.setOutlineColor(focus_color); //il colore del bordo al passaggio del mouse cambia in quello di focus color 
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

void State::draw (sf::RenderWindow& window){
    game_panel.draw (window); 
}

////////////////ALTRE FUNZIONI////////////////

//AGGIUNTA: funzione per il piazzamento casuale delle mine nella griglia post click di inizio partita
void Grid::place_mines(int starting_cell_index){

    int x = cells[starting_cell_index].row_index; //riga nella griglia della cella cliccata 
    int y = cells[starting_cell_index].column_index; //colonna nella griglia della cella cliccata 
    int casual_index; //variabile che verrà utilizzata per salvare temporaneamente gli indici delle celle scelte casualmente per contenere le mine

    //Ciclo per il piazzamento casuale nella griglia di esattamente mine_num mine
    int i=0; 
    while(i<mine_num){ 
        casual_index = (rand()% cells.size()+1)-1; //scelta cella casuale di una delle celle tramite suo indice

        //Per la regola del gioco, non vi posso essere mine intorno e nella cella cliccata per prima dal giocatore. Se fosse stato pescato prima casualmente un indice di quelle determinate celle bisognerà sceglierne un nuovo indice 
        if(abs(cells[casual_index].row_index -x)<=1 && abs(cells[casual_index].column_index - y)<=1) 
            continue;
        
        //anche nel caso sia stato scelto un indice di una cella contenente già uan mina bisognerà sceglierne un altro
        else if(cells[casual_index].cell_type == cell_type::Mine) 
            continue;

         //In tutti gli altri casi la cella dell'indice scelto diventerà di tipo Mine
        else{
            cells[casual_index].cell_type = cell_type::Mine; 
            i++; 
        }
    }

    //reset del numero di mine adiacenti a zero per ogni cella per sicurezza 
    for (Cell& c : cells) c.mine_adj = 0;

    int num_cols; //variabile che verrà utilizzata nei cicli successivi

    //ciclo per il calcolo del numero di mine adiacenti ad ogni cella 
    for(int j=0; j<cells.size(); j ++){

        //se la cella considerata nasconde una mina
        if(cells[j].cell_type == cell_type::Mine){
            //riuso x e y da prima in modo da avere i valori di row_index e column_index direttamente senza dover utilizzare ulteriore nuova memoria per la dichiarazione di due nuove variabili
            x = cells[j].row_index; 
            y = cells[j].column_index; 
            num_cols = cell_num.x; //essendo che questo valore verrà riutilizzato spesso per il calcolo delle adiacenti lo salvo in una variabile dichiarata precedentemente 

            //se la cella considerata non è nella riga zero della griglia 
            if(x > 0){
                //incrementa mine_adj della cella sopra la cella considerata
                cells[j-num_cols].mine_adj++; 

                //se la cella non è ne nella riga zero ne nella colonna zero incrementa mine_adj della cella in alto a sinistra della cella considerata
                if(y > 0) cells[j-num_cols-1].mine_adj++; 
                
                //se la cella non è ne nella riga zero ne nell'ultima colonna della griglia incrementa mine_adj della cella in alto a destra della cella considerata
                if(y < (num_cols-1)) cells[j-num_cols+1].mine_adj++;
            }

            //si appica lo stesso ragionamento di prima ma nel caso in cui la cella non sia nell'ultima riga della griglia
            if(x < (cell_num.y-1)){
                cells[j+num_cols].mine_adj++; //basso
                if(y > 0) cells[j+num_cols-1].mine_adj++; //basso a sinistra
                if(y < (num_cols-1)) cells[j+num_cols+1].mine_adj++; //basso a destra
            }

            //se la cella non è nella colonna zero incrementa mine_adj della cella a sinistra della cella considerata
            if(y > 0) cells[j-1].mine_adj++; 
            //se la cella non è nell'ultima colonna della griglia incrementa mine_adj della cella a sinistra della cella considerata
            if(y < (num_cols-1)) cells[j+1].mine_adj++; 
        }
    }
}

//AGGIUNTA: funzione per l'impostazione dello stato della cella nascosto corretto per ogni cella 
void Grid::place_numbers(){
    //per ogni cella nella griglia 
    for(int i=0; i<cells.size();i++){
        //se la cella ha come tipo Mine non fare nulla (il suo stato nascosto è già stato impostato correttamente nella funzione precedente)
        if(cells[i].cell_type == cell_type::Mine) continue;

        //se il numero di mine adiacenti alla cella considerata è zero imposta Empty come stato nascosto
        if(cells[i].mine_adj == 0) cells[i].cell_type = cell_type::Empty; 

        // in caso contrario imposta Number come stato nascosto della cella
        else cells[i].cell_type = cell_type::Number; 
    }
}

//AGGIUNTA: funzione per il rivelamento a cascata delle celle nel caso di una cella Empty o della prima cella
void State::flood_reveal(Grid& g, int starting_index_cell, Cell& start_c){

    //variabili create per contenere valori che verranno utilizzati spesso nelle istruzioni successive in questa funzione 
    int cols = g.cell_num.x; //contiene il numero di colonne della griglia
    int rows = g.cell_num.y;  //contiene il numero di righe della griglia
    int up_index = starting_index_cell - cols; 
    int down_index = starting_index_cell + cols; 
    int start_col = start_c.column_index; 
    int start_row = start_c.row_index; 

    //se la cella considerata non è nella riga zero della griglia 
    if(start_row > 0){
        //se la cella sopra la cella considerata non ha tipo Mine, allora rivelala
        if(g.cells[up_index].cell_type != cell_type::Mine) reveal(g,up_index); 

        //se la cella considerata non è nemmeno nella colonna zero e la cella in alto a sinistra della cella considerata non ha tipo Mine, rivela la cella in alto a sinistra
        if((start_col > 0) && (g.cells[up_index-1].cell_type != cell_type::Mine)) reveal(g, up_index-1); 

        //se la cella considerata non è nemmeno nell'ultima colonna e la cella in alto a destra della cella considerata non ha come tipo Mine, rivela la cella in alto a destra
        if((start_col < (cols-1)) && (g.cells[up_index+1].cell_type != cell_type::Mine)) reveal(g, up_index+1);
    }

        //se la cella considerata non è nell'ultima della griglia 
    if(start_row < (rows-1)){
        //se la cella sotto la cella considerata non ha tipo Mine, allora rivelala
        if(g.cells[down_index].cell_type !=  cell_type::Mine) reveal(g, down_index);

        //se la cella considerata non è nemmeno nella colonna zero e la cella in basso a sinistra della cella considerata non ha tipo Mine, rivela la cella in basso a sinistra
        if((start_col > 0) && (g.cells[down_index-1].cell_type != cell_type::Mine)) reveal(g, down_index-1);

        //se la cella considerata non è nemmeno nell'ultima colonna e la cella in basso a destra della cella considerata non ha come tipo Mine, rivela la cella in basso a destra
        if((start_col < (cols-1)) && (g.cells[down_index+1].cell_type != cell_type::Mine)) reveal(g, down_index+1);        
    }

    //se la cella considerata non è nella colonna zero della griglia e la cella a sinistra della cella considerata non è di tipo Mine, rivela la cella a sinistra 
    if((start_col > 0) && (g.cells[starting_index_cell-1].cell_type != cell_type::Mine)) reveal(g, starting_index_cell-1);

    //se la cella considerata non è nell'ultima colonna della griglia e la cella a destra della cella considerata non è di tipo Mine, rivela la cella a destra
    if((start_col < (cols-1)) && (g.cells[starting_index_cell+1].cell_type != cell_type::Mine)) reveal(g, starting_index_cell+1);
}

//AGGIUNTA: funzione per la rivelazione di una sola cella nella griglia al click del mouse
void State::reveal(Grid& g, int starting_index_cell){

    //salvataggio della cella cliccata
    Cell& c = g.cells[starting_index_cell]; 

    //se la cella cliccata aveva già lo stato Revealed non fare nulla (era già stata rivelata)
    if(c.cell_state == cell_state::Revealed) return;
    //imposto la cella come Revealed essendo che viene rivelata
    c.cell_state = cell_state::Revealed; 

    //se la cella è di tipo Mine il gioco dovrebbe finire (per ora si imposta solo la texture della mina esplosa per la cella e si fa return)
    if(c.cell_type == cell_type::Mine){
        c.cell_texture = &Exploded_Mine_texture;
        return; 
    } 
    //se la cella è di tipo Number si imposta come sua texture quella corrispondente al numero di bombe adiacenti alla cella rivelata e si fa return
    else if(c.cell_type == cell_type::Number){
        c.cell_texture = &Number_textures[c.mine_adj-1]; //-1 perchè l'array inizia con l'indice zero 
        return;
    }
    //negli altri casi la cella è allora di tipo empty e si imposta come sua texture quella della cella vuota e si fa partire una rivelazione a cascata
    else{
        c.cell_texture = &Empty_texture; 
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

    sf::View view(sf::FloatRect({0.f, 0.f}, {static_cast<float>(window_width), static_cast<float>(window_height)})); //AGGIUNTA: gestito l'aumento della dimensione della finestra in modo che funzionino sempre il click sulle celle e il focus al passaggio del mouse
     window.setSize(ws); 

}

//AGGIUNTA: funzione per la gestione di eventi che non devono portare a nessuna azione in gioco
template <typename T>
void handle (T& event, State& state) {}

//AGGIUNTA: funzione per la gestione dell'evento caratterizzato dalla finestra di gioco che prende il focus
void handle (const sf::Event::FocusGained&, State& state)
{
    state.focus = true;
}

//AGGIUNTA: funzione per la getione dell'evento caratterizzato dalla finestra di gioco che perde il focus
void handle (const sf::Event::FocusLost&, State& state)
{
    state.focus = false; 
}

//AGGIUNTA: funzione gestione dell'evento caratterizzato dal click di un pulsante del mouse 
void handle (const sf::Event::MouseButtonPressed& mouse, State& state)
{
    //se la posizione del click del mouse è al di fuori dalla griglia di gioco non fare nulla 
    if(state.mouse_cell <0 || state.mouse_cell >= state.game_panel.grid.cells.size()) return; 

    Cell& cur_cell = state.game_panel.grid.cells[state.mouse_cell]; 

    //se la posizione del click del mouse è su una cella scoperta non fare nulla
    if(cur_cell.cell_state == cell_state::Revealed) return; 

    //se è stato cliccato il tasto sinistro del mouse 
    if( mouse.button == sf::Mouse::Button::Left){

        //se è la mossa di inizio partita 
        if(state.first_move){
            state.first_move = false; //imposta che è stata fatta la prima mossa di gioco 
            state.game_panel.grid.place_mines(state.mouse_cell); //piazza le mine in base alla prima cella cliccata
            state.game_panel.grid.place_numbers(); //imposta il tipo di ogni cella 
            state.reveal(state.game_panel.grid, state.mouse_cell); //rivela la cella cliccata
        }
        else state.reveal(state.game_panel.grid, state.mouse_cell); // in caso sia stata fatta una mossa successiva alla prima, rivela solo la cella cliccata
    }

    //se è stato cliccato il tasto destro del mouse 
    if(mouse.button == sf::Mouse::Button::Right){
        if(state.first_move) return; //se non è ancora stata fatta la prima mossa non fare nulla 

        //se la cella cliccata non è già in stato Flag, cambia la texture e lo stato della cella in quello della bandierina
        if(cur_cell.cell_state != cell_state::Flag){
            cur_cell.cell_state = cell_state::Flag; 
            cur_cell.cell_texture = &Flag_texture;
        }
        //se invece è già in stato Flag, cambia la sua texture in Covered e anche il suo stato (togli la bandierina)
        else{
            cur_cell.cell_state = cell_state::Covered; 
            cur_cell.cell_texture = &Covered_texture;
        }
    }
}

//AGGIUNTA: gestione dell'evento caratterizzato dal movimento del mouse 
void handle_mouse_moved (const sf::Event::MouseMoved& mouse, sf::RenderWindow& window, State& state){
    //la posizione del mouse viene salvata 
    const sf::Vector2f mouse_float_pos = window.mapPixelToCoords(sf::Vector2i(mouse.position.x, mouse.position.y));

    //ricerca e salvataggio dell'indice della cella in cui si trova il mouse nella griglia (se non si trova nella griglia la posizione sarà -1, un indice impossibile per la griglia)
    int new_idx =-1;
    for (int i = 0; i < state.game_panel.grid.cells.size(); ++i) {
        if (state.game_panel.grid.cells[i].cell_bounds.contains(mouse_float_pos)) {
            new_idx = i; 
            break;
        }
    }

    //se il muose è rimasto nella stessa cella nella pur spostandosi non fare nulla (ad esempio attraverso un micro movimento)
    if (new_idx == state.mouse_cell) return;

    //se il mouse si trovava sopra una cella toglici il focus 
    if(state.mouse_cell >= 0)
        state.game_panel.grid.cells[state.mouse_cell].cell_mouse_focus = false;

    //metti il mouse_focus alla cella in cui si trova ora il mouse 
    state.mouse_cell = new_idx;
    if (state.mouse_cell >= 0)
        state.game_panel.grid.cells[state.mouse_cell].cell_mouse_focus = true;
}

////////////////LOOP////////////////

int main()
{
    srand(time(NULL)); //AGGIUNTA: per il piazzamento casuale delle mine
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
        //MODIFICATO: aggiunta la gestione dei nuovi eventi
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