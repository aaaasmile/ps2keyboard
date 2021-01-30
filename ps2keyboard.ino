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

char ScancodeToASCII[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 94, 0, 0, 0, 0, 0, 0, 113, 49, 0, 0, 0, 121, 115, 97, 119, 50, 0,
    0, 99, 120, 100, 101, 52, 51, 0, 0, 32, 118, 102, 116, 114, 53, 0, 0, 110, 98, 104, 103, 122, 54, 0, 0, 0, 109, 106, 117, 55, 56, 0,
    0, 44, 107, 105, 111, 48, 57, 0, 0, 46, 45, 108, 148, 112, 0, 0, 0, 0, 132, 0, 129, 96, 0, 0, 0, 0, 10, 43, 0, 35, 0, 0,
    0, 60, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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
      char ch = ScancodeToASCII[myKeyval & 127];
      if (nextIsRelease)
      {
        Keyboard.release(ch);
        nextIsRelease = false;
      }
      else
      {

        Keyboard.press(ch);
      }
    }
  }
}

void isr()
{
  keyval = keyval | (digitalRead(PinDT) << kbd_ptr); // legge un bit per volta (start, 8 bitdata, stop e parity)
  kbd_ptr++;
}
