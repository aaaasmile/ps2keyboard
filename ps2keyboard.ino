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

// Questa tabella l'ho presa da: https://github.com/slu4coder/ArduinoPS2Keyboard/blob/master/EasyPS2Keyboard.ino
// Lookup table (in: SHIFT state and PS2 scancode => out: desired ASCII code) change for your country
char ScancodeToASCII[2][128] = {
  { 0,0,0,0,0,0,0,0,         0,0,0,0,0,9,94,0,         0,0,0,0,0,113,49,0,       0,0,121,115,97,119,50,0,    // w/o SHIFT or ALT(GR)
    0,99,120,100,101,52,51,0,0,32,118,102,116,114,53,0,0,110,98,104,103,122,54,0,0,0,109,106,117,55,56,0,
    0,44,107,105,111,48,57,0,0,46,45,108,148,112,0,0,  0,0,132,0,129,96,0,0,     0,0,10,43,0,35,0,0,
    0,60,0,0,0,0,8,0,        0,0,0,0,0,0,0,0,          0,0,0,0,0,0,27,0,         0,0,0,0,0,0,0,0  },
  { 0,0,0,0,0,0,0,0,         0,0,0,0,0,0,248,0,        0,0,0,0,0,81,33,0,        0,0,89,83,65,87,34,0,       // with SHIFT
    0,67,88,68,69,36,0,0,    0,0,86,70,84,82,37,0,     0,78,66,72,71,90,38,0,    0,0,77,74,85,47,40,0,
    0,59,75,73,79,61,41,0,   0,58,95,76,153,80,63,0,   0,0,142,0,154,0,0,0,      0,0,0,42,0,39,0,0,
    0,62,0,0,0,0,0,0,        0,0,0,0,0,0,0,0,          0,0,0,0,0,0,0,0,          0,0,0,0,0,0,0,0  } };
    

void setup() {
  noInterrupts(); // prima di settare interrupt, si disabilitano
  PCICR = 0b00000010; // enable Port C PCI (A,B,C le tre PCI port di arduiono nano. Qui si usa la C)
  PCMSK1 = 0b00010000; // il bit 4 è il pin A4, dove ho collegato il mio clock
  interrupts(); // riabilita gli interrupts
  Serial.begin(9600);
  Serial.println("Start ps keyboard handler");
}

void loop() {
 
}

ISR(PCINT1_vect)
{
  int val = 0;
  for(int i =0; i< 11; i++) // si leggono 8 bit dei dati + start,stop e parity bit
  {
    while(bitRead(PINC,4) == true); // aspetta il clock true
    val |= bitRead(PINC, 3) << i; // legge il bit sul canale dei dati
    while(bitRead(PINC,4) == false); // aspetta che il clock scende a false
  }
  val = (val >> 1) & 255; // toglie parity e stop
  
  static bool shiftIsActive = false;      // state of the SHIFT key
  static bool nextIsReleased = false;     // indicating that the next key was released

  switch (val)
  {
    case 18: case 89: shiftIsActive = !nextIsReleased; nextIsReleased = false; break;  // SHIFT LEFT, SHIFT RIGHT     
    case 240: nextIsReleased = true; break;                                            // key release indicator      
    default:                                                                           // any other key
      if (!nextIsReleased)                // is it a 'key pressed' event?
      {
        Serial.print(ScancodeToASCII[shiftIsActive][val & 127]);                       // do lookup raw -> ASCII
      }
      nextIsReleased = false;
      break;
  }
  PCIFR = 0x02; // cancella il PCI flag per non rientrare nello stesso interrupt
}
