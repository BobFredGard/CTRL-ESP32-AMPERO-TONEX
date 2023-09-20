#include <Arduino.h>
const char* Version = "V0.50"; //OK pour PACER 6 boutons + 6 - Problème sur retour écran Ampero

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C LCD(0x3F,16,2); 

#include <SPIFFS.h>

#include <sqlite3.h>
sqlite3 *db_base;

#include <OneButton.h>
//OneButton button4(26, true); OneButton button5(27, true); OneButton button6(14, true); OneButton button7(5, true);
//OneButton button1(26, true); OneButton button2(27, true); OneButton button3(14, true); 
OneButton button7(5, true);
OneButton button8(23, true);

#include <ESP32Encoder.h>
ESP32Encoder encoder1;
ESP32Encoder encoder2;
static int enc1Last, enc2Last;

#include <midi.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);
#define MIDI_CHANNEL_OMNI 0

#include <Wire.h>
//#include <Adafruit_MCP23X17.h>
//Adafruit_MCP23X17 mcp;
/* Couleurs (format RGB)
const byte COLOR_BLACK = 0b000;
const byte COLOR_RED = 0b100;
const byte COLOR_GREEN = 0b010;
const byte COLOR_BLUE = 0b001;
const byte COLOR_MAGENTA = 0b101;
const byte COLOR_CYAN = 0b011;
const byte COLOR_YELLOW = 0b110;
const byte COLOR_WHITE = 0b111;*/
//const byte rgbcolor [8] = {0b000, 0b100, 0b010, 0b001, 0b101, 0b011, 0b110, 0b111};

int i = 0;
static short choixBD, oldBank, Menus = 0; 
static short id, id_Init, Count, startScreen, Bank, Couleur1, Couleur2, Count1, Count2, startCharg, Canal1 = 1; static short Canal2 = 2;
static short preID = -1;
static int ampValbk, toneValbk = 0;

//unsigned long previousMillis=0 ;
//unsigned long interval = 1200L;

//static byte MIDICanal[17] = {0, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187namequickTonecount2, 188, 189, 190, 191};
//static byte MIDIPC[17] = {0, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207};

static short totalCC [2][54] =  {
  //  |------CC------>>> AMPERO II STOMPS <<<-------CC---------| |------------CC------------>>> TONE X <<<--------------------------CC---------------------------------------------------------------------------------------------|
  {0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 13, 16, 18, 20, 7, 102, 23, 25, 28, 79, 19, 15, 106, 107, 103, 14, 16, 17, 18, 20, 21, 22, 24, 26, 27, 29, 30, 104, 75, 85, 76, 77, 78, 108, 109, 110, 111, 112, 113, 114, 115},
  {0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,  25,  26,  27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,  40, 41, 42, 43, 45, 45,  46,  47,  48,  49,  50,  51,  52,  53}
  //  Numéro colonne de la base de donnée
};

                                   //  |--------CC----------|      |--Positions dans BD--|
static short Amperoquick [2] [7] = {{0, 7, 16, 18, 20, 13, 59}, {0, 17, 14, 15, 16, 13, 12}};
static short toneXquick [2] [7] = {{0, 102, 79, 19, 15, 106, 107}, {0, 18, 22, 23, 24, 25, 26}};

static const char* namequickAmp [7]  {"", "Volume     : ", "Param 1    : ", "Param 2    : ", "Param 3    : ", "Exp        : ", "Reverb on  :"};
static const char*  namequickTone [7] {"", "GAIN       : ", "REVERB     : ", "COMPRESSOR : ", "GATE       : ", "PRESENCE   : ", "DEPHT      : "};
static short ledS [7] = {0,21,22,23,24,25,26};
static short ledsColor [7] = {0,33,32,25,4,5,6};
static short encValmax1 [7] = {0,100, 127, 127, 127, 130, 130};
static short encValmin1 [7] = {0,0, 0, 0, 0, 129, 129};
static short encValmax2 [7] = {0,100, 127, 127, 127, 127, 127};
static short encValmin2 [7] = {0,0, 0, 0, 0, 0, 0};

