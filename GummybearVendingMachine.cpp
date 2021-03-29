// Do not remove the include below
#include "GummybearVendingMachine.h"
#include "Arduino.h"
#include <Wire.h> // Wire Bibliothek einbinden
#include <Keypad.h>
#include <LiquidCrystal_I2C.h> // Vorher hinzugefugte LiquidCrystal_I2C Bibliothek einbinden
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // include i/o class header

#define LCD_TYPE_I2C_20_4 // 20x4 LCD with I2C module --> Zso
//#define LCD_TYPE_I2C_16_2 // 16x2 LCD with I2C module --> Gyuri

/* CONFIGURATION */

/* Set up LCD*/

#ifdef LCD_TYPE_I2C_16_2
LiquidCrystal_I2C lcd(0x27, 16, 2); //Hier wird festgelegt um was fur einen Display es sich handelt. In diesem Fall eines mit 16 Zeichen in 2 Zeilen und der HEX-Adresse 0x27. Fur ein vierzeiliges I2C-LCD verwendet man den Code "LiquidCrystal_I2C lcd(0x27, 20, 4)"
#endif /* LCD_TYPE_I2C_16_2 */

#ifdef LCD_TYPE_I2C_20_4
//LiquidCrystal_I2C lcd(0x27, 20, 4); //Hier wird festgelegt um was fur einen Display es sich handelt. In diesem Fall eines mit 16 Zeichen in 2 Zeilen und der HEX-Adresse 0x27. Fur ein vierzeiliges I2C-LCD verwendet man den Code "LiquidCrystal_I2C lcd(0x27, 20, 4)"
hd44780_I2Cexp lcd; // declare lcd object: auto locate & config display for hd44780 chip
#endif /* LCD_TYPE_I2C_20_4 */

/* Set up keypad */

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads


char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

int piros = 13; // the pin the LED is connected to
int zold = 12;
int retesz = 11;

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

/* GLOBAL CONSTANTS / PARAMETERS */
int GV_jelszo = 789;
const long GC_interval = 12000; //  [ms] length of a round
const int GC_NrOfRounds = 9; //  [db] number of rounds in a game, starts from 0, so 9 means 10
const long GC_TiTextDispLong = 3000; // [ms] duration of text display nyertel/vege stb.
const long GC_TiTextDispShort = 1500; // [ms] duration of text display nyertel/vege stb.


/* GLOBAL VARIABLES */

// Variables to hold entered number on Keypad
volatile int GV_firstnumber = 99;  // used to tell how many numbers were entered on keypad
volatile int GV_secondnumber = 99;
volatile int GV_thirdnumber = 99;
volatile int GV_fourthnumber = 99;
int GV_tipp = 9999; // concatenated input number
int GV_korSzama = 0; // globalis valtozo ami mutatja az aktualis kor sorszamat
int GV_eredmeny = 0;
int GV_selectedGame = 1; // 1: Reka 2: Gabi osszeadas 3: Gabi szorzas
bool GV_jatekFolyamatban = false;
bool GV_jelszoBekereseFolyamatban = false;
bool GV_jelszoHelyes = false;
bool GV_enaTimeoutCheck = false; // flag to enable timeout check during a running game
unsigned long GV_startMillis, GV_currentMillis;

//The setup function is called once at startup of the sketch
void setup() {

	#ifdef LCD_TYPE_I2C_16_2
		  lcd.init(); //Im Setup wird der LCD gestartet //only for 16x2 display
		  lcd.backlight(); //Hintergrundbeleuchtung einschalten (lcd.noBacklight(); schaltet die Beleuchtung aus). //only for 16x2 display
	#endif /* LCD_TYPE_I2C_16_2 */

	#ifdef LCD_TYPE_I2C_20_4
		  lcd.begin(20, 4); // in “20, 4” ändern wenn 20x04 benutzt wird
	#endif /* LCD_TYPE_I2C_20_4 */


	  pinMode(piros, OUTPUT);
	  pinMode(zold, OUTPUT);
	  pinMode(retesz, OUTPUT);
	  lcd.clear();
	  startScreen();
}

