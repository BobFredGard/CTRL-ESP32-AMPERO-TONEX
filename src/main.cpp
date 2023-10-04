#include <Arduino.h>

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
static int enc1last, enc2last;

struct Serial2MIDISettings
{
  static const long BaudRate = 31250;
  static const int8_t RxPin  = 16;
  static const int8_t TxPin  = 17;
};

#include <midi.h>
//MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial2, MIDI, Serial2MIDISettings);
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

static int i, oldid, j, k, l = 0;
static short choixbd, oldbank, menus = 0; 
static short id, id_init, count, startscreen, bank, couleur1, couleur2, count1, count2, startcharg, canal1, btn, cp1, cp2,idcopy = 1; static short canal2 = 2;
static short preid = -1;
static int ampvalbk, tonevalbk = 0;

//unsigned long previousMillis=0 ;
//unsigned long interval = 1200L;

//static byte MIDICanal[17] = {0, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187namequickTonecount2, 188, 189, 190, 191};
//static byte MIDIPC[17] = {0, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207};

static short totalcc [2][54] =  {
  //  |------CC------>>> AMPERO II STOMPS <<<-------CC-----------------| |------------CC------------>>> TONE X <<<--------------------------CC---------------------------------------------------------------------------------------------|
  {0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 13, 16, 18, 20, 7, 102, 23, 25, 28, 79, 19, 15, 106, 107, 103, 14, 16, 17, 18, 20, 21, 22, 24, 26, 27, 29, 30, 104, 75, 85, 76, 77, 78, 108, 109, 110, 111, 112, 113, 114, 115},
  {0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,  25,  26,  27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,  40, 41, 42, 43, 45, 45,  46,  47,  48,  49,  50,  51,  52,  53}
  //  Numéro colonne de la base de donnée
};

                                   //  |--------CC----------|      |--Positions dans BD--|
static short amperoquick [2] [7] = {{-10, 7, 16, 18, 20, 13, 59}, {0, 17, 14, 15, 16, 13, 12}};
static short tonexquick [2] [7] = {{-10, 103, 102, 19, 15, 106, 107}, {0, 27, 18, 23, 24, 25, 26}};

static const char* namequickamp [7]  {"", "Volume     : ", "Param 1    : ", "Param 2    : ", "Param 3    : ", "Exp        : ", "Reverb on  :"};
static const char*  namequicktone [7] {"", "MODEL.VOL  : ", "GAIN       : ", "COMPRESSOR : ", "GATE       : ", "PRESENCE   : ", "DEPHT      : "};
static short leds [7] = {0,21,22,23,24,25,26};
static short ledscolor [7] = {0,33,32,25,4,5,6};
static short encvalmax1 [7] = {0,100, 127, 127, 127, 130, 130};
static short encvalmin1 [7] = {0,0, 0, 0, 0, 129, 129};
static short encvalmax2 [7] = {0,127, 127, 127, 127, 127, 127};
static short encvalmin2 [7] = {0,0, 0, 0, 0, 0, 0};

static short params[43][60]; static short paramsCopy[43][60];  static short Copytemp[60];
static int progChang [43][2]; 

static const char* texteline1;
static const char* texteline2;
static String sql;

