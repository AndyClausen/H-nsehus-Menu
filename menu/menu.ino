#include <Time.h>
#include <TimeLib.h>

#include <EEPROM.h>

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void printDigits(int digits) {
  if (digits < 10)
    lcd.print("0");
  lcd.print(digits);
}

int mainAmt = 2;
int levelOneAmt[] = {1, 1, 1};

void openNest() {
  lcd.setCursor(0, 0);
  lcd.print("?bner reden.    "); delay(500);
  lcd.setCursor(12, 0);
  lcd.print("."); delay(500); lcd.print("."); delay(500);
  lcd.setCursor(0, 0);
  lcd.print("?bner reden.    "); delay(500);
  lcd.setCursor(12, 0);
  lcd.print("."); delay(500); lcd.print("."); delay(500);
}
void closeNest() {
  lcd.setCursor(0, 0);
  lcd.print("Lukker reden.   "); delay(500);
  lcd.setCursor(13, 0);
  lcd.print("."); delay(500); lcd.print("."); delay(500);
  lcd.setCursor(0, 0);
  lcd.print("Lukker reden.   "); delay(500);
  lcd.setCursor(13, 0);
  lcd.print("."); delay(500); lcd.print("."); delay(500);
}

void openDoors() {
  lcd.setCursor(0, 0);
  lcd.print("?bner d?rene.   "); delay(500);
  lcd.setCursor(13, 0);
  lcd.print("."); delay(500); lcd.print("."); delay(500);
  lcd.setCursor(0, 0);
  lcd.print("?bner d?rene.   "); delay(500);
  lcd.setCursor(13, 0);
  lcd.print("."); delay(500); lcd.print("."); delay(500);
}
void closeDoors() {
  lcd.setCursor(0, 0);
  lcd.print("Lukker d?rene.  "); delay(500);
  lcd.setCursor(14, 0);
  lcd.print("."); delay(500); lcd.print("."); delay(500);
  lcd.setCursor(0, 0);
  lcd.print("Lukker d?rene.  "); delay(500);
  lcd.setCursor(14, 0);
  lcd.print("."); delay(500); lcd.print("."); delay(500);
}


int mainCounter = 0;
int level = 0;
int levelOneCounter = 0;
String mainTexts[] = {"Rede           ", "D?re           ", "Indstillinger  "};
String levelOneTexts[][3] = {{"?bn rede       ", "Luk rede        "}, {"?bn d?re        ", "Luk d?re        "}, {"Indstil tid     ", "Indstil dato    "}};

int tMin;
int tHour;
int tDay;
int tMonth;
int tYear;
int curStep = 0;
void setTimeFunc() {
  //Serial.println("setTimeFunc begyndt...");
  if (curStep == 0) {
    tMin = minute();
    tHour = hour();
    curStep++;
  }
  else if (curStep == 1) {
    //Serial.print("curStep = 1");
    ReadKeyFunc();
    lcd.setCursor(0, 1);
    lcd.print("V?lg time       ");
    lcd.setCursor(11, 1);
    printDigits(tHour); lcd.print(":"); printDigits(tMin);
  }
  else if (curStep == 2) {
    //Serial.println("curStep = 2");
    ReadKeyFunc();
    lcd.setCursor(0, 1);
    lcd.print("V?lg minut      ");
    lcd.setCursor(11, 1);
    printDigits(tHour); lcd.print(":"); printDigits(tMin);
  }
  else if (curStep == 3) {
    setTime(tHour, tMin, second(), day(), month(), year());
    EEPROM.write(0, tHour);
    EEPROM.write(1, tMin);
    tHour = 0;
    tMin = 0;
    level = 1;
    curStep = 0;
  }
}
void setDateFunc() {
  if (curStep == 0) {
    tDay = day();
    tMonth = month();
    tYear = year();
    curStep++;
  }
  else if (curStep == 1) {
    //Serial.print("curStep = 1");
    ReadKeyFunc();
    lcd.setCursor(0, 1);
    lcd.print("V?lg dag        ");
    lcd.setCursor(11, 1);
    printDigits(tDay);
  }
  else if (curStep == 2) {
    //Serial.println("curStep = 2");
    ReadKeyFunc();
    lcd.setCursor(0, 1);
    lcd.print("V?lg m?ned      ");
    lcd.setCursor(11, 1);
    printDigits(tMonth);
  }
  else if (curStep == 3) {
    //Serial.println("curStep = 3");
    ReadKeyFunc();
    lcd.setCursor(0, 1);
    lcd.print("V?lg ?r         ");
    lcd.setCursor(11, 1);
    printDigits(tYear);
  }
  else if (curStep == 4) {
    setTime(hour(), minute(), second(), tDay, tMonth, tYear);
    EEPROM.write(3, tDay);
    EEPROM.write(4, tMonth);
    EEPROM.write(5, tYear-1970);
    tDay = 0;
    tMonth = 0;
    tYear = 0;

    level = 1;
    curStep = 0;
  }
  else {
    level = 1;
    curStep = 0;
  }
}

