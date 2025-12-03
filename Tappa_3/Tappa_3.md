# TAPPA 3 - BORDO

## OBBIETTIVO DELLA TAPPA: 
Creare un bordo con texture specificata (borders[] in texture_fonts.hpp) che vada a circondare la griglia e l'header andando a completare il Game Panel. 

## COSA È STATO IMPLEMENTATO: 
Rispetto alla tappa precedente: 
- sono state definite le struct (con le loro funzioni draw() e i loro costruttori):
    * 'Borer_Cell': rappresenta le celle che costituiscono insieme il bordo 
    * 'Border': rappresenta il bordo stesso (l'unione delle Border Cells)

## ISTRUZIONI PER ESEGUIRE DA TERMINALE: 
All'apertura della finestra dovrebbe vendersi un bordo grigetto intorno alla griglia di gioco e all'header. <br>
Detto bordo non deve essere interagibile e non deve andare a coprire l'illuminazione rossa del bordo di una cella esterna nel caso in cui detta cella prenda il focus. <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_tre](../risorse/risultati/tappa3.png)

## VERIFICA DEL RISULTATO: 
#### N.B: Le frecce nelle foto sono per indicare un particolare del risultato (tipo la cella considerata per quella verifica di risultato). Non devono comparire nel risultato

## PROBLEMI RISCONTRATI E SOLUZIONI: 
#### PROBLEMA: 
L'ordine di disegno iniziale (prima grid poi border) in Game_Panel.draw() causava che il bordo grafico (Border) andasse a coprire il gap tra le celle e il bordo e quindi l'effetto di focus (mouse_focus) delle celle esterne.
#### SOLUZIONE: 
è stato invertito l'ordine di disegno in modo che venga prima disegnato il bordo, seguito dall'header e poi la griglia garantendo che gli effetti di focus e le interazioni sulle celle siano sempre visibili in primo piano.

#### PROBLEMA: 
Inizialmente il bordo doveva essere rappresentato come una griglia di celle adiacenti simile alla struttura Grid. Tuttavia questo lavoro risultava molto complesso e con la creazione di diversi array di oggetti cella. Ciò andava a complicare di molto il codice, soprattutto se si considera con il bordo rimarrà sempre non interagibile.
#### SOLUZIONE: 
Si è optato per la creazione di soli 8 elementi fondamentali: 
- 4 Celle Angolari (angle_cells): Quadrati fissi che corrispondono ai vertici del bordo.
- 4 Celle Laterali (side_cells): Rettangoli che si adattano dinamicamente per coprire l'intera lunghezza/altezza tra gli angoli.<br>

Questa soluzione ha reso il bordo adattabile a diverse risoluzioni senza ricalcoli continui.

## FONTI DI RIFERIMENTO UTILIZZATE: 
Nessuna (oltre a quelle utilizzate nelle tappe precedenti).