static const char* namescc[57] = {  "", 
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

static short valmaxcc [57] = {0,
  130,130,130,130,130,130,
  130,130,130,130,130,130,
  130,127,127,127,100,
  127,127,127,127,127,127,
  127,127,127,127,140,127,
  127,130,127,127,140,
  127,127,127,127,140,127,
  130,9,127,127,127,127,
  127,127,127,127,127,127,127,
  61,149,300
};

static int idcopyplus [19] = {0,1,8,15,22,29,36,43,50,57,64,71,78,85,92,99,106,113,120};

static const char* verbname[5] = {"SPRING1", "SPRING2", "SPRING3", "ROOM ", "PLATE"};

void progChange(byte CC, int val, byte canal) {
  MIDI.sendControlChange(0, CC, canal);
  MIDI.sendProgramChange(val, canal);
}

void midiblabla(byte a, byte b, byte c) {
      MIDI.sendControlChange(a, b, c);
      /*(""); Serial.print("ToneX -> CC : "); Serial.print(a); Serial.print(" - Val : ");
      Serial.print(b); Serial.print(" - Channel : "); Serial.println(c); */
}

void typereverb() {
  if (params[id][count] >= 0 && params[id][count] < 2) {midiblabla(totalcc[0][count], 10, canal1);}
  if (params[id][count] >= 2 && params[id][count] < 4) {midiblabla(totalcc[0][count], 20, canal1);}
  if (params[id][count] >= 4 && params[id][count] < 6) {midiblabla(totalcc[0][count], 55, canal1);}
  if (params[id][count] >= 6 && params[id][count] < 8) {midiblabla(totalcc[0][count], 85, canal1);}
  if (params[id][count] >= 8 && params[id][count] < 10) {midiblabla(totalcc[0][count], 115, canal1);}
  Serial.println(params[id][count]);
}

static float valminimax[2][57] = {
  {0,
    0,0,0,0,0,0,
    0,0,0,0,0,0,
    0,0,0,0,100,
    0,0,0,0,0,0,
    -100,0,0,0,140,20,
    -20,130,-30,1,140,
    75,0.2,150,1000,140,0,
    130,1,0,0,0,0,
    0,0,0,0,0,0,0,
    61,150,300
  },
  {0,
    0,0,0,0,0,0,
    0,0,0,0,0,0,
    0,0,0,0,100,
    10,10,10,10,100,-40,
    0,10,10,10,140,500,
    -100,130,10,51,140,
    600,3,5000,4000,140,100,
    0,5,127,127,127,127,
    127,127,127,127,127,127,127,
    61,150,300
  }
};

float affichevaltonex, affichevaltonexneg;

void echelledevaleur(byte val3){
  affichevaltonex = (valminimax[1][count]-valminimax[0][count])*params[id][count]/valmaxcc[count]+valminimax[0][count];
  switch (val3){
  case 1 :
    LCD.setCursor(12,1); LCD.print("    "); 
    if (affichevaltonex >=0 && affichevaltonex < 10) {LCD.setCursor(13,1);}
    if (affichevaltonex > 9.99 && affichevaltonex <= 100) {LCD.setCursor(12,1);}
    if (affichevaltonex < 0 && affichevaltonex < -100) {LCD.setCursor(13,1);}
    if (affichevaltonex < 0 && affichevaltonex >= -100) {LCD.setCursor(12,1);}
    LCD.print((valminimax[1][count]-valminimax[0][count])*params[id][count]/valmaxcc[count]+valminimax[0][count]);
  break;
  case 2 :
    LCD.setCursor(12,1); LCD.print("    "); 
    if (affichevaltonex >=0 && affichevaltonex < 10) {LCD.setCursor(14,1);}
    if (affichevaltonex >=0 && affichevaltonex > 9 && affichevaltonex < 100) {LCD.setCursor(13,1);}
    if (affichevaltonex >=0 && affichevaltonex >= 100) {LCD.setCursor(12,1);}
    LCD.print(round((valminimax[1][count]-valminimax[0][count])*params[id][count]/valmaxcc[count]+valminimax[0][count]));
    LCD.setCursor(15,1); LCD.print("%"); 
  break;
  case 3 :
    LCD.setCursor(9,1); LCD.print("       "); LCD.setCursor(9,1); 
    if (params[id][count] >= 0 && params[id][count] < 2) {LCD.print(verbname[0]);}
    if (params[id][count] >= 2 && params[id][count] < 4) {LCD.print(verbname[1]);}
    if (params[id][count] >= 4 && params[id][count] < 6) {LCD.print(verbname[2]);}
    if (params[id][count] >= 6 && params[id][count] < 8) {LCD.print(verbname[3]);}
    if (params[id][count] >= 8 && params[id][count] < 10) {LCD.print(verbname[4]);}
  break;
  }
}

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
      //String val2 = azColName[m];
      switch (choixbd){
        case 0 :
          //Serial.println("");Serial.print("m = ");Serial.println(m);
          if (m < 55) {params[i][m] = val.toInt(); paramsCopy [i][m] = val.toInt();}
          if (m == 55) {progChang[i][0] = val.toInt();}
          if (m == 56) {progChang[i][1] = val.toInt();}
          if (m > 56) {params[i][m] = val.toInt(); paramsCopy [i][m] = val.toInt();}
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
      if (params[idcopy][i] != paramsCopy[idcopy][i]) {
        sql += "'"+String(i)+"'="+String(params[idcopy][i])+", "; 
      }
    }
    if (i == 55){
        sql += "'"+String(i)+"'="+String(progChang[idcopy][0])+", "; 
    }
    if (i == 56){
        sql += "'"+String(i)+"'="+String(progChang[idcopy][1]); 
    }
  }
  if (bank == 1 ) {sql += " WHERE stomps_id="+String(idcopy)+";";}
  if (bank == 2 ) {sql += " WHERE stomps_id="+String(idcopy+42)+";";}
  if (bank == 3 ) {sql += " WHERE stomps_id="+String(idcopy+84)+";";}
  db_exec(db_base, sql.c_str());
  sqlite3_close(db_base);
  Serial.println(sql);
}

