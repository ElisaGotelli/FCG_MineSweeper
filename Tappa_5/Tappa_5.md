# TAPPA 5 - SCHERMATA INIZIALE

## OBBIETTIVO DELLA TAPPA: 
Creare e rendere interattiva una schermata iniziale contenente: 
- un benvenuto al gioco 
- la scritta di premere ENTER per cominciare una nuova partita

## COSA È STATO IMPLEMENTATO: 
Rispetto alla tappa precedente: 
- è stata definita una nuova struct (con la sua funzione draw() e costruttore) 'Start_Panel' che rappresenta la schermata iniziale di gioco. 
- è stato definito l'oggetto Start_Panel all'interno della struct State
- sono state definite delle nuove costanti globali per le caratteristiche di dimensione, posizione, gap e testo del nuovo Start Panel 
- è stata modificata la funzione draw() della struct State in modo che, all'aperutura della finestra, venga reso solo visibile lo Start Panel fino al click del puslante ENTER su tastiera 
- sono state modificate le funzione per la gestione degli eventi: 
    * di perdita di focus in modo che la schermata di pausa (Stop Panel di tipo pause) non diventi visibile se lo Start Panel risulta ancora visibile 
    * di click di un pulsante sulla tastiera in modo che al click del pulsante ENTER se la schermata iniziale è ancora visibile gli venga tolta la visibilità
    * di click e movimento del mouse in modo che non venga eseguita nessuna azione quando la schermata iniziale è visibile 

## ISTRUZIONI PER ESEGUIRE DA TERMINALE: 
Prima di poter scrivere questo comando su terminale, bisogna aver: 
- creato la cartella build 
- configurato tramite cmake 
- compilato con make 

```bash
cd build 
./Tappa5
```

## VERIFICA DEL RISULTATO: 
All'apertura della finestra non dovrebbe più apparire la schermata di gioco ma la schermata iniziale (Start_Panel). <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_cinque](../risorse/risultati/tappa5.png)

Al click del pulsante ENTER su tastiera quando la schermata iniziale è ancora visibile, la schermata iniziale dovrebbe essere sostituita da quella di gioco default. 

## PROBLEMI RISCONTRATI E SOLUZIONI: 
Nessuno.

## FONTI DI RIFERIMENTO UTILIZZATE: 
Nessuna (oltre a quelle utilizzate nelle tappe precedenti).