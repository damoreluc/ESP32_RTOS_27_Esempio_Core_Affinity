# FreeRTOS Esercizio 27: Esempio illustrativo della affinità dei task ai core

Si considerano due task: __Task H__ ad alta priorità e __Task L__ a bassa priorità.

Task L e Task H stampano un messaggio diagnostico per indicare su quale core sono in esecuzione e poi saturano quel core per un tempo prestabilito (`time_hog`) con un ciclo di numerose istruzioni NOP.

I due task possono essere eseguiti indifferentemente sul __core 0__ e sul __core 1__ della ESP32.

Nella `setup()` provare a osservare come cambia il comportamento del programma modificando il pinning del task nell'istruzione `xTaskCreatePinnedToCore()`.

Le possibili opzioni sono: `PRO_CPU_NUM`, `APP_CPU_NUM` e `tskNO_AFFINITY`