static const char* namesCC[57] = {  "", 
  "Slot A1    : ", "Slot A2    : ", "Slot A3    : ", "Slot A4    : ", "Slot A5    : ", "Slot A6    : ",
  "Slot B1    : ", "Slot B2    : ", "Slot B3    : ", "Slot B4    : ", "Slot B5    : ", "Slot B6    : ",
  "Exp Pedal  : ", "Param 1    : ", "Param 2    : ", "Param 3    : ", "Volume     : ",
  "GAIN       : ", "BASS       : ", "MID        : ", "TREBLE     : ", "REVERB     : ", "COMPRESSOR : ",
  "NOISE GATE : ", "PRESENCE   : ", "DEPTH      : ", "MODEL.VOL  : ", "NOISE GT   : ", "NG REL     : ",
  "NG DPTH    : ", "COMP       : ", "CMP GAIN   : ", "COMP ATK   : ", "COMP PATC  : ",
  "BASS FRQ   : ", "MID Q      : ", "MID FRQ    : ", "TRBL FRQ   : ", "EQ PATCH   : ", "EQ MIX     : ",
  "REVERB     : ", "REV TYPE   : ", "REV TIME   : ", "VB PREDELAY: ", "REV COLOR  : ", "REV RESO   : ",
  "MIC 1      : ", "MIC 1 X    : ", "MIC 1 Z    : ", "MIC 2      : ", "MIC 2 X    : ", "MIC 2 Z    : ", "MICS MIX   : ",
  "NB FX MAX  : ", "PC Tone X  : ", "PC Ampero  : "
};

static short valmaxCC [57] = {0,
  130,130,130,130,130,130,
  130,130,130,130,130,130,
  130,127,127,127,100,
  127,127,127,127,127,127,
  127,127,127,127,140,127,
  127,130,127,127,140,
  127,127,127,127,140,127,
  130,127,127,127,127,127,
  127,127,127,127,127,127,127,
  61,150,300
};

static short Params[43][59]; static short paramsCopy[43][59];
static int progChang [43][2]; static int progChangcopy [43][2];

static const char* texteLine1;
static const char* texteLine2;
String sql;

// ----------------------------------------------GESTTION BASE DE DONNEE
static const char* daTa = "Callback function called";
char *zErrMsg = 0;

static int db_open(const char *filename, sqlite3 **db) {
  sqlite3_open(filename, db);
  return 0;
}

static int call(void *daTa, int argc, char **argv, char **azColName) {
  static int m;
  for (m = 0; m<argc; m++){
    String val = argv[m];
    String val2 = azColName[m];
    switch (choixBD){
      case 0 :
        if (m < 55) {Params[i][m] = val.toInt(); paramsCopy [i][m] = val.toInt();}
        if (m == 55) {progChang[i][0] = val.toInt(); progChangcopy[i][0] = val.toInt();;}
        if (m == 56) {progChang[i][1] = val.toInt(); progChangcopy[i][1] = val.toInt();;}
        if (m > 56) {Params[i][m] = val.toInt(); paramsCopy [i][m] = val.toInt();}
        //if (i==11){Serial.println(Params[i][m]);}
      break;
      case 1 :
      break;
    }    
  }
  id = 1;
  return 0;
}

static int db_exec(sqlite3 *db, const char *sql) {
  sqlite3_exec(db, sql, call, (void*)daTa, &zErrMsg);
  return 0;
}

void saveData() {
  if (db_open("/spiffs/base.db", &db_base)) return;
  sql = "UPDATE stomps SET ";
  for (i = 1; i < 57; i++){
    if (i < 55){
      if (Params[id][i] != paramsCopy[id][i]) {
        sql += "'"+String(i)+"'="+String(Params[id][i])+", "; 
      }
    }
    if (i == 55){
        sql += "'"+String(i)+"'="+String(progChang[id][0])+", "; 
    }
    if (i == 56){
        sql += "'"+String(i)+"'="+String(progChang[id][1]); 
    }
  }
  sql += " WHERE stomps_id="+String(id)+";";
  db_exec(db_base, sql.c_str());
  sqlite3_close(db_base);
  Serial.println(sql);
}

