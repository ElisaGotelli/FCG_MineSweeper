# TAPPA 4A - GRAFICA PANNELLO DI CONTROLLO 

## OBBIETTIVO DELLA TAPPA: 
Creare la grafica di un pannello di controllo posizionato alla sinistra della griglia di gioco e che conterrà per ora solo due pulsanti: 
- un pulsante per mettere in pausa la partita
- un pulsante per iniziare una nuova partita 

## COSA È STATO IMPLEMENTATO: 
Rispetto alla tappa precedente: 
- sono state definite le struct (con le loro funzioni draw() e i loro costruttori):
    * 'Control_Button': rappresenta i bottoni all'interno dei vari pannelli in gioco 
    * 'Control_Panel': rappresenta il pannello di gioco che sarà di grandezza uguale al Game Panel
- sono state definite delle nuove costanti globali per le caratteristiche di dimensione, tipi, bordo, posizionamento nel pannello, colore e testo dei Control Button
- sono state modificate le costanti 'window_horizontal_displacement' e 'window_vertical_displacement' per avere una migliore visibilità del Game Panel e Control Panel 
- è stato modificato un Game_Panel il calcolo della cell_size in modo che prenda in considerazione la larghezza della finestra e non l'altezza 

## ISTRUZIONI PER ESEGUIRE DA TERMINALE: 
Prima di poter scrivere questo comando su terminale, bisogna aver: 
- creato la cartella build 
- configurato tramite cmake 
- compilato con make 

```bash
cd build 
./Tappa4a
```

## VERIFICA DEL RISULTATO: 
#### N.B: Le frecce nelle foto sono per indicare un particolare del risultato (tipo la cella considerata per quella verifica di risultato). Non devono comparire nel risultato
All'apertura della finestra, a sinistra del Game Panel dovrebbe essere visibile il Control Panel con dimensioni uguali a quella del Game Panel e con all'interno i due pulsanti 'Pausa' e 'nuova Partita' in alto. <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_quattroa](../risorse/risultati/tappa4a.png)

## PROBLEMI RISCONTRATI E SOLUZIONI: 
#### PROBLEMA: 
Inizialmente si era lasciato invariato il calcolo della cell_size in Game Panel e quindi quest'ultima veniva calcolata in base all'altezza della finestra. Con l'aggiunta del Control Panel però i due essendo che erano delle stesse dimensioni si andavano a coprire a vicenda in quanto la zona in altezza disponibile per il Game Panel nella finestra diventava maggiore rispetto alla zona in larghezza.
Inizialmente, il calcolo della dimensione della cella (cell_size) all'interno di Game_Panel si basava esclusivamente sull'altezza della finestra. Tuttavia, con l'introduzione del Control_Panel affiancato orizzontalmente al pannello di gioco, lo spazio disponibile in larghezza per il Game Panel si è ridotto. Poiché l'altezza permetteva celle più grandi di quanto la larghezza potesse contenere, i due pannelli venivano renderizzati con dimensioni eccessive, sovrapponendosi e/o uscendo dai bordi della finestra. 

#### SOLUZIONE: 
Il calcolo della cell_size all'interno del Game_Panel è stato modificato, vincolando la dimensione delle celle alla larghezza disponibile della finestra anziché alla sua altezza.

## FONTI DI RIFERIMENTO UTILIZZATE: 
Nessuna (oltre a quelle utilizzate nelle tappe precedenti).