// The loop function is called in an endless loop
void loop() {
	/*lcd.setCursor(0, 0);
	 lcd.print("AZ-Delivery");
	 lcd.setCursor(0, 1);
	 lcd.print(millis() / 1000);
	*/

	char keypressed = customKeypad.getKey();  // Get value of keypad button if pressed
	if (keypressed != NO_KEY){  // If keypad button pressed check which key it was
		switch (keypressed) {

		  case '1':
			  if (GV_jatekFolyamatban || GV_jelszoBekereseFolyamatban) {checknumber(1);}
		  break;

		  case '2':
			  if (GV_jatekFolyamatban || GV_jelszoBekereseFolyamatban) {checknumber(2);}
		  break;

		  case '3':
			  if (GV_jatekFolyamatban || GV_jelszoBekereseFolyamatban) {checknumber(3);}
		  break;

		  case '4':
			  if (GV_jatekFolyamatban || GV_jelszoBekereseFolyamatban) {checknumber(4);}
		  break;

		  case '5':
			  if (GV_jatekFolyamatban || GV_jelszoBekereseFolyamatban) {checknumber(5);}
		  break;

		  case '6':
			  if (GV_jatekFolyamatban || GV_jelszoBekereseFolyamatban) {checknumber(6);}
		  break;

		  case '7':
			  if (GV_jatekFolyamatban || GV_jelszoBekereseFolyamatban) {checknumber(7);}
		  break;

		  case '8':
			  if (GV_jatekFolyamatban || GV_jelszoBekereseFolyamatban) {checknumber(8);}
		  break;

		  case '9':
			  if (GV_jatekFolyamatban || GV_jelszoBekereseFolyamatban) {checknumber(9);}
		  break;

		  case '0':
			  if (GV_jatekFolyamatban || GV_jelszoBekereseFolyamatban) {checknumber(0);}
		  break;

		  case 'A':
			  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			   * Az "A" gomb kulonbozo lehetseges szerepei:
			   *  - kezdokepernyon jatek inditasa
			   *  - jatek alatt megallitas, eredmeny mutatasa, majd kezdokepernyo mutatasa
			   *  - jelszobekeres alatt ujrainditas, kezdokepernyo mutatasa
			   *  - ha admin menu aktiv, akkor pedig ajto nyitasa
			   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			  //GV_jelszoBekereseFolyamatban = false; // ha epp az az ablak volt aktiv
			  if (!GV_jatekFolyamatban && !GV_jelszoBekereseFolyamatban && !GV_jelszoHelyes) {startGame(GV_selectedGame);}
			  else if (GV_jelszoBekereseFolyamatban) {resetGame();}
			  else if (GV_jatekFolyamatban) {stopGame();}
			  else if (GV_jelszoHelyes) {openDoor();}
		  break;

		  case 'B':
			  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			   * A "B" gomb kulonbozo lehetseges szerepei:
			   *  - aktiv admin menuben jatek mod valtasa
			   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			  //return;
			  //if (GV_jatekFolyamatban == false) {toggleActiveGame();}
			  if (GV_jelszoHelyes) {toggleActiveGame();}
		  break;

		  case 'C':
			  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			   * A "C" gomb kulonbozo lehetseges szerepei:
			   *  - semmi
			   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			  //return;
		  break;

		  case 'D':
			  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			   * A "D" gomb kulonbozo lehetseges szerepei:
			   *  - kezdokepernyon admin jelszo bekerese
			   *  - ha mar aktiv az admin menu, nem csinal semmit
			   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			  if (!GV_jatekFolyamatban && !GV_jelszoHelyes) {jelszo();}
		  break;

		  case '*':
			  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
			   * A "*" gomb kulonbozo lehetseges szerepei:
			   *  - jatek alatt jatek megallitasa, kezdokepernyo mutatasa
			   *  - ha admin menu aktiv, akkor pedig visszagomb es beirt jelszo torlese
			   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
			  if (GV_jatekFolyamatban) {stopGame();}
			  else if (GV_jelszoHelyes) {
				  GV_jelszoHelyes = false; // reset admin menu
				  resetGame(); // go back to initial screen
			  }
			  else {
				  resetGame();
			  }
		  break;

		  case '#':
			  if (GV_jatekFolyamatban) {kiertekel(GV_eredmeny, GV_tipp);}
			  else if (GV_jelszoBekereseFolyamatban) {kiertekelJelszo();}
		  break;

		} // end switch-case

	  } // end if (keypressed != NO_KEY)

	if (GV_jatekFolyamatban && GV_enaTimeoutCheck){
		// timeout miatti game over monitorozasa
		//a GV_startMillis minden kor elejen frissul
		//instant helyessegellenorzes
		if (GV_tipp == GV_eredmeny) {kiertekel(GV_eredmeny, GV_tipp);}
		// read in current run time
		GV_currentMillis = millis();

		// check for timeout
		if (GV_currentMillis - GV_startMillis >= GC_interval) {
			// timeout, game over
			//stopGame();
			// check input so far
			kiertekel(GV_eredmeny, GV_tipp);
		}
	}

	if (GV_jelszoBekereseFolyamatban){
		// egy ido utan alljon vissza a kezdokepernyo, ha a jelszo bekeres aktiv
		//a GV_startMillis a jelszo() fuggvenybe lepeskor frissul

		// read in current run time
		GV_currentMillis = millis();

		// check for timeout
		if (GV_currentMillis - GV_startMillis >= GC_interval) {
			// timeout, jelszobekeres vege
			GV_jelszoBekereseFolyamatban = false;
			resetGame();

		}
	}

} // end void loop()