int readkey;
bool b_in_use = 0;
void ReadKeyFunc() {
  if (b_in_use == false) {
    //////////////////////// RIGHT //////////////////////// = 0
    if (readkey < 50) {
      // Nothing

    }//////////////////////// UP //////////////////////// = 99
    else if (readkey < 195) {
      if (level == 0) {
        if (mainCounter > 0)
          mainCounter--;
        else
          mainCounter = mainAmt;
      }
      else if (level == 1) {
        if (levelOneCounter > 0)
          levelOneCounter--;
        else
          levelOneCounter = levelOneAmt[mainCounter];
      }
      else if (level == 2) {
        if (mainCounter == 2) {
          if (levelOneCounter == 0) {
            if (curStep == 1) {
              if (tHour >= 23)
                tHour = 0;
              else
                tHour++;
            }
            else if (curStep == 2) {
              if (tMin >= 59)
                tMin = 0;
              else
                tMin++;
            }
          }
          else if (levelOneCounter == 1) {
            if (curStep == 1) {
              if (tDay >= 31)
                tDay = 1;
              else
                tDay++;
            }
            else if (curStep == 2) {
              if (tMonth >= 12)
                tMonth = 1;
              else
                tMonth++;
            }
            else if (curStep == 3) {
              tYear++;
            }
          }
        }
      }
    }//////////////////////// DOWN //////////////////////// = 254
    else if (readkey < 380) {
      if (level == 0) {
        if (mainCounter < mainAmt)
          mainCounter++;
        else
          mainCounter = 0;
      }
      else if (level == 1) {
        if (levelOneCounter < levelOneAmt[mainCounter])
          levelOneCounter++;
        else
          levelOneCounter = 0;
      }
      else if (level == 2) {
        if (mainCounter == 2) {
          if (levelOneCounter == 0) {
            if (curStep == 1) {
              if (tHour <= 0)
                tHour = 23;
              else
                tHour--;
            }
            else if (curStep == 2) {
              if (tMin <= 0)
                tMin = 59;
              else
                tMin--;
            }
          }
          else if (levelOneCounter == 1) {
            if (curStep == 1) {
              if (tDay <= 1)
                tDay = 31;
              else
                tDay--;
            }
            else if (curStep == 2) {
              if (tMonth <= 1)
                tMonth = 12;
              else
                tMonth--;
            }
            else if (curStep == 3) {
              if (tYear > 1970)
                tYear--;
              else
                tYear = 2016;
            }
          }
        }
      }
    }//////////////////////// LEFT //////////////////////// = 407
    else if (readkey < 574) {
      if (level == 1) {
        level--;
        levelOneCounter = 0;
      }
      else if (level == 2) {
        if (mainCounter == 2) {
          curStep = -1;
        }
      }
    }//////////////////////// SELECT //////////////////////// = 638
    else if (readkey < 902) {
      if (level == 0) { // From Main Menu
        if (mainCounter == 0 || mainCounter == 1 || mainCounter == 2)
          level++;
      }
      else if (level == 1) { // Level One Menus

        if (mainCounter == 0) { // Nest menu
          if (levelOneCounter == 0) // Open
            openNest();
          else if (levelOneCounter == 1) // Close
            closeNest();
        }
        else if (mainCounter == 1) { // Doors menu
          if (levelOneCounter == 0) // Open
            openDoors();
          else if (levelOneCounter == 1) // Close
            closeDoors();
        }
        else if (mainCounter == 2) { // Settings menu
          if (levelOneCounter == 0) { // Set time
            level++;
            setTimeFunc();
          }
          else if (levelOneCounter == 1) { // Set date
            level++;
            setDateFunc();
          }
        }
      }
      else if (level == 2) { // Level Two Menus

        if (mainCounter == 2) { // Set time and date menu
          curStep++;
        }
      }
    }
    else // Void (no buttons pressed) = 1023
      b_in_use = 0;
  }
  if (readkey < 902)
    b_in_use = 1;
  else
    b_in_use = 0;
}

void setup() {
  Serial.begin(9600);
  //Serial.println("Loading menu...");
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Indl?ser menu."); delay(500); lcd.print("."); delay(500); lcd.print(".");
  if (EEPROM.read(6) != 1) { // If time has never been saved to Arduino
    EEPROM.write(6, 1);
  }
  else {
    setTime(EEPROM.read(0), EEPROM.read(1), EEPROM.read(2), EEPROM.read(3), EEPROM.read(4), EEPROM.read(5) + 1970); // h:m:s d/m/y
  }
  Serial.println(EEPROM.read(0)); Serial.println(EEPROM.read(1)); Serial.println(EEPROM.read(2)); Serial.println(EEPROM.read(3)); Serial.println(EEPROM.read(4)); Serial.println(EEPROM.read(5) + 1970);
  delay(500);
  lcd.clear();
}
// Function for top bar
void InfoBar() {
  if (second() > 34 && second() < 39 ) {
    lcd.print("Temperatur: "); lcd.print("????");
  } else if ( second() > 4 && second() < 9 ) {
    lcd.print("Simon's H?nsehus");
  }
  else {
    printDigits(day()); lcd.print("/"); printDigits(month()); lcd.print("-"); lcd.print(year()); lcd.print(" "); printDigits(hour()); lcd.print(":"); printDigits(minute());
  }
}

// Saves the time and date on the Arduino
void saveTimeAndDate() {
  EEPROM.write(0, hour());
  EEPROM.write(1, minute());
  EEPROM.write(2, second());
  EEPROM.write(3, day());
  EEPROM.write(4, month());
  EEPROM.write(5, year() - 1970);
  //Serial.println("Saved time and date!");
}
bool saving = false;

void loop() {
  lcd.setCursor(0, 0);
  InfoBar();

  lcd.setCursor(0, 1);
  readkey = analogRead(0);
  ReadKeyFunc();
  if (second() == 0 && saving == false) {
    saveTimeAndDate();
    saving = true;
  }
  else if (second() > 0 && saving == true) {
    saving = false;
  }

  if (level == 0) {
    lcd.print(mainTexts[mainCounter]);
  } else if (level == 1) {
    lcd.print(levelOneTexts[mainCounter][levelOneCounter]);
  }

  if (curStep != 0){
    if (levelOneCounter == 0)
      setTimeFunc();
    else if(levelOneCounter == 1)
      setDateFunc();
  }

  delay(20);
}