void saveDataCopy() {
  if (db_open("/spiffs/base.db", &db_base)) return;
  sql = "UPDATE stomps SET ";
  for (i = 1; i < 60; i++){
    if (i < 58){
      sql += "'"+String(i)+"'="+String(Copytemp[i])+", "; 
    }
    if (i == 58){
      sql += "'"+String(i)+"'="+String(Copytemp[i]); 
    }
  }
  sql += " WHERE stomps_id="+String(idcopy)+";";
  db_exec(db_base, sql.c_str());
  sqlite3_close(db_base);
  Serial.println(sql);
}

void saveDataPC() {
  if (db_open("/spiffs/base.db", &db_base)) return;
  sql = "UPDATE stomps SET ";
  sql += "'"+String(55)+"'="+String(progChang[id][0])+", "; progChang[idcopy][0] = progChang[id][0];
  sql += "'"+String(56)+"'="+String(progChang[id][1]);  progChang[idcopy][1] = progChang[id][1];
  sql += " WHERE stomps_id="+String(idcopy)+";";
  db_exec(db_base, sql.c_str());
  sqlite3_close(db_base);
  Serial.println(sql);
}

void readData (){
  choixbd = 0;
  if (db_open("/spiffs/base.db", &db_base)) return; 
  static byte o;
  Serial.print("Bank = "); Serial.println(bank);
  if (bank == 1 ) {o = id;}
  if (bank == 2 ) {o = id + 42;}
  if (bank == 3 ) {o = id + 84;}
  for (i = 1; i < 43; i++) {
    sql = "SELECT * FROM stomps WHERE stomps_id = " + String(o) + ";";
    db_exec(db_base, sql.c_str());
    o = o + 1;
    //Serial.println(sql);
  }
  sqlite3_close(db_base);
}
// ------------------------------------------FIN GESTTION BASE DE DONNEE

