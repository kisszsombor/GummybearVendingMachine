// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _GummybearVendingMachine_H_
#define _GummybearVendingMachine_H_
#include "Arduino.h"
//add your includes for the project GummybearVendingMachine here
#include "Arduino.h"
#include <Wire.h> // Wire Bibliothek einbinden
#include <Keypad.h>
#include <LiquidCrystal_I2C.h> // Vorher hinzugefugte LiquidCrystal_I2C Bibliothek einbinden
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // include i/o class header

//end of add your includes here


//add your function definitions for the project GummybearVendingMachine here
void startScreen();

void startGame(int selectedGame);

void startRound(int selectedGame, int korSorszama);

void checknumber(int n);

void feladatScreen(int sorszam, int randNumber1, int randNumber2, int muvelet);

void kiertekel(int megoldas, int tipp);

void kiertekelJelszo();

void stopGame();

void resetGame();

void resetInputNumbers();

void openDoor();

void jelszo();

void toggleActiveGame();

void debug(int nr);



//Do not add code below this line
#endif /* _GummybearVendingMachine_H_ */
