# CELLE INTERATTIVE 

## OBBIETTIVO DELLA TAPPA: 
Rendere le celle inerattive, in particolare: 
- Fare in modo che al solo passaggio del mouse su una cella il bordo di detta cella diventi rosso 
- Il primo click sinistro sulla griglia faccia partire la rivelazione a catena di parte delle celle della griglia 
- dopo il primo click: 
    * ogni click sinistro su una cella coperta faccia rivelare detta cella 
    * ogni click destro su una cella coperta la renda una cella coperta con bandierina sopra 

## COSA È STATO IMPLEMENTATO: 
Dalla tappa precedente sono state aggiunte come nuove implementazioni: 
- le texture per le varie celle scoperte (mine, numeri e vuota) e coperte (bandierina)
- diverse funzioni per la gestione degl eventi: 
    * è stata gestita la possibilità di eventi non programmati tramite una funzione di gestione generica con corpo vuoto 
    * è stato gestito il passaggio del muose su una cella in modo che il bordo diventi rosso 
    * è stato gestito l'evento di perdita e presa di focus della finestra di gioco 
    * è stato gestito l'evento di click sinistro su una cella che: 
        * nel caso la cella sia di tipo Covered o Flag riveli la cella tramite la funzione 'reveal' e: 
            * nel caso la cella sia di tipo Mine per ora rivelo solo la mina esplosa e basta 
            * nel caso la cella sia di tipo Number riveli il numero di mine adiacenti alla cella considerata 
            * nel caso la cella sia di tipo Empty faccia partire una flood_reveal 
        * nel caso la cella sia di tipo Revealed non faccia nulla 
    * è stato gestito l'evento di click destro su una cella che: 
        * nel caso non sia stata fatta la prima mossa oppure la cella sia di tipo Revealed non faccia nulla 
        * negli altri casi metta una bandierina sulla cella (texture Flag e stato Flag) e incrementi il contatore per il numero di bandierine in gioco 

- CASO PARTICOLARE PRIMA MOSSA: 
    * è stato fatto in modo che la prima cella cliccata e le 8 celle a lei adiacenti non siano mine
    * solo dopo che è stata cliccata la prima cella viene fatto partire il meccanismo di posizionamento delle mine nella griglia in modo casuale 
    * dopo il posizionamento delle mine, viene messo il tipo della cella in base al fatto che sia una mina o il numero di mine a lei adiecenti
    * a causa del fatto che la prima cella e le sue adiacenti sono senza mine (quindi la prima cella non ha nessuna bomba adiacente ed è quindi di tipo Empty) essa farà sempre partire una rivelaziona cascata 

## ISTRUZIONI PER ESEGUIRE DA TERMINALE: 
cmake -S . -B build <br> 
cmake --build build -j

#### Esecuzione: 
cd build <br> 
./Tappa1b

## VERIFICA DEL RISULTATO: 
- Al passaggio del mouse su una cella essa dovrebbe aver il bordo illuminato di rosso: 

- Al primo click sinistro su un cella coperta dovrebbe partire sempre una rivelazione a catena: 

- Al click sinistro dopo il primo su una cella sinistra dovrebbe rivelare la cella (mina, vuota, numero). Il numero rivelato da un cella dovrebbe corrispondere al numero di mine a lei adiacenti:  

- Al click destro su una cella coperta dovrebbe apparire una bandierina su detta cella: 

## PROBLEMI RISCONTRATI E SOLUZIONI:
#### PROBLEMA:  
Ho voluto aggiungere la caratteristica che la cella su cui passa il mouse venisse illuminata con il bordo rosso. Il bordo però veniva in parte coperto dalle altre celle. 
#### SOLUZIONE: 
Ho aggiunto un leggerO gap della stessa grandezza del bordo (2.f) tra le celle. 

#### PROBLEMA: 
Nella versione precedente del codice si salvava solamente per ogni la posizione della cella nella finestra ma non il suo indice riga/colonna nella griglia e senza saperlo non si poteva fare il flood reveal o accertare il fatto che le celle adiacenti alla prima della cliccata non fossero mai di tipo Mine. 
#### SOLUZIONE: 
Sono stati aggiunti questi due parametri in modo da non doverli ricalcolare ogni volta 

## FONTI DI RIFERIMENTO UTILIZZATE: 
Nessuna 