// ---------------------------------------------------------------ECRANS
void initEncoder(int val1) {
  switch (val1)  {
    case 1 :
      encoder1.setCount(params[id][amperoquick[1][count1]]);
      enc1last = params[id][amperoquick[1][count1]];
      encoder2.setCount(params[id][tonexquick[1][count2]]);
      enc2last = params[id][tonexquick[1][count2]];
    break;
    case 2 :
      encoder1.setCount(count);
      enc1last = count;
    break;
    case 3 :
      encoder1.setCount(params[id][count]);
      enc1last = params[id][count];
      encoder2.setCount(params[id][count]);
      enc2last = params[id][count];
    break;
    case 4 :
      encoder2.setCount(progChang[id][0]); enc2last = progChang[id][0];
    break;
    case 5 :
      encoder2.setCount(progChang[id][1]); enc2last = progChang[id][1];
    break;
    case 6 :
      encoder2.setCount(params[id][count]); enc2last = params[id][count];
    break;
    case 7 :
    cp1 = 1;
    cp2 = 2;
      encoder1.setCount(cp1);
      enc1last = cp1;
      encoder2.setCount(cp2);
      enc2last = cp2;
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
      if (count > 0 && count < 18){
        LCD.setCursor(0,0);
        if (texteline1 != "P:    *AMPERO II") {texteline1 = "P:    *AMPERO II"; LCD.print(texteline1);}
        texte = String(id) + " ";
        LCD.setCursor(2,0); LCD.print(texte);
      }
      if (count > 17 && count < 54){
        LCD.setCursor(0,0);        
        if (texteline1 != "P:       *TONE_X") {texteline1 = ("P:       *TONE_X"); LCD.print(texteline1);}
        LCD.setCursor(2,0); LCD.print(id);
      }
      if (count == 54){if (texteline1 != " Fin Sauvegd") {LCD.setCursor(5,0); texteline1 = " Fin Sauvegd"; LCD.print(texteline1);}}
      if (count == 55 or count == 56){if (texteline1 != " Prog Change") {LCD.setCursor(5,0); texteline1 = " Prog Change"; LCD.print(texteline1);}}
      LCD.setCursor(0,1);
      if (texteline2 != namescc[count]) {texteline2 = namescc[count]; LCD.print(texteline2);}
      LCD.setCursor(13,1); LCD.print("   ");
      LCD.setCursor(13,1); 
      if (count != 55 && count != 56) {
        if (val2 == 139 or val2 == 140) {
          if (val2 == 139) {LCD.setCursor(10,1); LCD.print(":  PRE"); val2 = 139;}
          else {LCD.setCursor(10,1); LCD.print(": POST"); val2 = 140;}
        }
        if (val2 == 130 or val2 == 129) {
            if (params[id][count] == 130) {LCD.print("ON "); val2 = 130;}
            else {LCD.print("OFF"); val2 = 129;}
        }
        if (params[id][count] < 128) {          
          if (count < 18) {LCD.print(params[id][count]);}
          if (count > 17) {
            if (valminimax[1][count] == 10) {echelledevaleur(1);}
            if (count == 22 or count == 40 or count == 53) {echelledevaleur(2);}
            if (count != 22 or count != 40 or count != 53 or valminimax[1][count] != 10 or count != 42) {LCD.print(params[id][count]);}
            if (count == 42) {echelledevaleur(3);}
          }
          val2 = -1;
        } 
      }
      if(count == 55) {LCD.print(progChang[id][0]); val2 = -1;}
      if(count == 56) {LCD.print(progChang[id][1]); val2 = -1;} 
    break;
    case 2 :
        LCD.setCursor(0,0);
        if (texteline1 != "P:     AMPERO II") {texteline1 = "P:     AMPERO II"; LCD.print(texteline1);}
        texte = String(id) + " ";
        LCD.setCursor(2,0); LCD.print(texte);
        LCD.setCursor(0,1);
        if (texteline2 != namequickamp[count1]) {texteline2 = namequickamp[count1]; LCD.print(texteline2);}
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
        if (params[id][amperoquick[1][count1]] < 128) {LCD.print(params[id][amperoquick[1][count1]]); val2 = -1;}
    break;
    case 3 :
        LCD.setCursor(0,0);
        if (texteline1 != "P:        TONE_X") {texteline1 = "P:        TONE_X"; LCD.print(texteline1);}
        //LCD.setCursor(0,0); LCD.print("P:");
        texte = String(id) + " ";
        LCD.setCursor(2,0); LCD.print(texte);
        LCD.setCursor(0,1);
        if (texteline2 != namequicktone[count2]) {texteline2 = namequicktone[count2]; LCD.print(texteline2);}
        LCD.setCursor(13,1); LCD.print("   ");
        LCD.setCursor(13,1);
        if (val2 == 139 or val2 == 140) {if (val2 == 139) {LCD.setCursor(10,1); LCD.print(":  PRE"); val2 = 139;}
          else {LCD.setCursor(10,1); LCD.print(": POST"); val2 = 140;}}
        if (val2 == 129 or val2 == 130) {
          if (val2 == 130) {LCD.print("ON "); val2 = 130;}
          else {LCD.print("OFF"); val2 = 129;}
        }
        if (params[id][tonexquick[1][count2]] < 128) {
          count = tonexquick[1][count2];
          //LCD.print(params[id][count]);
          if (valminimax[1][count] == 10) {echelledevaleur(1);}
          if (count == 22 or count == 40 or count == 53) {echelledevaleur(2);}
          if (count != 22 or count != 40 or count != 53 or valminimax[1][count] != 10 or count != 42) {LCD.print(params[id][count]);}
          if (count == 42) {echelledevaleur(3);}
          val2 = -1;
        }
    break;
    case 6 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print(" Copy Pacth : ");
      LCD.setCursor(0,1);
      LCD.print("...To Patch : ");
    break;
    case 7 :
      LCD.setCursor(14,0);
      LCD.print(cp1);
      if (cp1 < 10) {LCD.setCursor(15,0); LCD.print(" ");}
    break;
    case 8 :
      LCD.setCursor(14,1);
      LCD.print(cp2);
      if (cp2 < 10) {LCD.setCursor(15,1); LCD.print(" ");}
    break;
    case 9 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("** CHARGEMENT **");
      LCD.setCursor(0,1);
      LCD.print("***** BANK *****");
    break;
    case 10 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("** SAUVEGARDE **");
      LCD.setCursor(0,1);
      LCD.print("*** EN COURS ***");
    break;
    case 11 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("****  COPY  ****");
      LCD.setCursor(0,1);
      LCD.print("*** EN COURS ***");
    break;
    case 12 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("????  COPY  ????");
      LCD.setCursor(0,1);
      LCD.print("Progs AMP/TONE-X");
    break;
    case 13 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("****  COPY  ****");
      LCD.setCursor(0,1);
      LCD.print("Progs AMP/TONE-X");
    break;
  }
  oldid = id;
}

void displayColor1(byte color) {
  digitalWrite(ledscolor[0], bitRead(color, 2));
  digitalWrite(ledscolor[1], bitRead(color, 1));
  digitalWrite(ledscolor[2], bitRead(color, 0));
}