void readData (){
  if (db_open("/spiffs/base.db", &db_base)) return; 
  static byte o;
  if (Bank == 1 ) {o = id;}
  if (Bank == 2 ) {o = id + 43;}
  if (Bank == 3 ) {o = id + 85;}
  for (i = o; i < o + 42; i++) {
    choixBD = 0;
    sql = "SELECT * FROM stomps WHERE stomps_id = " + String(i) + ";";
    db_exec(db_base, sql.c_str());
    //Serial.println(sql);
  }
  sqlite3_close(db_base);
}
// ------------------------------------------FIN GESTTION BASE DE DONNEE

// ---------------------------------------------------------------ECRANS
void initEncoder(int val1) {
  switch (val1)  {
    case 1 :
      encoder1.setCount(Params[id][Amperoquick[1][Count1]]);
      enc1Last = Params[id][Amperoquick[1][Count1]];
      encoder2.setCount(Params[id] [toneXquick[1][Count2]]);
      enc2Last = Params[id] [toneXquick[1][Count2]];
    break;
    case 2 :
      encoder1.setCount(Count);
      enc1Last = Count;
    break;
    case 3 :
      encoder1.setCount(Params[id][Count]);
      enc1Last = Params[id][Count];
      encoder2.setCount(Params[id][Count]);
      enc2Last = Params[id][Count];
    break;
    case 4 :
      encoder2.setCount(progChang[id][0]); enc2Last = progChang[id][0];
    break;
    case 5 :
      encoder2.setCount(progChang[id][1]); enc2Last = progChang[id][1];
    break;
    case 6 :
      encoder2.setCount(Params[id][Count]); enc2Last = Params[id][Count];
    break;
  }
}

