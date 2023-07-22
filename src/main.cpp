/*
 * FreeRTOS Esercizio 27: Esempio illustrativo della affinità dei task ai core
 *
 * Si considerano due task: Task H ad alta priorità e Task L a bassa priorità.
 *
 * Task L e Task H stampano un messaggio diagnostico per indicare su quale core sono in esecuzione
 * e poi saturano quel core per un tempo prestabilito (time_hog) con un ciclo NOP.
 *
 * I due task possono essere eseguiti indifferentemente sul core 0 e sul core 1 della ESP32.
 * 
 * Nella setup() provare a osservare come cambia il comportamento del programma modificando 
 * il pinning del task nell'istruzione xTaskCreatePinnedToCore().
 * Le possibili opzioni sono: PRO_CPU_NUM, APP_CPU_NUM e tskNO_AFFINITY
 *
 * Nota: nel file soc.h sono definiti i riferimenti ai due core della ESP32:
 *   #define PRO_CPU_NUM (0)
 *   #define APP_CPU_NUM (1)
 *
 * Qui viene adoperata la APP_CPU
 *
 */

#include <Arduino.h>

// Impostazioni  ***************************************************************
// la CPU viene monopolizzata per time_hog ms
static const TickType_t time_hog = 200;  

// Variabili Globali  **********************************************************


//******************************************************************************
// Funzioni ausiliarie

// monopolizza il processore. Temporizzazione accurata per tempi <= 1 secondo
static void hog_delay(uint32_t ms) {
  for (uint32_t i = 0; i < ms; i++) {
    for (uint32_t j = 0; j < 40000; j++) {
      asm("nop");  // istruzione NO-OPERATION della CPU
    }
  }
}

//*****************************************************************************
// Tasks

// Task L (bassa priorità)
void doTaskL(void *parameters) {
  
  TickType_t timestamp;
  char str[20];

  // Ciclo infinito del task
  while (1) {

    // Stampa un messaggio diagnostico
    sprintf(str, "Task L, Core %i\r\n", xPortGetCoreID());
    Serial.print(str);
    
    // Monopolizza la CPU per un certo tempo in un loop a vuoto
    // NOTA: in una applicazione reale NON è mai una buona idea!
    hog_delay(time_hog);
  }
}

// Task H (alta priorità)
void doTaskH(void *parameters) {
  
  TickType_t timestamp;
  char str[20];

  // Ciclo infinito del task
  while (1) {

    // Stampa un messaggio diagnostico
    sprintf(str, "Task H, Core %i\r\n", xPortGetCoreID());
    Serial.print(str);
    
    // Monopolizza la CPU per un certo tempo in un loop a vuoto
    hog_delay(time_hog);
  }
}

//************************************************************************************
// Main (sul core 1, con priorità 1)

// configurazione del sistema
void setup()
{
  // Configurazione della seriale
  Serial.begin(115200);

  // breve pausa
  vTaskDelay(pdMS_TO_TICKS(1000));
  Serial.println();
  Serial.println("FreeRTOS Esempio di task and core affinity");

  // Crea e avvia i due task; 
  //  NOTA: provare a sostituire il pinning del task da APP_CPU_NUM a tskNO_AFFINITY

  // Start Task L (bassa priorità)
  xTaskCreatePinnedToCore(doTaskL,
                          "Task L",
                          3072,
                          NULL,
                          1,
                          NULL,
                          PRO_CPU_NUM);

  // Start Task H (alta priorità)
  xTaskCreatePinnedToCore(doTaskH,
                          "Task H",
                          3072,
                          NULL,
                          2,
                          NULL,
                          tskNO_AFFINITY);

  // Elimina il task con "Setup e Loop"
  vTaskDelete(NULL);
}

void loop()
{
  // lasciare vuoto
}
