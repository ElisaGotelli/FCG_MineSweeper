# BORDO

## OBBIETTIVO DELLA TAPPA: 

## COSA È STATO IMPLEMENTATO: 

## ISTRUZIONI PER ESEGUIRE DA TERMINALE: 

## VERIFICA DEL RISULTATO: 
#### N.B: Le frecce nelle foto sono per indicare un particolare del risultato (tipo la cella considerata per quella verifica di risultato). Non devono comparire nel risultato

## PROBLEMI RISCONTRATI E SOLUZIONI: 
#### PROBLEMA: 
Il bordo andava a coprire l'illuminarsi delle celle 
#### SOLUZIONE: 
disegnare prima il bordo e poi la griglia 

#### PROBLEMA: 
Inizialmente volevo suddividere il bordo in celle come la griglia ma l'header non è stato creato come moltiplicazione di celle quindi il lavoro si andava a complicare e si andavano a creare tantissimi array 
#### SOLUZIONE: 
visto che il bordo rimarrà sempre uguale e nello stesso posto per tutta la partita e non ineragibile, sono state create solo 8 celle: 
- 4 angoli (nel vettore apposito)
- 4 lati (di lunghezza/altezza che andasse a coprire tutto il bordo a parte gli angoli)

## FONTI DI RIFERIMENTO UTILIZZATE: 