void Screens(byte choixscreen, int val2) {
  //LCD.clear();
  String texte;
  switch (choixscreen) {
    case 0 :
      LCD.clear();
      LCD.setCursor(1,0);
      LCD.print("AMPERO 2 STOMP");
      LCD.setCursor(5,1);
      LCD.print("TONE X");
    break;
    case 1 :
      if (Count > 0 && Count < 18){
        LCD.setCursor(0,0);
        if (texteLine1 != "P:    *AMPERO II") {texteLine1 = "P:    *AMPERO II"; LCD.print(texteLine1);}
        texte = String(id) + " ";
        LCD.setCursor(2,0); LCD.print(texte);
      }
      if (Count > 17 && Count < 54){
        LCD.setCursor(0,0);        
        if (texteLine1 != "P:       *TONE_X") {texteLine1 = ("P:       *TONE_X"); LCD.print(texteLine1);}
        LCD.setCursor(2,0); LCD.print(id);
      }
      if (Count == 54){if (texteLine1 != " Fin Sauvegd") {LCD.setCursor(5,0); texteLine1 = " Fin Sauvegd"; LCD.print(texteLine1);}}
      if (Count == 55 or Count == 56){if (texteLine1 != " Prog Change") {LCD.setCursor(5,0); texteLine1 = " Prog Change"; LCD.print(texteLine1);}}
      LCD.setCursor(0,1);
      if (texteLine2 != namesCC[Count]) {texteLine2 = namesCC[Count]; LCD.print(texteLine2);}
      LCD.setCursor(13,1); LCD.print("   ");
      LCD.setCursor(13,1); 
      if (Count != 55 && Count != 56) {
        if (val2 == 139 or val2 == 140) {
          if (val2 == 139) {LCD.setCursor(10,1); LCD.print(":  PRE"); val2 = 139;}
          else {LCD.setCursor(10,1); LCD.print(": POST"); val2 = 140;}
        }
        if (val2 == 130 or val2 == 129) {
            if (Params[id][Count] == 130) {LCD.print("ON "); val2 = 130;}
            else {LCD.print("OFF"); val2 = 129;}
        }
        if (Params[id][Count] < 128) {LCD.print(Params[id][Count]); val2 = -1;} 
      }
      if(Count == 55) {LCD.print(progChang[id][0]); val2 = -1;}
      if(Count == 56) {LCD.print(progChang[id][1]); val2 = -1;} 
    break;
    case 2 :
        LCD.setCursor(0,0);
        if (texteLine1 != "P:     AMPERO II") {texteLine1 = "P:     AMPERO II"; LCD.print(texteLine1);}
        texte = String(id) + " ";
        LCD.setCursor(2,0); LCD.print(texte);
        LCD.setCursor(0,1);
        if (texteLine2 != namequickAmp[Count1]) {texteLine2 = namequickAmp[Count1]; LCD.print(texteLine2);}
        LCD.setCursor(13,1); LCD.print("   ");
        LCD.setCursor(13,1);
        if (val2 == 139 or val2 == 140) {
          if (val2 == 139) {LCD.setCursor(10,1); LCD.print(":  PRE"); val2 = 139;}
          else {LCD.setCursor(10,1); LCD.print(": POST"); val2 = 140;}
        }
        if (val2 == 130 or val2 == 129) {
          if (val2 == 130) {LCD.print("ON "); val2 = 130;}
          else {LCD.print("OFF"); val2 = 129;}
        }
        if (Params[id][Amperoquick[1][Count1]] < 128) {LCD.print(Params[id][Amperoquick[1][Count1]]); val2 = -1;}
    break;
    case 3 :
        LCD.setCursor(0,0);
        if (texteLine1 != "P:        TONE_X") {texteLine1 = "P:        TONE_X"; LCD.print(texteLine1);}
        //LCD.setCursor(0,0); LCD.print("P:");
        texte = String(id) + " ";
        LCD.setCursor(2,0); LCD.print(texte);
        LCD.setCursor(0,1);
        if (texteLine2 != namequickTone[Count2]) {texteLine2 = namequickTone[Count2]; LCD.print(texteLine2);}
        LCD.setCursor(13,1); LCD.print("   ");
        LCD.setCursor(13,1);
        if (val2 == 139 or val2 == 140) {if (val2 == 139) {LCD.setCursor(10,1); LCD.print(":  PRE"); val2 = 139;}
          else {LCD.setCursor(10,1); LCD.print(": POST"); val2 = 140;}}
        if (val2 == 129 or val2 == 130) {
          if (val2 == 130) {LCD.print("ON "); val2 = 130;}
          else {LCD.print("OFF"); val2 = 129;}
        }
        if (Params[id][toneXquick[1][Count2]] < 128) {LCD.print(Params[id][toneXquick[1][Count2]]); val2 = -1;}
    break;
    case 10 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("** SAUVEGARDE **");
      LCD.setCursor(0,1);
      LCD.print("*** EN COURS ***");
    break;
  }
}

void displayColor1(byte color) {
  digitalWrite(ledsColor[0], bitRead(color, 2));
  digitalWrite(ledsColor[1], bitRead(color, 1));
  digitalWrite(ledsColor[2], bitRead(color, 0));
}

void displayColor2(byte color) {
  digitalWrite(ledsColor[5], bitRead(color, 2));
  digitalWrite(ledsColor[4], bitRead(color, 1));
  digitalWrite(ledsColor[5], bitRead(color, 0));
}

void progChange(byte CC, int val, byte canal) {
  MIDI.sendControlChange(0, CC, canal);
  MIDI.sendProgramChange(val, canal);
}

void midiblabla(byte a, byte b, byte c) {
      MIDI.sendControlChange(a, b, c);
      /*(""); Serial.print("ToneX -> CC : "); Serial.print(a); Serial.print(" - Val : ");
      Serial.print(b); Serial.print(" - Channel : "); Serial.println(c); */
}

