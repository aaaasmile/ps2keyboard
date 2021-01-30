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

volatile int kbd_ptr;
volatile int keyval;
bool nextIsRelease = false;

const int PinCLK = 2; // Generating interrupts using CLK signal
const int PinDT = 3;  // Reading DT signal

void setup()
{
  pinMode(PinCLK, INPUT);
  pinMode(PinDT, INPUT);

  attachInterrupt(digitalPinToInterrupt(PinCLK), isr, FALLING);

  Serial.begin(9600);
  Serial.println("Start ps keyboard handler");
  Keyboard.begin();
}

void loop()
{
  int myKeyval = 0;
  if (11 == kbd_ptr)
  {
    myKeyval = (keyval >> 1) & 0xFF;
    keyval = 0;
    kbd_ptr = 0;
    Serial.println(myKeyval);
    if (myKeyval == 0xf0)
    {

      nextIsRelease = true;
    }
    else
    {
      if (nextIsRelease)
      {
        Keyboard.release(myKeyval);
        nextIsRelease = false;
      }
      else
      {
        Keyboard.press(myKeyval);
      }
    }
  }
}

void isr()
{
  keyval = keyval | (digitalRead(PinDT) << kbd_ptr); // legge un bit per volta (8 bit + start, stop e parity)
  kbd_ptr++;
}
