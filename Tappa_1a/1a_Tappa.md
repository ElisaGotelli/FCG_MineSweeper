# TAPPA 1A - SFONDO E CELLA 9X9

## OBIETTIVO DELLA TAPPA: 
Sviluppo della grafica: 
- dello sfondo della finestra
- di un bordo per la finestra 
-  della griglia di gioco iniziale 9x9 (che in tappe successive corrisponderà alla modalità FACILE)

## COSA È STATO IMPLEMENTATO: 
Rispetto alla tappa precedente:
- è stato impostato un nuovo colore di sfondo per la finestra (verde chiaro)
- è stato aggiunto un bordo alla finestra tramite la creazione di una RectangleShape  di colore trasparente, bordo verde scuro e dimensione leggermente più piccola della finestra.
- sono state definite le struct:
    * 'Cell': rappresenta una singola cella nel gioco
    * 'Grid': rappresenta l'insieme delle celle come vettore e va a gestire il loro posizionamento nella finestra
    * 'Game_Panel': rappresenta un contenitore di tutti gli oggetti che comporanno in futuro il pannello di gioco. er ora conterrà solo la griglia con le varie celle.
    * 'State': rappresenta lo stato complessivo della finestra

Tutti gli elementi elencati verrano messi su display tramite una serie di chiamate alle funzioni draw() e display(). 


## ISTRUZIONI PER ESEGUIRE DA TERMINALE: 
Prima di poter scrivere questo comando su terminale, bisogna aver: 
- creato la cartella build 
- configurato tramite cmake 
- compilato con make 

```bash
cd build 
./Tappa1a
```

## VERIFICA DEL RISULTATO: 
All'invio dell'ultimo comando per l'esecuzione dovrebbe aprirsi una finestra intitolata "Cell starting texture" con: 
- sfondo verde chiaro 
- bordo verde scuro 
- una griglia 9x9 di celle grigie, con texture 'Covered', posizionata nella parte destra/centrale della finestra.
<br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_unoa](../risorse/risultati/tappa1a.png)

## PROBLEMI RISCONTRATI E SOLUZIONI: 
#### PROBLEMA: 
Creare il bordo per la finestra.
#### SOLUZIONE: 
In SFML non esiste un comando che va ad impostare un bordo interno alla finestra stessa. 
Per poterlo creare allora, inizialmente si è deciso di creare un rettangolo delle stesse dimensioni e posizione della finestra di colore trasparente ma con un bordo colorato spesso 20. 
Ciò però ha portato ad un nuovo problema: il bordo veniva disegnato al di fuori dell'area visibile della finestra(e quindi tagliato). <br>  
Per risolvere il nuovo problema si è: 
- ridotta la dimensione del rettangolo del doppio dello spessore del bordo creato sia in altezza che larghezza rispetto alla finestra. 
- spostata l'origine del rettangolo di {20,20} (pari allo spessore del bordo). 

<br> In questo modo, il bordo viene disegenato perfettamente all'interno dell'area visibile.

## FONTI DI RIFERIMENTO UTILIZZATE:  
Nessuna (oltre a quelle utilizzate nelle tappe precedenti).