void encod1(byte valmini, byte valmaxi, byte pot, byte sel){
  switch (sel) {
    case 0 :
        pot = encoder1.getCount();
        if (pot > valmaxi) {pot = valmaxi;}
        if (pot < valmini) {pot = valmini;}
        midiblabla(Amperoquick[0][Count1], pot, Canal2);
        Params[id][Amperoquick[1][Count1]] = pot;
        encoder1.setCount(pot);
        enc1Last = pot;
        Screens(2, Params[id][Amperoquick[1][Count1]]);
    break;
    case 1 :
        pot = encoder1.getCount();
        if (pot > valmaxi) {pot = valmini;}
        if (pot < valmini) {pot = valmaxi;}        
        Count = pot;
        encoder1.setCount(pot);
        enc1Last = pot;
        Screens(1, Params[id][Count]);
    break;
  }
}

void encod2(int valmini, int valmaxi, int pot, byte sel){
  switch (sel) {
    case 0 :
        pot = encoder2.getCount();
        if (pot > valmaxi) {pot = valmaxi;}
        if (pot < valmini) {pot = valmini;}
        midiblabla(toneXquick[0][Count2], pot, Canal1);
        Params[id][toneXquick[1][Count2]] = pot;
        encoder2.setCount(pot);
        enc2Last = pot;
        Screens(3, Params[id][toneXquick[1][Count2]]);
    break;
    case 1 :
        pot = encoder2.getCount();
        if (pot > valmaxi) {pot = valmaxi;}
        if (pot < valmini) {pot = valmini;}
        if (Count == 55) {
          progChange(toneValbk, pot, Canal1);
          progChang[id][0] = pot;
        }
        if (Count == 56) {
          progChange(ampValbk, pot, Canal2);
          progChang[id][1] = pot;
          }
        if (Count != 55 or Count != 56){
          Params[id][Count] = pot;
          if (Params[id][Count] == 139 or Params[id][Count] == 140){
            if (Params[id][Count] == 139){midiblabla(totalCC[0][Count], 0, Canal1);}
            else {midiblabla(totalCC[0][Count], 127, Canal1);}
          }
          if (Count > 17 && Count < Params[id][54]) {
            if (Params[id][Count] == 129){midiblabla(totalCC[0][Count], 0, Canal1);}
            if (Params[id][Count] == 130){midiblabla(totalCC[0][Count], 127, Canal1);}
            if (Params[id][Count] < 128){
              midiblabla(totalCC[0][Count], pot, Canal1);
            }
          }
          if (Count > 0 && Count < 18) {
            if (Params[id][Count] == 129){midiblabla(totalCC[0][Count], 0, Canal2);}
            if (Params[id][Count] == 130){midiblabla(totalCC[0][Count], 127, Canal2);}
            if (Params[id][Count] < 128){
              midiblabla(totalCC[0][Count], pot, Canal2);
            }
          }
        }        
        encoder2.setCount(pot);
        enc2Last = pot;
    break;
  }
}

