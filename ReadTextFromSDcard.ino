#include <Wire.h> // Wire Bibliothek einbinden
#include <LiquidCrystal_I2C.h> // Vorher hinzugefügte LiquidCrystal_I2C Bibliothek einbinden
// include the SD library:
#include <SPI.h>
#include <SD.h>

#define ARRAYSIZE 11
// set up variables using the SD utility library functions:
File myFile;
LiquidCrystal_I2C lcd(0x27, 16, 2); //Hier wird festgelegt um was für einen Display es sich handelt. In diesem Fall eines mit 16 Zeichen in 2 Zeilen und der HEX-Adresse 0x27. Für ein vierzeiliges I2C-LCD verwendet man den Code "LiquidCrystal_I2C lcd(0x27, 20, 4)"

const int chipSelect = 4;
const int fwdButtonPin = 5;     // the number of the pushbutton pin
const int bwdButtonPin = 6;
const int button_Pause_Resume = 7;
const int revButtonPin = 8;

// variables will change:
int fwdButtonState = 0;         // variable for reading the pushbutton status
int bwdButtonState = 0;         // variable for reading the pushbutton status
int lastButtonState = 0;         // variable for reading the pushbutton status

int previousButtonState;  // the previous state of button
int currentButtonState;   // the current state of button
int revButtonState;

int counter = 0;
int cnt = 0;
String abc[ARRAYSIZE];
String list;

int minWordPerMinute = 200;
int maxWordPerMinute = 1000;

int incrementWordPerMinute = 0;
int decrementWordPerMinute = 0;

bool paused = false;
bool first10 = true;

void setup() {
  Serial.begin(38400);
  lcd.init(); //Im Setup wird der LCD gestartet
  lcd.backlight(); //Hintergrundbeleuchtung einschalten (lcd.noBacklight(); schaltet die Beleuchtung aus).
  incrementWordPerMinute = minWordPerMinute;
  pinMode(fwdButtonPin, INPUT);
  pinMode(bwdButtonPin, INPUT);
  pinMode(revButtonPin, INPUT);

  lcd.setCursor(0, 0);//Hier wird die Position des ersten Zeichens festgelegt. In diesem Fall bedeutet (0,0) das erste Zeichen in der ersten Zeile.
  lcd.print("\nInitializing SD card...");
  delay(200);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect))
  {
    lcd.print("Card failed, or not present");
    delay(200);
    // don't do anything more:
    return;
  }
}


void loop()  {
  lcd.clear();
  myFile = SD.open("myfile.txt"); // open the file for reading
  if (myFile) {
    while (myFile.available()) { //execute while file is available
      lcd.setCursor(4, 0);


      previousButtonState    = currentButtonState;
      currentButtonState = digitalRead(button_Pause_Resume); // read new state

      if (currentButtonState == HIGH)
      {
        paused = !paused;
      }

      if (paused == false)
      {
        list = myFile.readStringUntil(' ');

        if (cnt < 10 && first10 == true)
        {
          if (first10 == true)
          {
            abc[cnt] = list;
          }
          lcd.print(abc[cnt]); //display word
        }

        if (cnt >= 10)
        {
          cnt = 0;
          first10 = false;
        }

        if (first10 == false)
        {
          for (int i = 0; i < ARRAYSIZE - 1; i++)
          {
            abc[i] = abc[i + 1];
            //abc[i] = abc[i + 1];

          }
          abc[10] = list;
          lcd.print(abc[10]);
          //Serial.println(abc[9]); //display word
          reverseLogic();
        }
        cnt++;

      }

      else
      {
        lcd.print(list);
      }

      speedIncDec();
      delay(delayCalculator(incrementWordPerMinute));
      lcd.clear();
    }

    myFile.close(); //close file

  }
}

//function for increment and decrement speed of displaying word/Minute
void speedIncDec()
{
  fwdButtonState = digitalRead(fwdButtonPin);
  //word letter = myFile.read(); //read next character from file
  // if fwd button pushen then MinWordPerMinute should be added by 50 and it should not cross MaxWordPerMinute.
  if (fwdButtonState != lastButtonState)
  {
    if (fwdButtonState == HIGH && incrementWordPerMinute <= maxWordPerMinute)
    {
      incrementWordPerMinute = incrementWordPerMinute += 50;
      Serial.print("MinWordPerMinute:");
      Serial.print(incrementWordPerMinute);
      Serial.print("\n");
    }
    if (incrementWordPerMinute > maxWordPerMinute)
    {
      incrementWordPerMinute = maxWordPerMinute;
      Serial.print("You have reached maximum limit");
      Serial.print("\t MinWordPerMinute: ");
      Serial.print(incrementWordPerMinute);
    }

  }

  // if bwd button pushen then MinWordPerMinute should be added by 50 and it should not cross MaxWordPerMinute.
  //lastButtonState = fwdButtonState;// save the current state as the last state,for next time through the loop
  bwdButtonState = digitalRead(bwdButtonPin);

  if (bwdButtonState != lastButtonState)
  {
    if (bwdButtonState == HIGH && incrementWordPerMinute <= maxWordPerMinute)
    {
      incrementWordPerMinute = incrementWordPerMinute -= 50;
      Serial.print("newMinWordPerMinute: ");
      Serial.print(incrementWordPerMinute);
      Serial.print("\n");
    }
    if (incrementWordPerMinute < minWordPerMinute)
    {
      incrementWordPerMinute = minWordPerMinute;
      Serial.print("DMinWordPerMinute: ");
      Serial.print(incrementWordPerMinute);
      Serial.print("\n You have reached minimum limit");
    }
  }
}

void reverseLogic()
{
  revButtonState = digitalRead(revButtonPin);

  if (revButtonState != lastButtonState)
  {
    if (revButtonState == HIGH)
    {
      paused = true;

      for (int i = 0; i < ARRAYSIZE - 1; i++)
      {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print(abc[i]);
        delay(delayCalculator(incrementWordPerMinute));
      }
      abc[10] = list;

      paused = false;
    }
  }
}
//Function for calculate deley according to word/Minute
float delayCalculator(int samplePerMinute)
{
  float samplerate = samplePerMinute / 60;
  float printDelay = 1000 / samplerate;
  return printDelay;
}