void displayColor2(byte color) {
  digitalWrite(ledscolor[5], bitRead(color, 2));
  digitalWrite(ledscolor[4], bitRead(color, 1));
  digitalWrite(ledscolor[5], bitRead(color, 0));
}

void encod1(byte valmini, byte valmaxi, byte pot, byte sel){
  switch (sel) {
    case 0 :
      pot = encoder1.getCount();
      if (pot > valmaxi) {pot = valmaxi;}
      if (pot < valmini) {pot = valmini;}
      midiblabla(amperoquick[0][count1], pot, canal2);
      params[id][amperoquick[1][count1]] = pot;
      encoder1.setCount(pot);
      enc1last = pot;
      Screens(2, params[id][amperoquick[1][count1]]);
    break;
    case 1 :
      pot = encoder1.getCount();
      if (pot > valmaxi) {pot = valmini;}
      if (pot < valmini) {pot = valmaxi;}        
      count = pot;
      encoder1.setCount(pot);
      enc1last = pot;
      Screens(1, params[id][count]);
    break;
    case 2 :
      pot = encoder1.getCount();
      if (pot > valmaxi) {pot = valmini;}
      if (pot < valmini) {pot = valmaxi;}        
      cp1 = pot;
      encoder1.setCount(pot);
      enc1last = pot;
      Screens(7, cp1);
    break;
  }
}

void encod2(int valmini, int valmaxi, int pot, byte sel){
  switch (sel) {
    case 0 :
      pot = encoder2.getCount();
      if (pot > valmaxi) {pot = valmaxi;}
      if (pot < valmini) {pot = valmini;}
      midiblabla(tonexquick[0][count2], pot, canal1);
      Screens(3, pot);
      params[id][tonexquick[1][count2]] = pot;
      encoder2.setCount(pot);
      enc2last = pot;
    break;
    case 1 :
      pot = encoder2.getCount();
      if (pot > valmaxi) {pot = valmaxi;}
      if (pot < valmini) {pot = valmini;}
      if (count == 55) {
        progChange(tonevalbk, pot, canal1);
        progChang[id][0] = pot;
      }
      if (count == 56) {
        progChange(ampvalbk, pot, canal2);
        progChang[id][1] = pot;
        }
      if (count != 55 or count != 56){
        params[id][count] = pot;
        if (params[id][count] == 139 or params[id][count] == 140){
          if (params[id][count] == 139){midiblabla(totalcc[0][count], 0, canal1);}
          else {midiblabla(totalcc[0][count], 127, canal1);}
        }
        if (count > 17 && count < params[id][54]) {
          if (params[id][count] == 129){midiblabla(totalcc[0][count], 0, canal1);}
          if (params[id][count] == 130){midiblabla(totalcc[0][count], 127, canal1);}
          if (params[id][count] < 128){
            midiblabla(totalcc[0][count], pot, canal1);
          }
        }
        if (count > 0 && count < 18) {
          if (params[id][count] == 129){midiblabla(totalcc[0][count], 0, canal2);}
          if (params[id][count] == 130){midiblabla(totalcc[0][count], 127, canal2);}
          if (params[id][count] < 128){
            midiblabla(totalcc[0][count], pot, canal2);
          }
        }
      }        
      encoder2.setCount(pot);
      enc2last = pot;
    break;
    case 2 :
      pot = encoder2.getCount();
      if (pot > valmaxi) {pot = valmaxi;}
      if (pot < valmini) {pot = valmini;}
      params[id][42] = pot;
      encoder2.setCount(pot);
      enc2last = pot;
      typereverb();
    break;
    case 3 :
      pot = encoder2.getCount();
      if (pot > valmaxi) {pot = valmini;}
      if (pot < valmini) {pot = valmaxi;}        
      cp2 = pot;
      encoder2.setCount(pot);
      enc2last = pot;
      Screens(8, cp2);
    break;
  }
}