void commun(){
  Screens(1, Params[id][Count]);
  static int tmp, encvalMax, encvalMini;
  while (Menus == 1) {
    initEncoder(2);
    if (Count == 55){initEncoder(4);}
    if (Count == 56){initEncoder(5);}
    if (Count != 55 && Count !=56){initEncoder(6);}
    if (enc1Last != encoder1.getCount()) {encod1(1,56,Count,1); } 
    if (enc2Last != encoder2.getCount()) {
      if (Count != 55 && Count != 56) {
        if (Params[id][Count] == 139 or Params[id][Count] == 140){
          if (Params[id][Count] == 140) {tmp = 140; encvalMax = 140; encvalMini = 139;}
          else {tmp = 139; encvalMax = 140; encvalMini = 139;}
        }
        if (Params[id][Count] == 129 or Params[id][Count] == 130){
          if (Params[id][Count] == 130) {tmp = 130; encvalMax = 130; encvalMini = 129;}
          else {tmp = 129; encvalMax = 130; encvalMini = 129;}
        }
        if (Params[id][Count] < 128){
          if (Params[id][Count] < 128) {tmp = -1; encvalMax = valmaxCC[Count]; encvalMini = 0; }
        }
      } 
      else {
        if (progChang[id][0] < 128 && Count == 55){toneValbk = 0;}
        if (progChang[id][1] < 128 && Count == 56){ampValbk =  0;}
        if (progChang[id][0] > 127 && progChang[id][0] < 151 && Count == 55){toneValbk = 1;}
        if (progChang[id][1] > 127 && progChang[id][1] < 257 && Count == 56){ampValbk = 1;}
        if (progChang[id][1] > 255 && progChang[id][1] < 301 && Count == 56){ampValbk = 2;}
        tmp = -1; encvalMax = valmaxCC[Count]; encvalMini = 0; 
      }
    encod2(encvalMini,encvalMax,Count,1);          
    Screens(1, tmp);
    }
    if (digitalRead(5) == 0) {
      delay(250);
      if (digitalRead(23) == 0) {
        Screens(10, 0);
        saveData(); delay(250);
        Count1 = 0; Count2 = 0; 
        if(Count<18) {startCharg = 2;} else{startCharg = 3;} break;
      }
      Count1 = 0; Count2 = 0; 
      if(Count<18) {startCharg = 2;} else{startCharg = 3;} break;
    }
    if (digitalRead(23) == 0) {
      delay(250);
      if (digitalRead(5) == 0) {
        Screens(10, 0);
        saveData(); delay(250);
        Count1 = 0; Count2 = 0; 
        if(Count<18) {startCharg = 2;} else{startCharg = 3;} break;
      }
      Count1 = 0; Count2 = 0; 
      if(Count<18) {startCharg = 2;} else{startCharg = 3;} break;
    }
  }
}

void BoutRot(byte choixrot, byte menu) {
  switch (choixrot) {
    case 1 :
      switch (menu) {
        case 0 :
          if (Couleur1 == 1) {//displayColor1(rgbcolor[Count1]);
            Couleur1 = 2;
          }
          else {
            if (startCharg == 1){Count1 = 0; startCharg = 0;}
            if (Count1 >= 0 &&  Count1 < 6) {
              Count1 = Count1+1;
            }
            else {Count1 = 1;}
            initEncoder(1);
            byte tmp;
            if (Params[id][Amperoquick[1][Count1]] > 135){
              if (Params[id][Amperoquick[1][Count1]] == 139) {tmp = 139;}
              else {tmp = 140;}
            }
            if (Params[id][Amperoquick[1][Count1]] > 128 && Params[id][Amperoquick[1][Count1]] < 135) {
              if (Params[id][Amperoquick[1][Count1]] == 130) {tmp = 130;}
              else {tmp = 129;}
            } 
            if (Params[id][Amperoquick[1][Count1]] < 128) {tmp = -1;}
            if (startScreen != 1) {Screens(2, tmp);} else {startScreen = 0;}
            //displayColor1(rgbcolor[Count1]);
          break;
          }
        case 1 : 
          // Screens(1, Params[id][Count]);
          Count = 1;
          commun();
        break;
      }
    break;
    case 2 :
      switch (menu) {
        case 0 : 
          if (Couleur2 == 1) {//displayColor2(rgbcolor[Count2]); 
          Couleur2 = 2;
          }
          else {
            if (startCharg == 1){Count2 = 0; startCharg = 0;}
            if (Count2 >= 0 &&  Count2 < 6) {
              Count2 = Count2+1;
            }
            else {Count2 = 1;}
            initEncoder(1);
            static byte tmp;
            if (Params[id][toneXquick[1][Count2]] > 135){
              if (Params[id][toneXquick[1][Count2]] == 139) {tmp = 139;}
              else {tmp = 140;}
            }
            if (Params[id][toneXquick[1][Count2]] > 128 && Params[id][toneXquick[1][Count2]] > 135) {
                if (Params[id][toneXquick[1][Count2]] == 130) {tmp = 130;}
                else {tmp = 129;}
              }
            if (Params[id][toneXquick[1][Count2]] > 128) {tmp = -1;}
            if (startScreen != 1) {Screens(3, tmp);} else {startScreen = 0;}
           // displayColor1(rgbcolor[Count2]);
          }
        break;
        case 1 :
          Count = 18;
          commun();
        break;
      }
    break;
  }
}

