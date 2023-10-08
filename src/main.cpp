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

static int i, j, k, l, m, tmp, pied, tmp2 = 0;
static int choixbd, oldbank, menus, stomp = 0; 
static int id, id_init, count, startscreen, bank, couleur1, couleur2, count1, count2, startcharg, btn, cp1, cp2,idcopy, cpt = 1; static short canal2 = 7; static short canal1 = 8;
static int preid = -1;
static int ampvalbk, tonevalbk = 0;

unsigned long previousMillis = 0;
unsigned long interval = 3000;

static short totalcc [54] =  {
  //  |------CC------>>> AMPERO II STOMPS <<<-------CC-----------------| |------------CC------------>>> TONE X <<<--------------------------CC---------------------------------------------------------------------------------------------|
   0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 13, 16, 18, 20, 7, 102, 23, 25, 28, 79, 19, 15, 106, 107, 103, 14, 16, 17, 18, 20, 21, 22, 24, 26, 27, 29, 30, 104, 75, 85, 76, 77, 78, 108, 109, 110, 111, 112, 113, 114, 115};
//{0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,  25,  26,  27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,  40, 41, 42, 43, 45, 45,  46,  47,  48,  49,  50,  51,  52,  53}
  //  Numéro colonne de la base de donnée


                                   //  |--------CC----------|      |--Positions dans BD--|
static short amperoquick [2] [7] = {{-10, 7, 16, 18, 20, 13, 59}, {0, 17, 14, 15, 16, 13, 12}};
static short tonexquick [2] [7] = {{-10, 103, 102, 19, 15, 106, 107}, {0, 27, 18, 23, 24, 25, 26}};

static const char* namequickamp [7]  {"", "VOLUME     : ", "PARAM 1     : ", "PARAM 2    : ", "PARAM 3    : ", "PEDAL EXP  : ", "REVERB ON  : "};
static const char*  namequicktone [7] {"", "MODEL.VOL  : ", "GAIN       : ", "COMPRESSOR : ", "GATE       : ", "PRESENCE   : ", "DEPHT      : "};
static short leds [7] = {0,21,22,23,24,25,26};
static short ledscolor [7] = {0,33,32,25,4,5,6};
static int encvalmax1 [7] = {0,100, 127, 127, 127, 1300, 1300};
static int encvalmin1 [7] = {0,0, 0, 0, 0, 1299, 1299};
static int encvalmax2 [7] = {0,127, 127, 127, 127, 127, 127};
static int encvalmin2 [7] = {0,0, 0, 0, 0, 0, 0};

static int params[43][60]; static int paramsCopy[43][60];  static int Copytemp[60];

static const char* texteline1;
static const char* texteline2;
static String sql;

static const char* namescc[57] = {  "", 
  "SLOT A1    : ", "SLOT A2    : ", "SLOT A3    : ", "SLOT A4    : ", "SLOT A5    : ", "SLOT A6    : ",
  "SLOT B1    : ", "SLOT B2    : ", "SLOT B3    : ", "SLOT B4    : ", "SLOT B5    : ", "SLOT B6    : ",
  "PEDAL EXP  : ", "PARAM 1    : ", "PARAM 2    : ", "PARAM 3    : ", "VOLUME     : ",
  "GAIN       : ", "BASS       : ", "MID        : ", "TREBLE     : ", "REVERB     : ", "COMPRESSOR : ",
  "NOISE GATE : ", "PRESENCE   : ", "DEPTH      : ", "MODEL.VOL  : ", "NOISE GT   : ", "NG REL     : ",
  "NG DPTH    : ", "COMP       : ", "CMP GAIN   : ", "COMP ATK   : ", "COMP PATC  : ",
  "BASS FRQ   : ", "MID Q      : ", "MID FRQ    : ", "TRBL FRQ   : ", "EQ PATCH   : ", "EQ MIX     : ",
  "REVERB     : ", "REV TYPE   : ", "REV TIME   : ", "VB PREDELAY: ", "REV COLOR  : ", "REV RESO   : ",
  "MIC 1      : ", "MIC 1 X    : ", "MIC 1 Z    : ", "MIC 2      : ", "MIC 2 X    : ", "MIC 2 Z    : ", "MICS MIX   : ",
  "NB FX MAX  : ", "PC Tone X  : ", "PC Ampero  : "
};

