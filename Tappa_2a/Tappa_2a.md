# GRAFICA HEADER 

## OBBIETTIVO DELLA TAPPA: 
Creare la grafica per un header del pannello di gioco e per i suoi parametri, cioè: 
- un timer per la partita in corso 
- la faccina tipica del gioco stesso Minesweeper 
- un contatore per contare il numero di bandierine che si possono ancora mettere nella griglia rispetto al numero di mine nella griglia stessa 

## COSA È STATO IMPLEMENTATO: 
Rispetto alla tappa precedente:
- sono state definite le struct (con le loro funzioni draw() e i loro costruttori):
    * 'Number': rappresenta un singolo numero tramite una RectangleShape con texture 
    * 'Timer': rappresenta il timer di gioco tramite tre Number 
    * 'Face': rappresenta la faccina tramite un rettangolo con texture 
    * 'Flag_Counter': rappresenta il contatore delle bandierine tramire tre Number
    * 'Header': rappresenta la struttura che contiene il Timer, Flag_Counter e Face
- sono state definite delle nuove costanti globali contenenti le caratteristiche di colore e del bordo dell'header 

## ISTRUZIONI PER ESEGUIRE DA TERMINALE: 
Prima di poter scrivere questo comando su terminale, bisogna aver: 
- creato la cartella build 
- configurato tramite cmake 
- compilato con make 

```bash
cd build 
./Tappa2a
```

## VERIFICA DEL RISULTATO: 
Esattamente al di sopra della griglia di gioco dovrebbe comparire un rettangolo grigio con bordo nero contenente: 
- a sinistra tre numeri 0 attaccati (che corrispondono al timer)
- al centro una faccina sorridente 
- a destra due numeri 0 e un 9 (che rappresentano il flag counter)
<br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_duea](../risorse/risultati/tappa2a.png)

## PROBLEMI RISCONTRATI E SOLUZIONI: 
Nessuno.

## FONTI DI RIFERIMENTO UTILIZZATE: 
Nessuna (oltre a quelle utilizzate nelle tappe precedenti).