void commun(){
  Screens(1, params[id][count]);
  static int tmp, encvalMax, encvalMini;
  while (menus == 1) {
    initEncoder(2);
    if (count == 55){initEncoder(4);}
    if (count == 56){initEncoder(5);}
    if (count != 55 && count !=56){initEncoder(6);}
    if (enc1last != encoder1.getCount()) {encod1(1,56,count,1); } 
    if (enc2last != encoder2.getCount()) {
      if (count != 55 && count != 56) {
        if (params[id][count] == 139 or params[id][count] == 140){
          if (params[id][count] == 140) {tmp = 140; encvalMax = 140; encvalMini = 139;}
          else {tmp = 139; encvalMax = 140; encvalMini = 139;}
        }
        if (params[id][count] == 129 or params[id][count] == 130){
          if (params[id][count] == 130) {tmp = 130; encvalMax = 130; encvalMini = 129;}
          else {tmp = 129; encvalMax = 130; encvalMini = 129;}
        }
        if (params[id][count] < 128){
          if (params[id][count] < 128) {tmp = -1; encvalMax = valmaxcc[count]; encvalMini = 0; }
        }
      } 
      else {
        if (progChang[id][0] < 128 && count == 55){tonevalbk = 0;}
        if (progChang[id][1] < 128 && count == 56){ampvalbk =  0;}
        if (progChang[id][0] > 127 && progChang[id][0] < 151 && count == 55){tonevalbk = 1;}
        if (progChang[id][1] > 127 && progChang[id][1] < 257 && count == 56){ampvalbk = 1;}
        if (progChang[id][1] > 255 && progChang[id][1] < 301 && count == 56){ampvalbk = 2;}
        tmp = -1; encvalMax = valmaxcc[count]; encvalMini = 0; 
      }
    if (count != 42){encod2(encvalMini,encvalMax,count,1);}
    if (count == 42){encod2(0,valmaxcc[count],count,2);}    
    Screens(1, tmp);
    }
    if (digitalRead(5) == 0) {
      delay(250);
      if (digitalRead(23) == 0) {
        Screens(10, 0);
        idcopy = id;
        saveData(); delay(250);
        count1 = 0; count2 = 0; 
        if(count<18) {startcharg = 2;} else{startcharg = 3;} break;
      }
      count1 = 0; count2 = 0; 
      if(count<18) {startcharg = 2;} else{startcharg = 3;} break;
    }
    if (digitalRead(23) == 0) {
      delay(250);
      if (digitalRead(5) == 0) {
        Screens(10, 0);
        idcopy = id;
        saveData(); delay(250);
        count1 = 0; count2 = 0; 
        if(count<18) {startcharg = 2;} else{startcharg = 3;} break;
      }
      count1 = 0; count2 = 0; 
      if(count<18) {startcharg = 2;} else{startcharg = 3;} break;
    }
  }
}

void BoutRot(byte choixrot, byte menu) {
  switch (choixrot) {
    case 1 :
      switch (menu) {
        case 0 :
          if (couleur1 == 1) {//displayColor1(rgbcolor[count1]);
            couleur1 = 2;
          }
          else {
            if (startcharg == 1){count1 = 0; startcharg = 0;}
            if (count1 > 0 && count1 < 6) {
              count1 = count1+1;
            }
            else {count1 = 1; count2 = 1;}
            initEncoder(1);
            byte tmp;
            if (params[id][amperoquick[1][count1]] > 135){
              if (params[id][amperoquick[1][count1]] == 139) {tmp = 139;}
              else {tmp = 140;}
            }
            if (params[id][amperoquick[1][count1]] > 128 && params[id][amperoquick[1][count1]] < 135) {
              if (params[id][amperoquick[1][count1]] == 130) {tmp = 130;}
              else {tmp = 129;}
            } 
            if (params[id][amperoquick[1][count1]] < 128) {tmp = -1;}
            if (startscreen != 1) {Screens(2, tmp);} else {startscreen = 0;}
            //displayColor1(rgbcolor[count1]);
          break;
          }
        case 1 : 
          // Screens(1, params[id][count]);
          count = 1;
          commun();
        break;
      }
    break;
    case 2 :
      switch (menu) {
        case 0 : 
          if (couleur2 == 1) {//displayColor2(rgbcolor[count2]); 
          couleur2 = 2;
          }
          else {
            if (startcharg == 1){count2 = 0; startcharg = 0;}
            if (count2 > 0 && count2 < 6) {
              count2 = count2+1;
            }
            else {count2 = 1; count1 = 1;}
            initEncoder(1);
            static byte tmp;
            if (params[id][tonexquick[1][count2]] > 135){
              if (params[id][tonexquick[1][count2]] == 139) {tmp = 139;}
              else {tmp = 140;}
            }
            if (params[id][tonexquick[1][count2]] > 128 && params[id][tonexquick[1][count2]] > 135) {
                if (params[id][tonexquick[1][count2]] == 130) {tmp = 130;}
                else {tmp = 129;}
              }
            if (params[id][tonexquick[1][count2]] > 128) {tmp = -1;}
            if (startscreen != 1) {Screens(3, tmp);} else {startscreen = 0;}
           // displayColor1(rgbcolor[count2]);
          }
        break;
        case 1 :
          count = 18;
          commun();
        break;
      }
    break;
  }
}

