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
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 00 - 10
    0x00, 0x00, 0x00, 0x09, 0x5E, 0x00, 0x00, 0x00, 0x00, 0x00, // 11 - 20
    0x00, 0x71, 0x32, 0x00, 0x00, 0x00, 0x79, 0x73, 0x61, 0x77, // 21 - 30
    0x32, 0x00, 0x00, 0x63, 0x78, 0x64, 0x65, 0x34, 0x33, 0x00, // 31 - 40
    0x00, 0x20, 0x76, 0x66, 0x74, 0x72, 0x35, 0x00, 0x00, 0x6E, // 41 - 50
    0x62, 0x68, 0x67, 0x7A, 0x36, 0x00, 0x00, 0x00, 0x6D, 0x6A, // 51 - 60
    0x75, 0x37, 0x38, 0x00, 0x00, 0x2C, 0x6B, 0x69, 0x6F, 0x30, // 61 - 70
    0x39, 0x00, 0x00, 0x2E, 0x2D, 0x6C, 0x94, 0x70, 0x00, 0x00, // 71 - 80
    0x00, 0x00, 0x84, 0x00, 0x81, 0x60, 0x00, 0x00, 0x00, 0x00, // 81 - 90
    0x0A, 0x2B, 0x00, 0x23, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, // 91 - 100
    0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 101 - 110
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x00, // 111 - 120
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};            // 121 - 127

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