void choixprogchang(){
  if (progChang[id][0] < 128 or progChang[id][1] < 128){ampValbk = 0; toneValbk = 0;}
  if (progChang[id][0] > 127 && progChang[id][0] < 151){toneValbk = 1;}
  if (progChang[id][1] > 127 && progChang[id][1] < 257){ampValbk = 1;}
  if (progChang[id][1] > 255 && progChang[id][1] < 301){ampValbk = 2;}
}

void firstcharg(){
  progChange(toneValbk, progChang[id][0], Canal1);
  progChange(ampValbk,  progChang[id][1], Canal2);
  delay(10);
  for (i = 18; i < Params[id][54]; i++){
      MIDI.sendControlChange(totalCC[0][i], Params[id][i], Canal1);
  }
  for (i = 1; i < 18; i++){
      MIDI.sendControlChange(totalCC[0][i], Params[id][i], Canal2);
  }
}

void chgtPedal() {
  if (progChang[id][0] != progChangcopy[id][0]) {progChange(toneValbk, progChang[id][0], Canal1);}
  if (progChang[id][1] != progChangcopy[id][1]) {progChange(ampValbk,  progChang[id][1], Canal2);}
  delay(10);
  for (i = 18; i < Params[id][54]; i++){
    if (Params[id][i] != Params[preID][i]) {
      MIDI.sendControlChange(totalCC[0][i], Params[id][i], Canal1);
    }
  }
  for (i = 1; i < 18; i++){
    if (Params[id][i] != Params[preID][i]) {
      MIDI.sendControlChange(totalCC[0][i], Params[id][i], Canal2);
    }
  }
  //mcp.digitalWrite(ledS[id], HIGH);
  //mcp.digitalWrite(ledS[id], LOW);
}

void selectBank() {
  if (id < 43) {Bank = 1;}
  if (id > 42 && id < 85) {Bank = 2;}
  if (id > 84 && id < 127) {Bank = 3;}
  choixprogchang();
  if (startCharg == 1) {readData(); choixprogchang(); firstcharg(); oldBank = Bank;}
  if (Bank != oldBank) {readData(); oldBank = Bank;}
}

void handleControlChange(byte channel, byte number, byte value) {
  if (channel == 16) {
    preID = id; 
    id_Init = value; id = value;
    Count1 = 0; Count2 = 0; Menus = 0;
    selectBank(); chgtPedal();
    BoutRot(1, Menus);
    Screens(2, 0);
  }

  if (channel == 15) {
    preID = id; 
    id = id_Init + value;
    Count1 = 0; Count2 = 0; Menus = 0;
    selectBank(); chgtPedal();
    BoutRot(1, Menus);
    Screens(2, 0);
  }
}