void SavePatch(){
  Screens(11,0);
  //int j, k, l;
  l = idcopyplus[cp1];
  k = idcopyplus[cp2];
  for (k ; k < idcopyplus[cp2]+7; k++ && l++) {
    for (j = 1; j < 60; j++){
      if (j != 55 or j != 56){
        if (k < id_init + 43)  {
          params[k][j] = params[l][j];
          paramsCopy[k][j] = paramsCopy[l][j];
          Copytemp[j] = params[l][j];
        }
        if (k > id_init + 42) {Copytemp[j] = params[l][j];}
      }
      if (j == 55){Copytemp[j] = progChang[l][0];}
      if (j == 56){Copytemp[j] = progChang[l][1];}
    }
    Serial.println("");Serial.print(" ID = ");Serial.println(l);
    idcopy = k;
    saveDataCopy();
    delay(50);
  }
}

void CopyPatch(){
  byte tmp = 1;
  initEncoder(7);
  while (menus == 2){
    if (tmp == 1) {Screens(6, 0); Screens(7,0); Screens(8,0); tmp = 0;}
    if (enc1last != encoder1.getCount()) {encod1(1,18,cp1,2);} 
    if (enc2last != encoder2.getCount()) {encod2(1,18,cp2,3);} 
    if (digitalRead(5) == 0) {
      delay(250);
      if (digitalRead(23) == 0) {SavePatch();}
      count1 = 0; count2 = 0;
      startcharg = 2;
      break;
    }
    if (digitalRead(23) == 0) {
      delay(250);
      if (digitalRead(5) == 0) {SavePatch();}
      count1 = 0; count2 = 0; 
      startcharg = 3;
      break;
    }
  }
}

void CopyPC(){
  byte tmp = 1;
  while (menus == 2){
    if (tmp == 1) {Screens(12, 0);tmp = 0;}
    if (digitalRead(5) == 0) {
      delay(250);
      if (digitalRead(23) == 0) {Screens(13, 0); for (j = id+1; j < id+7; j++) {idcopy = j; saveDataPC();}}
      count1 = 0; count2 = 0;
      startcharg = 2;
      break;
    }
    if (digitalRead(23) == 0) {
      delay(250);
      if (digitalRead(5) == 0) {Screens(13, 0); for (j = id+1; j < id+7; j++) {idcopy = j; saveDataPC();}}
      count1 = 0; count2 = 0; 
      startcharg = 3;
      break;
    }
  }
}

void choixprogchang(){
  if (progChang[id][0] < 128 or progChang[id][1] < 128){ampvalbk = 0; tonevalbk = 0;}
  if (progChang[id][0] > 127 && progChang[id][0] < 151){tonevalbk = 1;}
  if (progChang[id][1] > 127 && progChang[id][1] < 257){ampvalbk = 1;}
  if (progChang[id][1] > 255 && progChang[id][1] < 301){ampvalbk = 2;}
}

void firstcharg(){
  progChange(tonevalbk, progChang[id][0], canal1);
  delay(100);
  progChange(ampvalbk,  progChang[id][1], canal2);
  delay(100);
  for (i = 18; i < params[id][54]; i++){
    if (params[id][i] == 139) {MIDI.sendControlChange(totalcc[0][i], 0, canal1);}
    if (params[id][i] == 140) {MIDI.sendControlChange(totalcc[0][i], 127, canal1);}
    if (params[id][i] == 129) {MIDI.sendControlChange(totalcc[0][i], 0, canal1);}
    if (params[id][i] == 130) {MIDI.sendControlChange(totalcc[0][i], 127, canal1);}
    if (i == 42) {typereverb();}
    else {if (params[id][i] < 118) {MIDI.sendControlChange(totalcc[0][i], params[id][i], canal1);}}
    delay(5);
  }
  for (i = 1; i < 18; i++){
    if (i < 14) {
      if (params[id][i] == 129) {MIDI.sendControlChange(totalcc[0][i], 0, canal2);}
      else {MIDI.sendControlChange(totalcc[0][i], 127, canal2);}
    if (i > 13) {MIDI.sendControlChange(totalcc[0][i], params[id][i], canal2);}
    delay(5);
    }
  }
}

