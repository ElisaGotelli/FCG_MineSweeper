# TAPPA 1C - VITTORIA, SCONFITTA E RESET PER UNA NUOVA PARTITA 

## OBBIETTIVO DELLA TAPPA: 
Rendere la versione base del gioco (quindi solo con la griglia) giocabile, cioè: 
- impostare le condizioni e l'animazione per vittoria (condizione: sono state scoperte tutte le celle nella griglia tranne quelle con nascosta una mina) e sconfitta (condizione: è stata scoperta una cella con nascosta una mina)
- impostare una schermata di fine partita e un modo per poterne cominciare una nuova 

## COSA È STATO IMPLEMENTATO: 
Rispetto alla tappa precedente:
- è stata definita la struct (con la sua funzione draw() e costruttore)'Stop_Panel' per il pannello di fine partita 
- è stato implementato il metodo di: 
    * vittoria tramite una variabile num_revealed che conta il numero di celle rivelate 
    * sconfitta tramite un controllo di che tipo di mina è stata rivelata e nel caso di una mina farà partire l'animazione di sconfitta
- sono state implementate due nuove funzioni con lo scopo di: 
    * gestire la fine di una partita con la rivelazione di tutte le celle mine (ending reveal), in cui nel caso di una sconfitta, la mina esplosa sarà l'unica a possedere la texture della mina esplosa. Inoltre a fine partita la cella non sarà più cliccabili e non sarà possibile avere focus sulle celle 
    * gestire il reset della finestra al suo stato iniziale in modo da poter cominciare una nuova partita (reset) 
- sono state definite delle nuove costanti globali contenenti: 
    * le caratteristiche del nuovo pannello creato (larghezza, altezza, posizione,..)
    * le caratteristiche di tutto il testo nel gioco 
    * le caratteristiche di colore e bordo del nuovo pannello creato che serviranno anche per i pannelli che verranno creati nelle tappe successive
- è stata implementata una nuova funzione per la gestione del click del pulsante ENTER sulla tastiera per cominciare una nuova partita

## ISTRUZIONI PER ESEGUIRE DA TERMINALE: 
Prima di poter scrivere questo comando su terminale, bisogna aver: 
- creato la cartella build 
- configurato tramite cmake 
- compilato con make 

```bash
cd build 
./Tappa1c
```

## VERIFICA DEL RISULTATO:
#### SCONFITTA: 
Alla rivelazione di una mina deve comparire la schermata di fine partita con tipo sconfitta (quindi con scritto 'Hai Perso!'). <br><br>
Allego uno screenshot del risultato da me ottenuto:
![risultato_1c_uno](../risorse/risultati/tappa1c_1.png)

#### VITTORIA: 
Alla rivelazione di tutte le celle a parte quelle che nascondono una mina deve apparire la schermata di fine partita con tipo vittoria (quindi con scritto 'Hai Vinto!'). <br><br>
Allego uno screenshot del risultato da me ottenuto:
![risultato_1c_due](../risorse/risultati/tappa1c_2.png)

## PROBLEMI RISCONTRATI E SOLUZIONI: 
Nessuno.

## FONTI DI RIFERIMENTO UTILIZZATE: 
Nessuna (oltre a quelle utilizzate nelle tappe precedenti).