/*************************************************
 * FUNCTIONS
 * ***********************************************/

void startScreen() {
	/*
	 * display the initial screen
	 * dependent on LCD type
	 */
	lcd.clear();
	lcd.setCursor(0, 0);

	#ifdef LCD_TYPE_I2C_16_2
		lcd.print("AKARSZ JATSZANI?");
		lcd.setCursor(0, 1);
		lcd.print("NYOMD MEG AZ A-T");
	#endif /* LCD_TYPE_I2C_16_2 */

	#ifdef LCD_TYPE_I2C_20_4
		lcd.print("Szeretnel jatszani?");
		lcd.setCursor(0, 1);
		lcd.print("Nyomd meg az A-t!");
		lcd.setCursor(0, 2);
		lcd.print("Mod: ");
		//String text = ("Aktiv jatek: " + String(GV_selectedGame));
		//lcd.print(text);
		if (GV_selectedGame == 1) {
					lcd.print("osszead Reka");
				}
				else if (GV_selectedGame == 2) {
					lcd.print("osszead Gabi");
				}
				else if (GV_selectedGame == 3) {
					lcd.print("szorzas Gabi");
				}
				else if (GV_selectedGame == 4) {
					lcd.print("osztas");
				}
				else {lcd.print("hiba!");}
		lcd.setCursor(0, 3);
		lcd.print("Admin menu: D");
	#endif /* LCD_TYPE_I2C_20_4 */
}

void startGame(int selectedGame){
	// start game

	GV_korSzama = 0; // Kor szamanak kinullazasa

	GV_jatekFolyamatban = true; // flag, ami mutatja, hogy elindult a jatek

	// LEDek kikapcsolasa
	digitalWrite(piros, LOW);
	digitalWrite(zold, LOW);

	if(GV_korSzama == 0){
		// elso kor
		GV_startMillis = millis(); // save timestamp of the start of the round
		startRound(selectedGame, GV_korSzama);
		//GV_korSzama++;

	}
}

void startRound(int selectedGame, int korSorszama){
	// start rounds in an already active game

	int rand1 = 0, rand2 = 0, result = 0, muvelet = 0;

	switch (selectedGame) {
		case 1:
			// Reka - osszeadas
			rand1 = random(2,8);
			rand2 = random(2,7);
			result = rand1 + rand2;
			muvelet = 0; // osszeadas
		break;

		case 2:
			// Gabi - osszeadas bonyolult
			rand1 = random(2,50);
			rand2 = random(2,50);
			result = rand1 + rand2;
			muvelet = 1; // osszeadas
		break;

		case 3:
			// Gabi - szorzas
			rand1 = random(2,10);
			rand2 = random(2,10);
			result = rand1 * rand2;
			muvelet = 2; // szorzas
		break;

		case 4:
			// osztas
			int maradek = 1;
			do {
				rand1 = random(2,100);
				rand2 = random(2,50);
				maradek = rand1%rand2;
			} while ((maradek != 0) || ((maradek == 0) && (rand1 == rand2)));
			result = rand1/rand2;
			muvelet = 3; // osztas
		break;
	}

	GV_eredmeny = result; // elmenteni az adott kor eredmenyet globalis valtozoba

	feladatScreen(korSorszama, rand1, rand2, muvelet); // feladat kiiratasa

	GV_startMillis = millis(); // save timestamp of the start of the round
	GV_enaTimeoutCheck = true; // enable timeout check from here on
	GV_korSzama++;

	return;
}

