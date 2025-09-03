# Regole/Caratteristiche definitive (Minesweeper “E.0”)

* la schermata iniziale con scritto: **Benvenuto su MineSweeper (versione E.0)** e la possibilità di scegliere tra i tre livelli: facile, medio, difficile
* scelta la difficoltà verrà creato:
* il campo in base alla difficoltà: **facile (9×9)**, **medio (16×16)**, **difficile (30×16)**
* nascoste le mine nel campo (**10 per facile, 40 per medio, 99 per difficile**)
* l'orologio per cronometrare la partita che partirà appena il giocatore cliccherà una cella a caso
* la faccina solita del gioco (normale, triste alla sconfitta, festosa alla vittoria)
* il numero di bandierine ancora da piazzare
* un pulsante di pausa del gioco
* un pulsante che ti permette di creare una nuova partita (cliccato questo pulsante si deve ridare la possibilità di scelta del livello) e quindi riniziare

quando il giocatore cliccherà la prima cella:

* detta cella **non deve mai contenere una mina** e **deve sempre essere uno zero**
* automaticamente deve scoprire tutte le celle senza numero intorno a lei fino ad avere un bordo di celle con numeri (**effetto flood-fill**)
* deve partire il cronometro

il giocatore potrà premere:

* il pulsante destro per mettere una bandierina (ciclo: coperta → bandiera → punto interrogativo → coperta)

* il pulsante sinistro per scoprire una cella

* se il giocatore scopre una cella vuota (0) dovrà rifare l'effetto a catena (flood-fill) come al primo click

* se il giocatore scopre una bomba dovrà esserci l'animazione di sconfitta come nel gioco originale

* quando il giocatore scopre tutte le celle a parte quelle con le bombe ci dovrà essere l'animazione come nel gioco originale di vittoria

* a sinistra della cella di gioco ci dovrà essere una piccola legenda per indicare i comandi di gioco

* alla vittoria deve apparire oltre all'animazione originale: il tempo fatto, la scritta **“hai vinto!”**, la possibilità di iniziare una nuova partita con il livello che si preferisce, la faccina che festeggia

inoltre sappi che ho in una cartella tutte le texture di minesweeper originale

---

### Aggiunte rispetto al classico:

* **Faccina “pressed” temporanea**: quando tieni premuto il mouse, la faccina fa un’espressione sorpresa/“😮” fino al rilascio.
* **Primo click sempre safe e sempre zero**: la cella del primo click non solo non può contenere mine, ma deve essere un **0** che attiva subito l’apertura a catena.
* **Menu di scelta difficoltà anche in partita**: oltre al pulsante nuova partita, si può cambiare difficoltà direttamente dall’HUD senza tornare alla schermata iniziale.
* **Doppio click / chord click**: se clicchi con il sinistro su un numero già rivelato e attorno a lui ci sono bandierine pari al numero indicato, il gioco rivela automaticamente tutte le celle adiacenti non flaggate (con eventuale flood sugli zeri).