static int valmaxcc [57] = {0,
  1300,1300,1300,1300,1300,1300,
  1300,1300,1300,1300,1300,1300,
  1300,127,127,127,100,
  127,127,127,127,127,127,
  127,127,127,127,1400,127,
  127,1300,127,127,1400,
  127,127,127,127,1400,127,
  1300,9,127,127,127,127,
  127,127,127,127,127,127,127,
  61,149,300
};

static int idcopyplus [37] = {0,1,8,15,22,29,36,43,50,57,64,71,78,85,92,99,106,113,120,127,134,141,148,155,162,169,176,183,190,197,204,211,218,225,232,239,246};

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
  if (params[id][42] >= 0 && params[id][42] < 2) {midiblabla(totalcc[42], 10, canal1);}
  if (params[id][42] >= 2 && params[id][42] < 4) {midiblabla(totalcc[42], 20, canal1);}
  if (params[id][42] >= 4 && params[id][42] < 6) {midiblabla(totalcc[42], 55, canal1);}
  if (params[id][42] >= 6 && params[id][42] < 8) {midiblabla(totalcc[42], 85, canal1);}
  if (params[id][42] >= 8 && params[id][42] < 10) {midiblabla(totalcc[42], 115, canal1);}
  //Serial.println(42);
}

static float valminimax[2][57] = {
  {0,
    0,0,0,0,0,0,
    0,0,0,0,0,0,
    0,0,0,0,100,
    0,0,0,0,0,0,
    -100,0,0,0,1400,20,
    -20,1300,-30,1,1400,
    75,0.2,150,1000,1400,0,
    1300,1,0,0,0,0,
    0,0,0,0,0,0,0,
    61,150,300
  },
  {0,
    0,0,0,0,0,0,
    0,0,0,0,0,0,
    0,0,0,0,100,
    10,10,10,10,100,-40,
    0,10,10,10,1400,500,
    -100,1300,10,51,1400,
    600,3,5000,4000,1400,100,
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
        if (texteline1 != "P  B  *AMPERO II") 
           {texteline1  = "P  B  *AMPERO II"; 
          LCD.print(texteline1);
          LCD.setCursor(4,0); LCD.print(bank);
        }
        texte = String(pied) + " ";LCD.setCursor(1,0); LCD.print(texte);
      }
      if (count > 17 && count < 54){
        LCD.setCursor(0,0);        
        if (texteline1 != "P  B     *TONE_X") {
           texteline1  = ("P  B     *TONE_X"); 
          LCD.print(texteline1);
          LCD.setCursor(4,0); LCD.print(bank);
        }
        texte = String(pied) + " ";LCD.setCursor(1,0); LCD.print(texte);
      }
      if (count == 54){if (texteline1 != "*Fin Sauvegd") {LCD.setCursor(5,0); texteline1 = "*Fin Sauvegd"; LCD.print(texteline1);}}
      if (count == 55 or count == 56){if (texteline1 != "*Prog Change") {LCD.setCursor(5,0); texteline1 = "*Prog Change"; LCD.print(texteline1);}}
      LCD.setCursor(0,1);
      if (texteline2 != namescc[count]) {texteline2 = namescc[count]; LCD.print(texteline2);}
      LCD.setCursor(13,1); LCD.print("   ");
      LCD.setCursor(13,1); 
      if (count != 55 && count != 56) {
        if (val2 == 1399 or val2 == 1400) {
          if (val2 == 1399) {LCD.setCursor(10,1); LCD.print(":  PRE"); val2 = 1399;}
          else {LCD.setCursor(10,1); LCD.print(": POST"); val2 = 1400;}
        }
        if (val2 == 1300 or val2 == 1299) {
            if (params[id][count] == 1300) {LCD.print("ON "); val2 = 1300;}
            else {LCD.print("OFF"); val2 = 1299;}
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
      if(count == 55) {LCD.print(params[id][55]); val2 = -1;}
      if(count == 56) {LCD.print(params[id][56]); val2 = -1;} 
    break;
    case 2 :
        LCD.setCursor(0,0);
        if (texteline1 != "P  B   AMPERO II") {
            texteline1  = "P  B   AMPERO II"; 
          LCD.print(texteline1);
          LCD.setCursor(4,0); LCD.print(bank);
        }
        texte = String(pied) + " ";LCD.setCursor(1,0); LCD.print(texte);
        LCD.setCursor(0,1);
        if (texteline2 != namequickamp[count1]) {texteline2 = namequickamp[count1]; LCD.print(texteline2);}
        LCD.setCursor(13,1); LCD.print("   ");
        LCD.setCursor(13,1);
        if (val2 == 1399 or val2 == 1400) {
          if (val2 == 1399) {LCD.setCursor(10,1); LCD.print(":  PRE"); val2 = 1399;}
          else {LCD.setCursor(10,1); LCD.print(": POST"); val2 = 1400;}
        }
        if (val2 == 1300 or val2 == 1299) {
          if (val2 == 1300) {LCD.print("ON "); val2 = 1300;}
          else {LCD.print("OFF"); val2 = 1299;}
        }
        if (params[id][amperoquick[1][count1]] < 128) {LCD.print(params[id][amperoquick[1][count1]]); val2 = -1;}
    break;
    case 3 :
        LCD.setCursor(0,0);
        if (texteline1 != "P  B      TONE_X") {
            texteline1  = "P  B      TONE_X";
          LCD.print(texteline1);
          LCD.setCursor(4,0); LCD.print(bank);
        }
        texte = String(pied) + " ";LCD.setCursor(1,0); LCD.print(texte);
        LCD.setCursor(0,1);
        if (texteline2 != namequicktone[count2]) {texteline2 = namequicktone[count2]; LCD.print(texteline2);}
        LCD.setCursor(13,1); LCD.print("   ");
        LCD.setCursor(13,1);
        if (val2 == 1399 or val2 == 1400) {if (val2 == 1399) {LCD.setCursor(10,1); LCD.print(":  PRE"); val2 = 1399;}
          else {LCD.setCursor(10,1); LCD.print(": POST"); val2 = 1400;}}
        if (val2 == 1299 or val2 == 1300) {
          if (val2 == 1300) {LCD.print("ON "); val2 = 1300;}
          else {LCD.print("OFF"); val2 = 1299;}
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
    case 4 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("Pied :  |Bank:  ");
      LCD.setCursor(7,0);
      LCD.print(pied);
      LCD.setCursor(15,0);
      LCD.print(bank);
      LCD.setCursor(0,1);
      LCD.print("Stomp:  | Amp/Tx");
      LCD.setCursor(7,1);
      LCD.print(stomp);
    break;
    case 5 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("***  RESTART ***");
      LCD.setCursor(0,1);
      LCD.print("***** WAIT *****");
    break;
    case 6 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print(" Copy PATCH : ");
      LCD.setCursor(0,1);
      LCD.print("...To PATCH : ");
    break;
    case 7 :
      LCD.setCursor(14,0);
      LCD.print(cp1);
      if (cp1 < 10) {LCD.setCursor(15,0); LCD.print(" ");}
    break;
    case 8 :
      byte tmp3, tmp4;
      LCD.setCursor(13,1);
      if (cp2 < 7) {tmp3 = 1;} if (cp2 > 6 && cp2 < 13) {tmp3 = 2;} if (cp2 > 12 && cp2 < 19) {tmp3 = 3;} if (cp2 > 18 && cp2 < 25) {tmp3 = 4;}
      if (cp2 > 24 && cp2 < 31) {tmp3 = 5;} if (cp2 > 30 && cp2 < 37) {tmp3 = 6;}
      if (cp2 == 1 or cp2 == 7  or cp2 == 13 or cp2 == 19 or cp2 == 25 or cp2 == 31) {tmp4 = 1;}
      if (cp2 == 2 or cp2 == 8  or cp2 == 14 or cp2 == 20 or cp2 == 26 or cp2 == 32) {tmp4 = 2;}
      if (cp2 == 3 or cp2 == 9  or cp2 == 15 or cp2 == 21 or cp2 == 27 or cp2 == 33) {tmp4 = 3;}
      if (cp2 == 4 or cp2 == 10 or cp2 == 16 or cp2 == 22 or cp2 == 28 or cp2 == 34) {tmp4 = 4;}
      if (cp2 == 5 or cp2 == 11 or cp2 == 17 or cp2 == 23 or cp2 == 29 or cp2 == 35) {tmp4 = 5;}
      if (cp2 == 6 or cp2 == 12 or cp2 == 18 or cp2 == 24 or cp2 == 30 or cp2 == 36) {tmp4 = 6;}
      texte = String(tmp3) + "-" + String(tmp4);
      LCD.print(texte);
      Serial.println(cp2);
    break;
    case 9 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("** CHARGEMENT **");
      LCD.setCursor(0,1);
      LCD.print("***  BANK    ***");
      LCD.setCursor(10,1);
      LCD.print(bank);
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
      LCD.print("Copy Patch :    ");
      LCD.setCursor(0,1);
      LCD.print("<-DATA ** COPY->");
    break;
    case 13 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("* Copy en cours *");
      LCD.setCursor(0,1);
      LCD.print("** AMP/TONE-X **");
    break;
    case 14 :
      LCD.clear();
      LCD.setCursor(13,0);
      LCD.print(cp1);
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
  static int m; String test = "Val = ";
    for (m = 0; m<argc; m++){
      String val = argv[m];
      String val2 = azColName[m];
      switch (choixbd){
        case 0 :
          params[i][m] = val.toInt(); paramsCopy [i][m] = val.toInt();
          //test += String(params[i][m]) + ", ";
        break;
        case 1 :
        break;
      } 
    }
  //Serial.println(test);
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
    if (i < 56){
      //if (params[idcopy][i] != paramsCopy[idcopy][i]) {
        sql += "'"+String(i)+"'="+String(params[idcopy][i])+", "; 
      //}
    }
    if (i == 56){
        sql += "'"+String(i)+"'="+String(params[idcopy][i]); 
    }
  }
  if (bank == 1 ) {sql += " WHERE stomps_id="+String(idcopy)+";";}
  if (bank == 2 ) {sql += " WHERE stomps_id="+String(idcopy+42)+";";}
  if (bank == 3 ) {sql += " WHERE stomps_id="+String(idcopy+42*2)+";";}
  if (bank == 4 ) {sql += " WHERE stomps_id="+String(idcopy+42*3)+";";}
  if (bank == 5 ) {sql += " WHERE stomps_id="+String(idcopy+42*4)+";";}
  if (bank == 6 ) {sql += " WHERE stomps_id="+String(idcopy+42*5)+";";}
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

void saveDataScene() {
  if (l == 1) { //Data Read Only
    sql = "UPDATE stomps SET ";
    for (i = 1; i < 59; i++){
      if (i < 58){
      sql += "'"+String(i)+"'="+String(params[j][i])+", "; 
      } 
      if (i == 58){
      sql += "'"+String(i)+"'="+String(params[j][i]);
      }
    }
    sql += " WHERE stomps_id="+String(idcopy)+";";
    Serial.println(sql);
  }
  if (l == 2) { // Copy Data Main -> 6 prochains
    if (db_open("/spiffs/base.db", &db_base)) return;
    sql = "UPDATE stomps SET ";
    for (i = 1; i < 59; i++){
      if (i < 58){
      sql += "'"+String(i)+"'="+String(params[j][i])+", ";paramsCopy[j][i] = params[j][i];
      } 
      if (i == 58){
      sql += "'"+String(i)+"'="+String(params[j][i]); paramsCopy[j][i] = params[j][i];
      }
    }
    sql += " WHERE stomps_id="+String(idcopy)+";";
    db_exec(db_base, sql.c_str());
    sqlite3_close(db_base);
    Serial.println(sql);
  }
}

void readData (){
  choixbd = 0;
  if (db_open("/spiffs/base.db", &db_base)) return; 
  static byte o;
  //Serial.print("Bank = "); Serial.println(bank);
  if (bank == 1 ) {o = id;}
  if (bank == 2 ) {o = id + 42;}
  if (bank == 3 ) {o = id + 42*2;}
  if (bank == 4 ) {o = id + 42*3;}
  if (bank == 5 ) {o = id + 42*4;}
  if (bank == 6 ) {o = id + 42*5;}
  for (i = 1; i < 43; i++) {
    sql = "SELECT * FROM stomps WHERE stomps_id = " + String(o) + ";";
    db_exec(db_base, sql.c_str());
    o = o + 1;
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
      encoder2.setCount(params[id][count]); enc2last = params[id][count];
    break;
    case 5 :
      encoder2.setCount(params[id][count]); enc2last = params[id][count];
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

void encod1(int valmini, int valmaxi, int pot, byte sel){
  switch (sel) {
    case 0 :
      pot = encoder1.getCount();
      if (pot > valmaxi) {pot = valmaxi;}
      if (pot < valmini) { pot = valmini; }
      if ( count1 < 5 ) {
        midiblabla(amperoquick[0][count1], pot, canal2);
        params[id][amperoquick[1][count1]] = pot;
      }
      else {
        if (pot == 1299) {
          midiblabla(amperoquick[0][count1], 0, canal2); 
          params[id][amperoquick[1][count1]] = pot;
        }
        if (pot == 1300) {
          midiblabla(amperoquick[0][count1], 127, canal2);
          params[id][amperoquick[1][count1]] = pot;
        }
        Serial.println(pot);
      }
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
    case 3 :
      pot = encoder1.getCount();
      if (pot > valmaxi) {pot = valmini;}
      if (pot < valmini) {pot = valmaxi;}        
      cp1 = pot;
      encoder1.setCount(pot);
      enc1last = pot;
      Screens(14, cp1);
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
        params[id][count] = pot;
      }
      if (count == 56) {
        progChange(ampvalbk, pot, canal2);
        params[id][count] = pot;
        }
      if (count != 55 or count != 56 or count != 54){ // Suppression reglage 54
        params[id][count] = pot;
        if (params[id][count] == 1399 or params[id][count] == 1400){
          if (params[id][count] == 1399){midiblabla(totalcc[count], 0, canal1);}
          else {midiblabla(totalcc[count], 127, canal1);}
        }
        if (count > 17 && count < params[id][54]) {
          if (params[id][count] == 1299){midiblabla(totalcc[count], 0, canal1);}
          if (params[id][count] == 1300){midiblabla(totalcc[count], 127, canal1);}
          if (params[id][count] < 128){
            midiblabla(totalcc[count], pot, canal1);
          }
        }
        if (count > 0 && count < 18) {
          if (params[id][count] == 1299){midiblabla(totalcc[count], 0, canal2);}
          if (params[id][count] == 1300){midiblabla(totalcc[count], 127, canal2);}
          if (params[id][count] < 128){
            midiblabla(totalcc[count], pot, canal2);
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
  static int encvalMax, encvalMini;
  while (menus == 1) {
    initEncoder(2);
    if (count == 55){initEncoder(4);}
    if (count == 56){initEncoder(5);}
    if (count != 55 && count !=56){initEncoder(6);}
    if (enc1last != encoder1.getCount()) {encod1(1,56,count,1); } 
    if (enc2last != encoder2.getCount()) {
      if (count != 55 && count != 56) {
        if (params[id][count] == 1399 or params[id][count] == 1400){
          if (params[id][count] == 1400) {tmp = 1400; encvalMax = 1400; encvalMini = 1399;}
          else {tmp = 1399; encvalMax = 1400; encvalMini = 1399;}
        }
        if (params[id][count] == 1299 or params[id][count] == 1300){
          if (params[id][count] == 1300) {tmp = 1300; encvalMax = 1300; encvalMini = 1299;}
          else {tmp = 1299; encvalMax = 1300; encvalMini = 1299;}
        }
        if (params[id][count] < 128){
          if (params[id][count] < 128) {tmp = -1; encvalMax = valmaxcc[count]; encvalMini = 0; }
        }
      } 
      else {
        if (params[id][55] < 128 && count == 55){tonevalbk = 0;}
        if (params[id][56] < 128 && count == 56){ampvalbk =  0;}
        if (params[id][55] > 127 && params[id][55] < 151 && count == 55){tonevalbk = 1;}
        if (params[id][56] > 127 && params[id][56] < 257 && count == 56){ampvalbk = 1;}
        if (params[id][56] > 255 && params[id][56] < 301 && count == 56){ampvalbk = 2;}
        tmp = -1; encvalMax = valmaxcc[count]; encvalMini = 0; 
      }
    if (count != 42 && count != 54){encod2(encvalMini,encvalMax,count,1);}
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
  previousMillis = millis(); tmp2 = 1;
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
            if (params[id][amperoquick[1][count1]] > 1390){
              if (params[id][amperoquick[1][count1]] == 1399) {tmp = 1399;}
              else {tmp = 1400;}
            }
            if (params[id][amperoquick[1][count1]] > 1290 && params[id][amperoquick[1][count1]] < 1305) {
              if (params[id][amperoquick[1][count1]] == 1300) {tmp = 1300;}
              else {tmp = 1299;}
            } 
            if (params[id][amperoquick[1][count1]] < 1000) {tmp = -1;}
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
            if (params[id][tonexquick[1][count2]] > 1390){
              if (params[id][tonexquick[1][count2]] == 1399) {tmp = 1399;}
              else {tmp = 1400;}
            }
            if (params[id][tonexquick[1][count2]] > 1290 && params[id][tonexquick[1][count2]] > 1305) {
                if (params[id][tonexquick[1][count2]] == 1300) {tmp = 1300;}
                else {tmp = 1299;}
              }
            if (params[id][tonexquick[1][count2]] > 1000) {tmp = -1;}
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
  previousMillis = millis(); tmp2 = 1;
}

void SavePatch(){
  Screens(11,0);
  //int j, k, l;
  l = idcopyplus[cp1];
  k = idcopyplus[cp2];
  for (k ; k < idcopyplus[cp2]+7; k++ && l++) {
    for (j = 1; j < 60; j++){
      if (j != 55 or j != 56) {Copytemp[j] = params[l][j];}
      if (j == 55) {Copytemp[j] = params[l][55];}
      if (j == 56) {Copytemp[j] = params[l][56];}
    }
    //Serial.println("");Serial.print(" ID = ");Serial.println(l);
    idcopy = k;
    saveDataCopy();
    delay(50);
  }  
  Screens(5,0);
  delay(800);
  esp_restart();
}

void CopyPatch(){
  tmp = 1;
  initEncoder(7);
  while (menus == 2){
    if (tmp == 1) {Screens(6, 0); Screens(7,0); Screens(8,0); tmp = 0;}
    if (enc1last != encoder1.getCount()) {
      if(bank == 1){encod1(1,6,cp1,2);}
      if(bank == 2){encod1(7,12,cp1,2);}
      if(bank == 3){encod1(13,18,cp1,2);}
      if(bank == 4){encod1(19,24,cp1,2);}
      if(bank == 5){encod1(25,30,cp1,2);}
      if(bank == 6){encod1(31,36,cp1,2);}
    } 
    if (enc2last != encoder2.getCount()) {encod2(1,36,cp2,3);} 
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
  previousMillis = millis(); tmp2 = 1;
}

void saveScenes(){
  Screens(13, 0);               //  1 2  3  4  5  6   7  8  9 10 11 12  13 14 15  16  17  18   19  20  21  22  23  24   25  26  27  28  29  30   31  32  33  34  35  36 
  //static int idcopyplus [37] = {0,1,8,15,22,29,36, 43,50,57,64,71,78, 85,92,99,106,113,120, 127,134,141,148,155,162, 169,176,183,190,197,204, 211,218,225,232,239,246};
  if (bank == 1){m = idcopyplus[cp1];}
  if (bank == 2){m = idcopyplus[cp1+6];}
  if (bank == 3){m = idcopyplus[cp1+12];}
  if (bank == 4){m = idcopyplus[cp1+18];}
  if (bank == 5){m = idcopyplus[cp1+24];}
  if (bank == 6){m = idcopyplus[cp1+30];}
  for (j = idcopyplus[cp1]; j < idcopyplus[cp1]+7; j++ && m++){idcopy = m; saveDataScene();
  }
  count1 = 0; count2 = 0;
}

void CopyPC(){
  tmp = 1; l = 0;
  while (menus == 2){
    if (tmp == 1) {Screens(12, 0);tmp = 0;}
    if (enc1last != encoder1.getCount()) {
      encod1(1,6,cp1,2);
    } 
    if (digitalRead(5) == 0) {
      delay(250);
      if (digitalRead(5) == 0) {
          l = 1;
          saveScenes();
        }
      startcharg = 2;
      break;
    }
    if (digitalRead(23) == 0) {
      delay(250);
      if (digitalRead(23) == 0) {
          l = 2;
          saveScenes();
        }
      startcharg = 3;
      break;
    }
  }
}

void choixprogchang(){
  if (params[id][55] < 128 or params[id][56] < 128){ampvalbk = 0; tonevalbk = 0;}
  if (params[id][55] > 127 && params[id][55] < 151){tonevalbk = 1;}
  if (params[id][56] > 127 && params[id][56] < 257){ampvalbk = 1;}
  if (params[id][56] > 255 && params[id][56] < 301){ampvalbk = 2;}
}

void firstcharg(){
  progChange(tonevalbk, params[id][55], canal1);
  delay(100);
  progChange(ampvalbk,  params[id][56], canal2);
  delay(100);
  for (i = 18; i < params[id][54]; i++){
    if (params[id][i] == 1399) {MIDI.sendControlChange(totalcc[i], 0, canal1);}
    if (params[id][i] == 1400) {MIDI.sendControlChange(totalcc[i], 127, canal1);}
    if (params[id][i] == 1299) {MIDI.sendControlChange(totalcc[i], 0, canal1);}
    if (params[id][i] == 1300) {MIDI.sendControlChange(totalcc[i], 127, canal1);}
    if (i == 42) {typereverb();}
    else {if (params[id][i] < 128) {MIDI.sendControlChange(totalcc[i], params[id][i], canal1);}}
    delay(15);
  }
  for (i = 1; i < 18; i++){
    if (i < 14) {
      if (params[id][i] == 1299) {MIDI.sendControlChange(totalcc[i], 0, canal2);}
      else {MIDI.sendControlChange(totalcc[i], 127, canal2);}
    if (i > 13) {MIDI.sendControlChange(totalcc[i], params[id][i], canal2);}
    delay(5);
    }
  }
}

void changtAmpero() {
  if (params[id][i] == 1299) {MIDI.sendControlChange(totalcc[i], 0, canal2);}
  if (params[id][i] == 1300) {MIDI.sendControlChange(totalcc[i], 127, canal2);}
  if (i > 13) {MIDI.sendControlChange(totalcc[i], params[id][i], canal2);}
  //Serial.print(i);Serial.print("="); Serial.print(params[id][i]);Serial.print("|");
}

void changToneX() {
  if (params[id][i] == 1399) {MIDI.sendControlChange(totalcc[i], 0, canal1);}
  if (params[id][i] == 1400) {MIDI.sendControlChange(totalcc[i], 127, canal1);}
  if (params[id][i] == 1299) {MIDI.sendControlChange(totalcc[i], 0, canal1);}
  if (params[id][i] == 1300) {MIDI.sendControlChange(totalcc[i], 127, canal1);}
  if (params[id][i] < 128) {if (i != 42) {MIDI.sendControlChange(totalcc[i], params[id][i], canal1);}}
  if (i == 42) {count = 42; typereverb();}
  delay(15);
  //Serial.print(i);Serial.print("="); Serial.print(params[id][i]);Serial.print("|");
}

void chgtPedal() {
  byte tmp5 = 0;
  if (params[id][56] != params[preid][56]) {progChange(ampvalbk,  params[id][56], canal2); }
  if (params[id][55] != params[preid][55]) {progChange(tonevalbk, params[id][55], canal1); }
  
  for (i = 1; i < 18; i++){
    if (params[id][56] != params[preid][56]) {changtAmpero(); tmp5 = 1;}
    if (params[id][i] != paramsCopy[preid][i] && tmp5 == 0) {changtAmpero(); tmp5 = 0;}
  }
  tmp5 = 0;
  //Serial.print(" // "); 
  if (params[id][55] != params[preid][55]) {progChange(tonevalbk, params[id][55], canal1); }
  delay(30);
  for (i = 18; i < 46; i++){
    if (params[id][55] != params[preid][55]) {changToneX(); tmp5 = 1;}
    if (params[id][i] != paramsCopy[preid][i] && tmp5 == 0) {changToneX(); tmp5 = 0;}
  }
  //Serial.println("");
}

void Select() {
  choixprogchang();
  if (startcharg == 1) {readData(); choixprogchang(); firstcharg(); oldbank = bank;}
  if (bank != oldbank) {
    Screens(9, 0); 
    readData(); 
    oldbank = bank; texteline1 = "toto"; texteline2 = "toto"; 
    Screens(2, 0);}
}

void handleControlChange(byte channel, byte number, byte value) {
  if (channel == 12){bank = value; Select(); choixprogchang(); firstcharg();}
  //Serial.print("Bank = ");Serial.println(bank);
  if (channel == 16) {
    pied = 1;
    preid = id; 
    id_init = value; id = value;
    count1 = 0; count2 = 0; menus = 0;
    Select(); 
    chgtPedal();
    BoutRot(1, menus);
    if (id < 8) {pied = 1;}
    if (id > 7 && id < 14) {pied = 2;}
    if (id > 13 && id < 21) {pied = 3;}
    if (id > 20 && id < 28) {pied = 4;}
    if (id > 27 && id < 35) {pied = 5;}
    if (id > 34 && id < 43) {pied = 6;}
    stomp = 0;
    Screens(4, 0);
    tmp2 = 0; texteline1 = "toto"; texteline2 = "toto";
  }

  if (channel == 15) {
    preid = id;
    stomp = value;
    id = id_init + value;
    count1 = 0; count2 = 0; menus = 0;
    chgtPedal();
    BoutRot(1, menus);
    Screens(4, 0);
    tmp2 = 0; texteline1 = "toto"; texteline2 = "toto";
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
  MIDI.begin(MIDI_CHANNEL_OMNI);  
  MIDI.setHandleControlChange(handleControlChange);

  Wire.begin(22, 21);

  button7.setPressTicks(500);
  button8.setPressTicks(500);

  button7.attachClick([] () {if(menus == 0) {btn = 1; BoutRot(1, menus);}});
  button8.attachClick([] () {if(menus == 0) {btn = 2; BoutRot(2, menus);}});

  button7.attachLongPressStop([] () {texteline2 = "toto"; menus = 1; btn = 1; BoutRot(1, menus);});
  button8.attachLongPressStop([] () {texteline2 = "toto"; menus = 1; btn = 2; BoutRot(2, menus);});

  button7.attachDoubleClick([] () {texteline2, texteline1 = "toto"; menus = 2; CopyPC();});
  button8.attachDoubleClick([] () {texteline2, texteline1 = "toto"; menus = 2; CopyPatch();});

  if(SPIFFS.begin(true)){
    File root = SPIFFS.open("/"); bank = 1; id = 1, id_init = id; startcharg = 1; pied = 1; Select(); preid = 0;}
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
  previousMillis = millis();
  tmp2 = 1;
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
  if (startcharg == 2) {startcharg = 0; menus = 0; texteline1 = "toto"; texteline2 = "toto"; BoutRot(1, menus); Screens(2, 0);}  
  if (startcharg == 3) {startcharg = 0; menus = 0; texteline1 = "toto"; texteline2 = "toto"; BoutRot(2, menus); Screens(3, 0);}
  if (millis() - previousMillis >= interval && tmp2 == 1) {Screens(4, 0); tmp2 = 0; texteline1 = "toto"; texteline2 = "toto";}
}