void checknumber(int n){
  if (GV_firstnumber == 99) {
	// Check if this is the first number entered
	GV_firstnumber = n;
	String displayvalue = String(GV_firstnumber);  //  Transform int to a string for display
	lcd.setCursor(0, 1);
	lcd.print(displayvalue);
	GV_tipp = GV_firstnumber;
  }
  else  if (GV_secondnumber == 99) {
	  // Check if it's the second number entered
	  GV_secondnumber = n;
	  String displayvalue = (String(GV_firstnumber) + String(GV_secondnumber));
	  lcd.setCursor(0, 1);
	  lcd.print(displayvalue);
	  GV_tipp = GV_firstnumber*10 + GV_secondnumber;
	}
  else if (GV_thirdnumber == 99) {
	  // It must be the 3rd number entered
	  GV_thirdnumber = n;
	  String displayvalue = (String(GV_firstnumber) + String(GV_secondnumber) + String(GV_thirdnumber));
	  lcd.setCursor(0, 1);
	  lcd.print(displayvalue);
	  GV_tipp = GV_firstnumber*100 + GV_secondnumber*10 + GV_thirdnumber;
	}
  else if (GV_fourthnumber == 99) {
		// It must be the 4th number entered
		GV_fourthnumber = n;
		String displayvalue = (String(GV_firstnumber) + String(GV_secondnumber) + String(GV_thirdnumber) + String(GV_fourthnumber));
		lcd.setCursor(0, 1);
		lcd.print(displayvalue);
		GV_tipp = GV_firstnumber*1000 + GV_secondnumber*100 + GV_thirdnumber*10 + GV_fourthnumber;
  }
  else {
	  // all 4 characters are full, stop
  }
}

void feladatScreen(int sorszam, int randNumber1, int randNumber2, int muvelet) {
	/*
	 *
	 * feladat kijelzese
	 *
	 */
	String displayvalue;

	// feladat kiirasa kijelzore
	lcd.clear();
	lcd.setCursor(0, 0); //Hier wird die Position des ersten Zeichens festgelegt. In diesem Fall bedeutet (0,0) das erste Zeichen in der ersten Zeile.
	lcd.print(sorszam+1);
	lcd.print(".  ");
	//String displayvalue = (String(randNumber1) + " + " + String(randNumber2));
	displayvalue = String(randNumber1);
	lcd.print(displayvalue);

	if (muvelet == 0 || muvelet == 1) {lcd.print(" + ");} // osszeadas
	else if (muvelet == 2) {lcd.print(" x ");} // szorzas
	else if (muvelet == 3) {lcd.print(" : ");} // osztas
	else {lcd.print(" err ");}

	displayvalue = String(randNumber2);
	lcd.print(displayvalue);
	//lcd.print(" + ");
	//lcd.print(randNumber2);
	lcd.print(" = ");
	lcd.setCursor(0, 1);// In diesem Fall bedeutet (0,1) das erste Zeichen in der zweiten Zeile.
	lcd.cursor(); // Turn on the cursor

	//delay(1000); // ez kell ide?
}

void kiertekel(int megoldas, int tipp) {
	/* ****************************************
	 * megoldas: a helyes megoldas
	 * tipp: a user altal bepotyogott eredmeny
	 * ****************************************/

	GV_enaTimeoutCheck = false; // disable timeout check while message is being displayed

	if (tipp == megoldas) {
		lcd.noCursor(); // Turn off the cursor
		lcd.setCursor(5, 1);
		lcd.print("HELYES");

		// reset pressed buttons
		resetInputNumbers();

		digitalWrite(zold, HIGH);
		delay(GC_TiTextDispLong);
		digitalWrite(zold, LOW);
		//GV_stateContinueGame=true;

		if (GV_korSzama == GC_NrOfRounds) {
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("GRATULALOK,");
			lcd.setCursor(0, 1);
			lcd.print("NYERTEL!");
			delay(GC_TiTextDispLong);
			//GV_stateContinueGame=false;
			openDoor();
			resetGame();
		}
		else {
			// game shall continue
			startRound(GV_selectedGame, GV_korSzama); // start next round
		}

	}
	else {
		lcd.noCursor(); // Turn off the cursor
		lcd.setCursor(5, 1);
		lcd.print("ROSSZ");
		lcd.setCursor(13, 1);
		lcd.print(megoldas);

		// reset pressed buttons
		GV_firstnumber = 99;
		GV_secondnumber = 99;
		GV_thirdnumber = 99;

		digitalWrite(piros, HIGH);
		delay(GC_TiTextDispLong);
		digitalWrite(piros, LOW);

		resetGame();
	}
}

