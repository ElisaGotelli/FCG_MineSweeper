# TAPPA 7A - GRAFCA PULSANTE DI EXIT 

## OBBIETTIVO DELLA TAPPA: 
Creare la grafice per un nuovo pulsante EXIT per il ritorno dalla schermata di gioco alla schermata iniziale. <br>
Detto pulsante dovrà trovarsi: 
- nel pannello di controllo 
- nel pannello di pausa 
- nel pannello di vittoria/sconfitta 

## COSA È STATO IMPLEMENTATO: 
Rispetto alla tappa precedente: 
- è stata definita una nuova costante globale 'control_vertical_gap' che contiene il valore di gap verticale tra i due pulsanti PAUSA e NUOVA PARTITA e il nuovo pulsante 
- è stata modificata la classe enum 'button_type' per contenere il nuovo tipo di pulsante di uscita 
- è stato definito un nuovo oggetto Control_Button per le struct Control_Panel e Stop_Panel per la creazione del nuovo pulsante ESCI in detti pannelli
- è stata modificata la posizione del pulsante NUOVA PARTITA nella struct Stop_Panel per fare spazio al nuovo pulsante appena creato
- sono state modificate le funzioni draw() delle struct Control_Panel, Stop_Panel e Control_Button per la gestione della rappresentazione del nuovo puslsante ESCI

## ISTRUZIONI PER ESEGUIRE DA TERMINALE: 
Prima di poter scrivere questo comando su terminale, bisogna aver: 
- creato la cartella build 
- configurato tramite cmake 
- compilato con make 

```bash
cd build 
./Tappa7a
```

## VERIFICA DEL RISULTATO: 
#### N.B: Le frecce nelle foto sono per indicare un particolare del risultato (tipo la cella considerata per quella verifica di risultato). Non devono comparire nel risultato
#### CONTROL PANEL: 
Nel Control Panel dovrebbe essere presente appena sotto i pulsanti PAUSA e NUOVA PARTITA e centrale orizzontalmente il nuovo pulsante ESCI. <br><br>
Allego uno screenshot del risultato da me ottenuto:  
![risultato_setteauno](../risorse/risultati/tappa7a_1.png)

#### STOP PANEL: 
Negli Stop Panel di tipo Pause, Win e Lose ora dovrebbe essere presente anche il pulsante ESCI a fianco al pulsante NUOVA PARTITA.  <br><br>
Allego degli screenshot dei risultati da me ottenuti:  
![risultato_setteadue](../risorse/risultati/tappa7a_2.png)
![risultato_setteatre](../risorse/risultati/tappa7a_3.png)
![risultato_setteaquattro](../risorse/risultati/tappa7a_4.png)

#### CONTROL PANEL: 

## PROBLEMI RISCONTRATI E SOLUZIONI: 
Nessuno.

## FONTI DI RIFERIMENTO UTILIZZATE: 
Nessuna (oltre a quelle utilizzate nelle tappe precedenti).
