# TAPPA 6 - LIVELLI DI DIFFICOLTÀ

## OBBIETTIVO DELLA TAPPA: 
Implementare i restanti due livelli di difficoltà del gioco. <br>
Ora si potranno fare partite in difficoltà: 
- FACILE: 
    * dimensione griglia: 9x9 celle
    * numero mine: 15 
- MEDIO: 
    * dimensione griglia: 16x16 celle
    * numero mine: 40
- DIFFICILE: 
    * dimensione griglia: 30X20 celle
    * numero mine: 99 
Rendere possibile, inoltre, la scelta della difficoltà di gioco già dalla schermata iniziale.

## COSA È STATO IMPLEMENTATO: 
Rispetto alla tappa precedente: 
- sono state definite diverse nuove costanti globali: 
    * per definire una proprorzione fissa tra altezza dell'header e della griglia (header_grid_proportion)
    * per impostare dei valori di default per la dimensione e il gap delle celle della griglia
    * per avere una nuova dimensione di testo per lo Stop Panel (stop_title2_size)
    * per una nuova classe enum Difficulty che andrà a definire i tipi di difficoltà possibili di gioco 
- sono stati modificati i valori delle costanti globali:
    * 'button_type' per contenere i nuovi tre tipi di pulsanti possibili (cioè i pulsanti delle tre difficoltà)
    * 'stop_type' per contenere il nuovo tipo di Stop Panel per la schermata di scelta della difficoltà di gioco 
- sono stati modificati i calcoli della dimensione e posizione dell'header e i parametri dei suoi pulsanti per fare in modo che considerino sempre nei calcoli la cell_size di default (quindi quella della mdoalità EASY)
- è stato modificato il costruttore della struct Game_Panel in modo che richieda come argomento anche un valore di gap tra le celle in quanto detto valore non sarà più fisso ma varierà in base alla difficoltà di gioco 
- sono stati definiti tra nuovi oggetti Control_Button nella struct Stop_Panel in modo da poter rappresentare i tre pulsanti delle difficoltà nello Stop Panel di tipo New Game. Detti pulsanti verranno posizionati in fondo allo Stop Panel
- è stata definita una nuova variabile di tipo Difficulty nella Struct State che conterrà la difficoltà di dioco 
- è stata definita nella struct State una nuova funzione che va a settare la difficoltà di gioco andando a modificare il numero di celle e mine e il valore di gap della griglia e il valore della variabile diff nella struct State in quelli corretti.
- sono state modificate le funzioni draw():
    * di Stop_Panel in modod a gestire anche la rappresentazione dello Stop Panel di tipo new game per la scelta della difficoltà di gioco
    * di Control_Button per gestire la rappresentazione dei tre nuovi pulsanti 
    * di Start_Panel in modo che al posto di contenere il testo 'Premere ENTER per iniziare una nuova partita', contenga i tre pulsanti per la scelta della difficoltà di gioco 
- modificata la funzione pause() per fare in modo che tenga in considerazione anche che la aprtita è considerata in pausa anche quando lo Stop_Panel risulta di tipo New_Game
- sono state modificate le funzione per la gestione degli eventi: 
    * del click di un pulsante e movimento del mouse in modo che gestiscano anche il click e focus per i nuovi pulsanti creati
    * del click del pulsante ENTER sulla tastiera in modo che non permetta più di iniziare una nuova partita  

## ISTRUZIONI PER ESEGUIRE DA TERMINALE: 
Prima di poter scrivere questo comando su terminale, bisogna aver: 
- creato la cartella build 
- configurato tramite cmake 
- compilato con make 

```bash
cd build 
./Tappa6
```

## VERIFICA DEL RISULTATO: 
#### N.B: Le frecce nelle foto sono per indicare un particolare del risultato (tipo la cella considerata per quella verifica di risultato). Non devono comparire nel risultato
#### START PANEL
All'apertura della finestra, nello Start Panel, dovrebbero ora apparire il testo 'Scegliere la modalità di gioco' e tre nuovi pulsanti al posto del testo 'Premere ENTER per cominciare una nuova partita'. <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_seiuno](../risorse/risultati/tappa6_1.png)

Al passaggio del mouse sopra uno dei tre pulsanti delle difficoltà, il bordo di detto pulsante dovrebbe diventare da nero a rosso e poi tornare nero appena il mouse si sposta al di fuori dei confini del pulsante. <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_seidue](../risorse/risultati/tappa6_2.png)

Cliccando il pulsante FACILE la schermata dovrebbe cambiare in quella del gioco in modalità EASY (quindi griglia 9x9 e 15 mine). <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_seitre](../risorse/risultati/tappa6_3.png)

Cliccando il pulsante MEDIA la schermata dovrebbe cambiare in quella del gioco in modalità MEDIUM (quindi griglia 16x16 e 40 mine). <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_seiquattro](../risorse/risultati/tappa6_4.png)

Cliccando il pulsante DIFFICILE la schermata dovrebbe cambiare in quella del gioco in modalità HARD (quindi griglia 30x20 e 99 mine). <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_seicinque](../risorse/risultati/tappa6_5.png)

#### CONTROL PANEL: 
Cliccando il pulsante NUOVA PARTITA nel Control Panel dovrebbe diventare visibile lo Stop Panel di tipo New Game. 

#### STOP PANEL: 
- Cliccando il pulsante NUOVA PARTITA nello Stop Panel di tipo Pause, Win o Lose dovrebbe diventare visibile lo Stop Panel di tipo New Game. 

- Cliccando uno dei tre pulsanti delle difficoltà presenti nello Stop Panel di tipo New Game la aschermata di dovrebbe cambiare in quella di gioco della difficoltà selezionata. 

## PROBLEMI RISCONTRATI E SOLUZIONI: 
#### PROBLEMA 
Con l'introduzione delle difficoltà MEDIO e DIFFICILE, il numero di celle aumenta drasticamente e, per quanto riguarda la modalità DIFFICILE, va a modificare anche l'aspetto della griglia da quadrata a rettangolare. 
Per queste due nuove modalità la griglia e il panel (essendo che era stato impostato che avessere sempre le stesse dimensioni) andavano a coprirsi l'uno con l'altro.

#### SOLUZIONE 
È stata implementata la funzione set_difficulty in modo che in base alla difficoltà scelta in modo che la dimensione del Game Panel venga variata per essere totalmente contenuta nello spazio tra Control Panel e bordo finestra cambiando diversi parametri e chiamando la funzione reset. <br>
Essendo che la funzione reset non va a ricosturire ogni volta un nuovo Control Panel, il Control Panel rimarrà sempre delle stesse dimensioni e non adrà a coprire il Game Panel. 

#### PROBLEMA: 
Con il cambiamento delle dimensioni del Game Panel si andava a variare anche la dimensione dell'Header e dei suoi elementi rendendoli in alcuni casi illeggibili. 

#### SOLUZIONE: 
Si è optato per lasciare sempre uguale (cioè come sarebbero rappresentati nella modalità EASY di gioco) l'altezza dell'Header e le dimensioni dei suoi elementi.

## FONTI DI RIFERIMENTO UTILIZZATE: 
Nessuna (oltre a quelle utilizzate nelle tappe precedenti).