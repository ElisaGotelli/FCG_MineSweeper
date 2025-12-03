# TAPPA 2B - HEADER INTERATTIVO 

## OBBIETTIVO DELLA TAPPA: 
Rendere l'header interattivo: 
- al piazzamento di una bandierina nella griglia, decrementa il contatore delle bandierine e mostra il valore a schermo 
- la faccina: 
    * deve cambiare texture (texture: Click_face_texture) quando si va a cliccare e si tiene premuto il tasto sinistro del mouse su una cella coperta e deve tornare con la texture di default (smile_face_texture) quando si rilascia il tasto (quindi quando la cella coperta viene rivelata)
    * deve cambiare texture (texture: lost_face_texture) quando la partita finisce con una sconfitta 
    * deve texture (texture: win_face_texture) quando la partita finirà con una vittoria 
- il timer deve contenere i secondi di gioco aggiornati in ogni momento e il tempo totale della partita deve essere mostrato nel pannello di fine partita

## COSA È STATO IMPLEMENTATO: 
Rispetto alla tappa precedente: 
- è stato implementato un Clock interno a SFML per il calcolo reale del tempo per il timer 
- sono state implementate due nuove funzioni: 
    * 'update': per l'aggiornamento dei valori del timer 
    * 'set_number': per l'aggiornamento del contatore delle bandierine 
- è stata aggiunta l'indicazione del tempo totale (in ore, minuti e secondi) della partita al pannello di fine partita 
- sono stati implementati diversi controlli per il blocco e riavvio del timer nel caso in cui la finestra di gioco perdesse e poi riprendesse il focus 
- sono stati implementati diversi controlli per il cambio della texture della faccina nel caso di fine partita o di rivelazione di una cella
- è stata implementata una nuova funzione per la gestione dell'evento di rilascio del pulsante del mouse in modo da far tornare la faccina alla texture di default 

## ISTRUZIONI PER ESEGUIRE DA TERMINALE: 
Prima di poter scrivere questo comando su terminale, bisogna aver: 
- creato la cartella build 
- configurato tramite cmake 
- compilato con make 

```bash
cd build 
./Tappa2b
```

## VERIFICA DEL RISULTATO: 
#### N.B: Le frecce nelle foto sono per indicare un particolare del risultato (tipo la cella considerata per quella verifica di risultato). Non devono comparire nel risultato
#### INIZIO PARTITA: 
Appena aperta (o appena prima dell'inizio di una nuova partita) la finestra l'header dovrebbe presentare: 
- tre zero per il timer 
- la faccina con la texture di default
- la rappresentazione con Number del numeri 15 per il contatore (cioè il numero di mine nella griglia)
Non dovrebbe essere possibile incrementare o decrementare il contatore bandierine prima della prima mossa.  <br> 
Non dovrebbe essere possibile far partire il timer con il tasto destro del mouse. <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_dueb_uno](../risorse/risultati/tappa2b_1.png)

#### TIMER:
- Appena viene fatta la prima il timer dovrebbe iniziare ad incrementarsi. <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_dueb_due](../risorse/risultati/tappa2b_2.png)

- In caso di perdita del focus della finestra gioco il timer dovrebbe stopparsi e ripartire solo nel momento in cui la finestra riprende il focus. <br><br>

#### FLAG COUNTER:
- Al piazzamento di una bandierina il contatore delle bandierine dovrebbe diminuire di uno, a parte quando arriva a zero (il contatore non ammette numeri negativi). <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_dueb_tre](../risorse/risultati/tappa2b_3.png)
- Al togliere una bandierina il contatore delle bandierine dovrebbe aumentare di uno a parte se, dopo aver tolto detta bandierina, nella griglia vi sono ancora più bandierine che mine (ad esempio se nella griglia vi erano 17 bandierina e se ne a togliere 1, facendone rimanere 16, esse risultano più del numero di mine (15) nella griglia e quindi il contatore rimarrà a zero). <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_dueb_quattro](../risorse/risultati/tappa2b_4.png)

#### FACE:  
- Alla rivelazione di una cella coperta la faccina dovrebbe cambaire texture in quella sorpresa, per poi tornare nella etxture di default appena si smette di premere 
- Alla sconfitta in una partita la faccina dovrebbe cambaire texture in quello della sconfitta 
- Alla vittoria in una partita la faccina dovrebbe cambaire texture in quello della vittoria 

## PROBLEMI RISCONTRATI E SOLUZIONI: 
#### PROBLEMA: 
Inizialmente si erano invertiti le istruzioni nella funzione set_number didecremento/incremento di num_flag e controllo che num_flag fosse maggiore o uguale a zero. Ciò era stato fatto poichè nel gioco originale il contatore di bandierine poteva andare anche in negativo. 
Questo però portava ad un problema di out_of_bound per l'array delle etxture in quanto si andava a cercare un array negativo. 

#### SOLUZIONE: 
Essendo che le etxture utilizzate non permettono di scrivere valori negativi si è deciso di rinunciare alla rappresentazione dei valori negativi andando ad invertire l'ordine delle due istruzioni indicate precedentemente.

## FONTI DI RIFERIMENTO UTILIZZATE: 
Nessuna (oltre a quelle utilizzate nelle tappe precedenti).