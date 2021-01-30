// DAlla tastiera logitec ps2 ho 4 fili: rosso, nero marrone e bianco
// il rosso e il nero vanno ai 5v di arduino nano
// Poi ho il clock e il data.
//Proviamo ad usare il A3 per i dati e A4 per il clock. I colori per il momento sono:
// verde: clock
// bianco: data
// che dovrebbe essere lo standard logitech
// Questa potrebbe essere la base per avere una tastiera per l'Arduino Nano, nel caso
// ci volesse un input da tastiera. Esempio per aggiornare un display o per cambiare
// dei valori senza dover ricompilare il codice.

#include <Keyboard.h> // Questo funziona solo con Leonardo

volatile boolean KeyDetected;
volatile int keyval;

const int PinCLK = 2; // Generating interrupts using CLK signal
const int PinDT = 3;  // Reading DT signal

void setup()
{
  pinMode(PinCLK, INPUT);
  pinMode(PinDT, INPUT);

  attachInterrupt(0, isr, FALLING);

  Serial.begin(9600);
  Serial.println("Start ps keyboard handler");
  Keyboard.begin();
}

void loop()
{
  if (KeyDetected)
  {
    Serial.println(keyval);
    //Keyboard.print(keyval);
    //Keyboard.press(ctrlKey);
    //Keyboard.releaseAll();
    KeyDetected = false;
  }
}

// ISR(PCINT1_vect)
// {
//   keyval = 0;
//   for (int i = 0; i < 11; i++) // si leggono 8 bit dei dati + start,stop e parity bit
//   {
//     while (bitRead(PINC, 4) == true)
//       ;                              // aspetta il clock true
//     keyval |= bitRead(PINC, 3) << i; // legge il bit sul canale dei dati
//     while (bitRead(PINC, 4) == false)
//       ; // aspetta che il clock scende a false
//   }
//   keyval = (keyval >> 1) & 255; // toglie parity e stop
//   KeyDetected = true;
// }

void isr()
{
  delay(4); // delay for Debouncing
  if (digitalRead(PinCLK))
    keyval = digitalRead(PinDT);
  else
    keyval = !digitalRead(PinDT);
    
  KeyDetected = true;
}
