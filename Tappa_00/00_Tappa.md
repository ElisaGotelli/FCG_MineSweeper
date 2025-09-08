# TAPPA 00 - CREAZIONE BASE DELLA FINESTRA 

## OBBIETTIVO DELLA TAPPA:
Creazione della finestra SFML e gestione dei suoi eventi di base: 
- chiusura della finestra 
- ridimensionamento della finestra (che va a mantenere il rapporto 4:3)
- loop di interazione 

## ISTRUZIONI PER ESEGUIRE DA TERMINALE: 
#### Creazione della build: 
cmake -S . -B build <br> 
cmake --build build -j

#### Esecuzione del file.cpp: 
cd build <br> 
./00_hello.cpp 

## VERIFICA DEL RISULTATO: 
All'invio dell'ultimo comando per l'esecuzione dovrebbe aprirsi una finestra vuota delle dimensioni 1200x900 e con il titolo "Base Window". <br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_zero](../risorse/risultati/tappa00.png)

## PROBLEMI RISCONTRATI E SOLUZIONI: 
Nessuno (per motivazione spiegata nel punto sottostante).

## FONTI DI RIFERIMENTO UTILIZZATE: 
Per questa tappa è stato utilizzato il codice sviluppato per il laboratorio 3 sviluppato durante le lezioni insieme al professor Puppo. <br>
Sono solo stati aggiunti dei commenti e cambiata la dimensione della finestra e il titolo. 
