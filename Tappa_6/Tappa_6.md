# TAPPA 6 - LIVELLI DI DIFFICOLTÀ

## OBBIETTIVO DELLA TAPPA: 
In questa parte ho voluto lasciare visibili i cambiamenti drastici fatti dalla aprte precedente . Questo per mostrare quanto è cambiato il programma con l'aggiungimento dei livelli. Solitamente invece cambiavo tutto anche nella altre parti ma almeno così mostravo appieno le difficoltà riscontrate 

## COSA È STATO IMPLEMENTATO: 

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

## PROBLEMI RISCONTRATI E SOLUZIONI: 
#### PROBLEMA 
fare si che le due nuove dimensioni della cella abbiano un buon layout 
#### SOLUZIONE 
creare una funzione che gestisce il cambio di difficoltà. 
impostare che l'altezza dell'header e i vari suoi dettagli non cambino dimensione della difficoltà facile 
lasciare sempre uguale il control_panel poichè aveva un buona dimensione 
mettere che la grandezza della griglia sia sempre la metà della grandezza della finestra meno 2 volte il panel_horizontal_displacement (in realtà leggermente + piccola)
prendere sempre in considerazione solo la larghezza poichè nella difficoltà difficile non si usa + un quadrato ma un rettangolo più lungo che alto

## FONTI DI RIFERIMENTO UTILIZZATE: 
Nessuna (oltre a quelle utilizzate nelle tappe precedenti).