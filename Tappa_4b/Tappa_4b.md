# TAPPA 4B - PANNELLO DI CONTROLLO INTERATTIVO

## OBBIETTIVO DELLA TAPPA: 
- Rendere interattivo il Control Panel in modo che consenta di: 
    * cominciare una nuova partita quando viene cliccato il pulsante 'Nuova Partita' 
    * mettere in pausa il gioco quando viene cliccato il pulsante 'Pausa' 
    * sia possibile avere lo stesso effetto di focus per i vari pulsanti del Control Panel come per le celle
- Modificare il pannello di stop in modo che possa essere anche utilizzato come pannello per la Pausa di una partita e che contenga un pulsante di nuova partita al posto del testo 'premere ENTER per cominciare una nuova partita'

## COSA È STATO IMPLEMENTATO: 
Rispetto alla tappa precedente: 
- sono state definite delle nuove costanti globali per le caratteristiche di dimensione dei pulsanti e nuovi testi che saranno d'ora in poi presenti nello Stop Panel 
- è stata modificata l'enum class stop_type che rappresenta i possibili tipi di Stop Panel con l'aggiunta del tipo Pause. Questo tipo servirà per la rappresentazione de pannello di pausa
- sono state definite due nuove variabili: 
    * 'mouse_focus' nella struct Control_Button per poter sui nuovi pulsanti creati avere lo stesso meccanismo di focus (illuminazione rossa del bordo) che viene utilizzato per le celle al passaggio del mouse 
    * 'game_paused' nella struct State che va ad indicare se il gioco è o meno in pausa 
- è stato definito un nuovo oggetto Control_Button di tipo new_game nello Stop Panel che quando cliccato permetterà di cominciare automaticamente una nuova partita 
- è stata modificata la funzione draw() dello Stop Panel con al definizione della rappresentazione grafica dello Stop Panel di tipo Pause
- sono state implmentate due nuove funzioni all'interno della struct State: 
    * 'pause': implementa il meccanismo per mettere in pausa il gioco 
    * 'restart': implmenta il meccanismo per far ripartire il gioco se era stato messo in pausa 
- sono state modificate le funzione per la gestione degli eventi: 
    * di perdita di focus con l'aggiunta che con quell'evento il gioco deve essere messo automaticamente in pausa chiamando la funzione opportuna pause()
    * di click del pulsante sinistro del mouse in modo che nel caso il mouse sia sopra ad un pulsante vengano fatto tutte le operazione per la gestione del click su detto pulsante
    * del movimento del mouse in modo che se il mouse si trova sopra ad un pulsante esso abbia il focus (bordo rosso)
    * di click di ENTER su tastiera in modo che nel caso il gioco sia in pausa permetta di ricominciare la partita da dove ci si era fermati
    

## ISTRUZIONI PER ESEGUIRE DA TERMINALE: 
Prima di poter scrivere questo comando su terminale, bisogna aver: 
- creato la cartella build 
- configurato tramite cmake 
- compilato con make 

```bash
cd build 
./Tappa4b
```

## VERIFICA DEL RISULTATO: 
#### N.B: Le frecce nelle foto sono per indicare un particolare del risultato (tipo la cella considerata per quella verifica di risultato). Non devono comparire nel risultato
#### FOCUS: 
Al passaggio del mouse sopra un pulsante, il bordo di detto pulsante dovrebbe diventare da nero a rosso e poi tornare nero appena il mouse si sposta al di fuori dei confini del pulsante. <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_quattrobuno](../risorse/risultati/tappa4b_1.png)

#### CONTROL PANEL: 
Al cliccare con il pulsante sinistro del mouse il pulsante PAUSA nel Control Panel dovrebbe comparire lo Stop Panel di tipo Pause e il gioco dovrebbe stopparsi. <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_quattrobdue](../risorse/risultati/tappa4b_2.png)

Al cliccare con il pulsante sinistro del mouse il pulsante NUOVA PARTITA nel Control Panel la schermata di gioco dovrebbe tornare a quella di default. <br><br>


#### STOP PANEL: 
Negli Stop Panel di tipo Win e Lose non dovrebbe più essere presente la scritta 'premere ENTER per cominciare una nuova PARTITA' ma vi dovrebbe essere il pulsante di NUOVA PARTITA. <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_quattrobtre](../risorse/risultati/tappa4b_3.png)  
![risultato_quattrobquattro](../risorse/risultati/tappa4b_4.png) 

Al cliccare il pulsante ENTER nello Stop Panel di tipo Pause la partita dovrebbe ripartire dall'esatto momento in cui era stata fermata (stesso valore del timer). <br><br>
Allego uno screenshot del risultato da me ottenuto:  

Al cliccare con il pulsante sinistro del mouse il pulsante NUOVA PARTITA nello Stop Panel la schermata di gioco dovrebbe tornare a quella di default (quindi con lo Stop Panel non visibile) <br><br>

## PROBLEMI RISCONTRATI E SOLUZIONI: 
Nessuno.

## FONTI DI RIFERIMENTO UTILIZZATE: 
Nessuna (oltre a quelle utilizzate nelle tappe precedenti).