void kiertekelJelszo() {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	* Az eddig beutott karakterek osszehasonlitasa a jelszoval.
	* Ha a jelszo stimmel, admin menu aktivalasa.
	* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	GV_jelszoBekereseFolyamatban = false;

	if (GV_tipp == GV_jelszo) {
		GV_jelszoHelyes = true;
		lcd.clear();
		lcd.setCursor(0, 0);

		#ifdef LCD_TYPE_I2C_16_2
			lcd.print("A: nyit B: mod");
			lcd.setCursor(0, 1);
			lcd.print("*: kilepes");
		#endif /* LCD_TYPE_I2C_16_2 */

		#ifdef LCD_TYPE_I2C_20_4
			lcd.print("Helyes jelszo!");;
			lcd.setCursor(0, 1);
			lcd.print("A: nyit, B: jatekmod");
			lcd.setCursor(0, 2);
			lcd.print("*: kilepes");
		#endif /* LCD_TYPE_I2C_20_4 */

		//delay(GC_TiTextDispShort);
		//openDoor();
	}
	else {
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("Hibas jelszo,");
		lcd.setCursor(0, 1);
		lcd.print("szolj Apanak!");
		delay(GC_TiTextDispShort);
		resetGame();
	}

}

void stopGame(){
	// called in case of time out (game over) or when pressing '*' key

	GV_enaTimeoutCheck = false; // disable timeout check while message is being displayed
	lcd.noCursor(); // Turn off the cursor

#ifdef LCD_TYPE_I2C_16_2
	lcd.setCursor(0, 1);
	lcd.print(GV_eredmeny);
	lcd.setCursor(10, 1);
	lcd.print("VEGE!");
#endif /* LCD_TYPE_I2C_16_2 */

#ifdef LCD_TYPE_I2C_20_4
	lcd.setCursor(0, 1);
	lcd.print(GV_eredmeny);
	lcd.print(" a helyes valasz");
	lcd.setCursor(0, 3);
	lcd.print("       VEGE!        ");
#endif /* LCD_TYPE_I2C_20_4 */



	digitalWrite(piros, HIGH);
	delay(GC_TiTextDispLong);
	digitalWrite(piros, LOW);

	resetGame();
}

void resetGame(){
	// set back everything to default
	GV_korSzama = 0;
	GV_tipp = 999;
	GV_eredmeny = 0;
	GV_jatekFolyamatban = false;
	GV_jelszoHelyes = false;
	resetInputNumbers();
	startScreen();
}

void resetInputNumbers(){
	// set back input numbers to default
	GV_firstnumber = 99;
	GV_secondnumber = 99;
	GV_thirdnumber = 99;
	GV_fourthnumber = 99;
}

void openDoor(){
	// open gumicukor door

	#ifdef LCD_TYPE_I2C_16_2
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print(" Szezam tarulj! ");
	#endif /* LCD_TYPE_I2C_16_2 */

	#ifdef LCD_TYPE_I2C_20_4
		lcd.setCursor(0, 3);
		lcd.print("   Szezam tarulj!   ");
	#endif /* LCD_TYPE_I2C_20_4 */

	GV_jelszoHelyes = false;

	digitalWrite(retesz, HIGH);
	delay(GC_TiTextDispLong);
	digitalWrite(retesz, LOW);


	kiertekelJelszo(); //go back to admin menu
}

void jelszo(){
	GV_jelszoBekereseFolyamatban = true;
	//GV_jatekFolyamatban = false;
	GV_startMillis = millis();

	resetInputNumbers();
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Jelszo, majd #:");
	lcd.setCursor(0, 1);
}

void toggleActiveGame(){
		//toggle game number on start screen
		if (GV_selectedGame == 1) {
			GV_selectedGame = 2;
		}
		else if (GV_selectedGame == 2) {
			GV_selectedGame = 3;
		}
		else if (GV_selectedGame == 3) {
			GV_selectedGame = 4;
		}
		else if (GV_selectedGame == 4) {
			GV_selectedGame = 1;
		}
		else {GV_selectedGame = 1;}

		//display selected game
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("AKTIV JATEK:");
		lcd.setCursor(0, 1);
		lcd.print(String(GV_selectedGame));
		lcd.print(": ");
		if (GV_selectedGame == 1) {
			lcd.print("osszead Reka");
		}
		else if (GV_selectedGame == 2) {
			lcd.print("osszead Gabi");
		}
		else if (GV_selectedGame == 3) {
			lcd.print("szorzas Gabi");
		}
		else if (GV_selectedGame == 4) {
			lcd.print("osztas");
		}
		else {lcd.print("hiba!");}
		delay(GC_TiTextDispShort);

		kiertekelJelszo(); 		//go back to admin menu
}

void debug(int nr) {
	// display DEBUG character in the bottom right
	// only on 20x4 display

	#ifdef LCD_TYPE_I2C_20_4
		lcd.setCursor(18, 3);
		lcd.print(nr);
		delay(2000);
	#endif /* LCD_TYPE_I2C_20_4 */
}