void setup() {
  pinMode(33, OUTPUT); pinMode(32, OUTPUT); pinMode(25, OUTPUT);

  preID = -1; 
  
  LCD.init();
  LCD.backlight();
  
  Screens(0, 0);
  
  Serial.begin(115200);

  Wire.begin(22, 21);

  /*cp.begin_I2C();
  for (i = 1; i < 7; i++){mcp.pinMode(i, OUTPUT); mcp.digitalWrite(i, LOW);}
  for (i = 21; i < 27; i++){mcp.pinMode(i, OUTPUT); mcp.digitalWrite(i, LOW);}*/

  /*button1.setPressTicks(500);
  button2.setPressTicks(500);
  button3.setPressTicks(500);
  button4.setPressTicks(500);
  button5.setPressTicks(500);
  button6.setPressTicks(500);*/
  button7.setPressTicks(500);
  button8.setPressTicks(500);

  /*button1.attachClick([] () {if(Menus == 0) {id = id_Init;   chgtPedal();}});
  button2.attachClick([] () {if(Menus == 0) {preID = id; id = id_Init+1; chgtPedal();}});
  button3.attachClick([] () {if(Menus == 0) {preID = id; id = id_Init+2; chgtPedal();}});
  button4.attachClick([] () {if(Menus == 0) {preID = id; id = id_Init+3; chgtPedal();}});
  button5.attachClick([] () {if(Menus == 0) {preID = id; id = id_Init+4; chgtPedal();}});
  button6.attachClick([] () {if(Menus == 0) {preID = id; id = id_Init+5; chgtPedal();}});*/
  button7.attachClick([] () {if(Menus == 0) {BoutRot(1, Menus);}}); //if(Menus == 1) {BoutRot(1, Menus);}});
  button8.attachClick([] () {if(Menus == 0) {BoutRot(2, Menus);}}); //if(Menus == 1) {BoutRot(2, Menus);}});

 /*button1.attachLongPressStop([] () {});
  button2.attachLongPressStop([] () {});
  button3.attachLongPressStop([] () {});
  button4.attachLongPressStop([] () {});
  button5.attachLongPressStop([] () {});
  button6.attachLongPressStop([] () {});*/
  button7.attachLongPressStop([] () {texteLine2 = "toto"; Menus = 1; BoutRot(1, Menus);});
  button8.attachLongPressStop([] () {texteLine2 = "toto"; Menus = 1; BoutRot(2, Menus);});

  MIDI.begin(MIDI_CHANNEL_OMNI);  
  MIDI.setHandleControlChange(handleControlChange);

  if(SPIFFS.begin(true)){
    File root = SPIFFS.open("/"); id = 1, preID = 1; selectBank(); chgtPedal();}
  else {Serial.println("SPIFFS marche pas");} 

  ESP32Encoder::useInternalWeakPullResistors=NONE;
  encoder1.attachSingleEdge(15, 4);
  encoder2.attachSingleEdge(18, 19);
  encoder1.clearCount();
  encoder1.setFilter(1023);
  encoder2.clearCount();
  encoder2.setFilter(1023);
    
  //previousMillis = millis();

  initEncoder(1);

  texteLine2, texteLine1 = "toto";
  BoutRot(1, Menus);
  BoutRot(2, Menus);
  //id = 40;
  Screens(2, 0);
}
//----------------------------------------------------------------------------------END SETUP

void loop(){
  /*button1.tick();
  button2.tick();
  button3.tick();
  button4.tick();
  button5.tick();
  button6.tick();*/
  button7.tick();
  button8.tick();
  if (enc1Last != encoder1.getCount()) {
    if (Count1 == 0) {Count1 = 1;}
    encod1(encValmin1[Count1],encValmax1[Count1],Params[id][Amperoquick[1][Count1]],0);
    texteLine2, texteLine1 = "toto";
  }  
  if (enc2Last != encoder2.getCount()) {
    if (Count2 == 0) {Count2 = 1;}
    encod2(encValmin2[Count2],encValmax2[Count2],Params[id] [toneXquick[1][Count2]],0);
    texteLine2, texteLine1 = "toto";
  }
  MIDI.read();
  if (startCharg == 2) {startCharg = 0; Menus = 0; texteLine1 = "toto"; texteLine2 = "toto"; BoutRot(1, Menus); Screens(2, 0);}  
  if (startCharg == 3) {startCharg = 0; Menus = 0; texteLine1 = "toto"; texteLine2 = "toto"; BoutRot(2, Menus); Screens(3, 0);}
}