void chgtPedal() {
  if (progChang[id][0] != progChang[preid][0]) {progChange(tonevalbk, progChang[id][0], canal1);}
  delay(20);
  if (progChang[id][1] != progChang[preid][1]) {progChange(ampvalbk,  progChang[id][1], canal2);}
  delay(20);
  for (i = 18; i < params[id][54]; i++){
    if (params[id][i] != params[preid][i]) {
      if (i != 42) {
        if (params[id][i] == 139) {MIDI.sendControlChange(totalcc[0][i], 0, canal1);}
        if (params[id][i] == 140) {MIDI.sendControlChange(totalcc[0][i], 127, canal1);}
        if (params[id][i] == 129) {MIDI.sendControlChange(totalcc[0][i], 0, canal1);}
        if (params[id][i] == 130) {MIDI.sendControlChange(totalcc[0][i], 127, canal1);}
        if (params[id][i] < 118) {
          MIDI.sendControlChange(totalcc[0][i], params[id][i], canal1);
        }
      }
      else {typereverb();}
  delay(10);
    }
  }
  for (i = 1; i < 18; i++){
    if (params[id][i] != params[preid][i]) {
      if (params[id][i] == 129) {MIDI.sendControlChange(totalcc[0][i], 0, canal2);}
      if (params[id][i] == 130) {MIDI.sendControlChange(totalcc[0][i], 127, canal2);}
      if (count1 > 13) {MIDI.sendControlChange(totalcc[0][i], params[id][i], canal2);}
    }
  }
}

void selectBank() {
  choixprogchang();
  if (startcharg == 1) {readData(); choixprogchang(); firstcharg(); oldbank = bank;}
  if (bank != oldbank) {Screens(9, 0); readData(); oldbank = bank; texteline1 = "toto"; texteline2 = "toto"; Screens(2, 0);}
}

void handleControlChange(byte channel, byte number, byte value) {
  if (channel == 12){bank = value;}
  Serial.print("Bank = ");Serial.println(bank);
  if (channel == 16) {
    preid = id; 
    id_init = value; id = value;
    count1 = 0; count2 = 0; menus = 0;
    selectBank(); 
    chgtPedal();
    BoutRot(1, menus);
    Screens(2, 0);
  }

  if (channel == 15) {
    preid = id; 
    id = id_init + value;
    count1 = 0; count2 = 0; menus = 0;
    //selectBank(); 
    chgtPedal();
    BoutRot(1, menus);
    Screens(2, 0);
  }
}

void setup() {
  pinMode(33, OUTPUT); pinMode(32, OUTPUT); pinMode(25, OUTPUT);

  preid = -1; 
  
  LCD.init();
  LCD.backlight();
  
  Screens(0, 0);
  
  Serial.begin(115200);
  //Serial2.begin(9600);

  Wire.begin(22, 21);

  button7.setPressTicks(500);
  button8.setPressTicks(500);

  button7.attachClick([] () {if(menus == 0) {btn = 1; BoutRot(1, menus);}});
  button8.attachClick([] () {if(menus == 0) {btn = 2; BoutRot(2, menus);}});

  button7.attachLongPressStop([] () {texteline2 = "toto"; menus, btn = 1; BoutRot(1, menus);});
  button8.attachLongPressStop([] () {texteline2 = "toto"; menus = 1; btn = 2; BoutRot(2, menus);});

  button7.attachDoubleClick([] () {texteline2, texteline1 = "toto"; menus = 2; CopyPC();});
  button8.attachDoubleClick([] () {texteline2, texteline1 = "toto"; menus = 2; CopyPatch();});


  MIDI.begin(MIDI_CHANNEL_OMNI);  
  MIDI.setHandleControlChange(handleControlChange);

  if(SPIFFS.begin(true)){
    File root = SPIFFS.open("/"); bank = 1; id = 1, startcharg = 1; selectBank(); preid = 1;}
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

  texteline2, texteline1 = "toto";
  BoutRot(1, menus);
  BoutRot(2, menus);
  Screens(2, 0);
}
//----------------------------------------------------------------------------------END SETUP

void loop(){
  button7.tick();
  button8.tick();
  if (enc1last != encoder1.getCount()) {
    if (count1 == 0) {count1 = 1;}
    encod1(encvalmin1[count1],encvalmax1[count1],params[id][amperoquick[1][count1]],0);
    texteline2, texteline1 = "toto";
  }  
  if (enc2last != encoder2.getCount()) {
    if (count2 == 0) {count2 = 1;}
    if (count2 != 0) {encod2(encvalmin2[count2],encvalmax2[count2],params[id][tonexquick[1][count2]],0);}
    texteline2, texteline1 = "toto";
  }
  MIDI.read();
  if (startcharg == 2) {startcharg = 0; menus = 0; texteline1, texteline2 = "toto"; BoutRot(1, menus); Screens(2, 0);}  
  if (startcharg == 3) {startcharg = 0; menus = 0; texteline1, texteline2 = "toto"; BoutRot(2, menus); Screens(3, 0);}
}

