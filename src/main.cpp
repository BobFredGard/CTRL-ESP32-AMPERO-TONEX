#include <Arduino.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C LCD(0x3F,16,2); 

#include <SPIFFS.h>

#include <WiFi.h>
const char *ssid2 = "HUAWEI-B715-A231";
const char *password2 = "G6TT4Y3H5DA";
IPAddress ip;

#include "Network.h"
#include "Sys_Variables.h"
#include "CSS.h"

#include <ESP32WebServer.h>
ESP32WebServer server(80);
#include <ESPmDNS.h>
#include <DataServer.h>

#include <sqlite3.h>
sqlite3 *db_base;

#include <OneButton.h>
//OneButton button4(26, true); OneButton button5(27, true); OneButton button6(14, true); OneButton button7(5, true);
//OneButton button1(26, true); OneButton button2(27, true); OneButton button3(14, true); 
OneButton button7(5, true);
OneButton button8(23, true);

#include <ESP32Encoder.h>
ESP32Encoder left_encoder;
ESP32Encoder right_encoder;
static int last_value_left_encoder, last_value_right_encoder;

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

static int i, j, k, l, m, tmp, choiced_foot_on_pacer, tmp2 = 0;
static int data_base_choice, old_bank, menus, stomp = 0; 
static int id, id_init, idcopy, global_count, startscreen, bank, ampero_count, tonex_count = 1;
static int initial_state, boutton_choice, encoder1_copy_source, encoder2_copy_destination = 1;
static short canal_midi_2 = 7; static short canal_midi_1 = 8;
static int preid = -1;
static int ampero_bank_choice, tonex_bank_choice = 0;
static byte tx1, tx2, tx3 = 1;

unsigned long previousMillis = 0;
unsigned long interval = 3000;

static short global_control_change [54] =  {
  //  |------CC------>>> AMPERO II STOMPS <<<-------CC-----------------| |------------CC------------>>> TONE X <<<--------------------------CC---------------------------------------------------------------------------------------------|
   0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 13, 16, 18, 20, 7, 102, 23, 25, 28, 79, 19, 15, 106, 107, 103, 14, 16, 17, 18, 20, 21, 22, 24, 26, 27, 29, 30, 104, 75, 85, 76, 77, 78, 108, 109, 110, 111, 112, 113, 114, 115};
//{0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,  25,  26,  27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,  40, 41, 42, 43, 45, 45,  46,  47,  48,  49,  50,  51,  52,  53}
  //  Numéro colonne de la base de donnée


                                   //  |--------CC----------|      |--Positions dans BD--|
static short ampero_quick_values [2] [7] = {{-10, 7, 16, 18, 20, 13, 59}, {0, 17, 14, 15, 16, 13, 12}};
static short tonex_quick_values [2] [7] = {{-10, 103, 102, 19, 15, 106, 107}, {0, 27, 18, 23, 24, 25, 26}};

static const char* ampero_quick_names [7]  {"", "VOLUME     : ", "PARAM 1     : ", "PARAM 2    : ", "PARAM 3    : ", "PEDAL EXP  : ", "REVERB ON  : "};
static const char*  tonex_quick_names [7] {"", "MODEL.VOL  : ", "GAIN       : ", "COMPRESSOR : ", "GATE       : ", "PRESENCE   : ", "DEPHT      : "};
static int values_max_ampero_quick [7] = {0,100, 127, 127, 127, 1300, 1300};
static int values_mini_ampero_quick [7] = {0,0, 0, 0, 0, 1299, 1299};
static int values_max_tonex_quick [7] = {0,127, 127, 127, 127, 127, 127};
static int values_mini_tonex_quick [7] = {0,0, 0, 0, 0, 0, 0};

static int all_parameters [43][ 60];
static int all_data_base_parameters_copy [43] [60];
static int temp_transfert_datas [60];

static const char* line1_lcd_text;
static const char* line2_lcd_text;
static String slq_request;

static const char* global_names [57] = {  "", 
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

static int global_control_change_max_values [57] = {0,
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

static int id_values_for_6_foot [37] = {0,1,8,15,22,29,36,43,50,57,64,71,78,85,92,99,106,113,120,127,134,141,148,155,162,169,176,183,190,197,204,211,218,225,232,239,246};

static const char* tonex_reverb_names [5] = {"SPRING1", "SPRING2", "SPRING3", "ROOM ", "PLATE"};

void program_change(byte CC, int val, byte canal) {
  MIDI.sendControlChange(0, CC, canal);
  MIDI.sendProgramChange(val, canal);
}

void midiblabla(byte a, byte b, byte c) {
      MIDI.sendControlChange(a, b, c);
      /*(""); Serial.print("ToneX -> CC : "); Serial.print(a); Serial.print(" - Val : ");
      Serial.print(b); Serial.print(" - Channel : "); Serial.println(c); */
}

void typereverb() {
  if (all_parameters[id][42] >= 0 && all_parameters[id][42] < 2) {midiblabla(global_control_change[42], 10, canal_midi_1);}
  if (all_parameters[id][42] >= 2 && all_parameters[id][42] < 4) {midiblabla(global_control_change[42], 20, canal_midi_1);}
  if (all_parameters[id][42] >= 4 && all_parameters[id][42] < 6) {midiblabla(global_control_change[42], 55, canal_midi_1);}
  if (all_parameters[id][42] >= 6 && all_parameters[id][42] < 8) {midiblabla(global_control_change[42], 85, canal_midi_1);}
  if (all_parameters[id][42] >= 8 && all_parameters[id][42] < 10) {midiblabla(global_control_change[42], 115, canal_midi_1);}
  //Serial.println(42);
}

static float valminimax [2] [57] = {
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

float view_tonex_value, all_data_base_parameters;

void Scale_value_switch (byte val3) {
  view_tonex_value = (valminimax[1][global_count]-valminimax[0][global_count])*all_parameters[id][global_count]/global_control_change_max_values[global_count]+valminimax[0][global_count];
  switch (val3){
  case 1 :
    LCD.setCursor(12,1); LCD.print("    "); 
    if (view_tonex_value >=0 && view_tonex_value < 10) {LCD.setCursor(13,1);}
    if (view_tonex_value > 9.99 && view_tonex_value <= 100) {LCD.setCursor(12,1);}
    if (view_tonex_value < 0 && view_tonex_value < -100) {LCD.setCursor(13,1);}
    if (view_tonex_value < 0 && view_tonex_value >= -100) {LCD.setCursor(12,1);}
    LCD.print((valminimax[1][global_count]-valminimax[0][global_count])*all_parameters[id][global_count]/global_control_change_max_values[global_count]+valminimax[0][global_count]);
  break;
  case 2 :
    LCD.setCursor(12,1); LCD.print("    "); 
    if (view_tonex_value >=0 && view_tonex_value < 10) {LCD.setCursor(14,1);}
    if (view_tonex_value >=0 && view_tonex_value > 9 && view_tonex_value < 100) {LCD.setCursor(13,1);}
    if (view_tonex_value >=0 && view_tonex_value >= 100) {LCD.setCursor(12,1);}
    LCD.print(round((valminimax[1][global_count]-valminimax[0][global_count])*all_parameters[id][global_count]/global_control_change_max_values[global_count]+valminimax[0][global_count]));
    LCD.setCursor(15,1); LCD.print("%"); 
  break;
  case 3 :
    LCD.setCursor(9,1); LCD.print("       "); LCD.setCursor(9,1); 
    if (all_parameters[id][global_count] >= 0 && all_parameters[id][global_count] < 2) {LCD.print(tonex_reverb_names[0]);}
    if (all_parameters[id][global_count] >= 2 && all_parameters[id][global_count] < 4) {LCD.print(tonex_reverb_names[1]);}
    if (all_parameters[id][global_count] >= 4 && all_parameters[id][global_count] < 6) {LCD.print(tonex_reverb_names[2]);}
    if (all_parameters[id][global_count] >= 6 && all_parameters[id][global_count] < 8) {LCD.print(tonex_reverb_names[3]);}
    if (all_parameters[id][global_count] >= 8 && all_parameters[id][global_count] < 10) {LCD.print(tonex_reverb_names[4]);}
  break;
  }
}

#include"ChoixCopyPatch.h"
void Screens (byte screen_choice, int val2) {
  //LCD.clear();
  String text;
  switch (screen_choice) {
    case 0 :
      LCD.clear();
      LCD.setCursor(1,0);
      LCD.print("AMPERO 2 STOMP");
      LCD.setCursor(5,1);
      LCD.print("TONE X");
    break;
    case 1 :
      if (global_count > 0 && global_count < 18){
        LCD.setCursor(0,0);
        if (line1_lcd_text != "P  B  *AMPERO II") 
           {line1_lcd_text  = "P  B  *AMPERO II"; 
          LCD.print(line1_lcd_text);
          LCD.setCursor(4,0); LCD.print(bank);
        }
        text = String(choiced_foot_on_pacer) + " ";LCD.setCursor(1,0); LCD.print(text);
      }
      if (global_count > 17 && global_count < 54){
        LCD.setCursor(0,0);        
        if (line1_lcd_text != "P  B     *TONE_X") {
           line1_lcd_text  = ("P  B     *TONE_X"); 
          LCD.print(line1_lcd_text);
          LCD.setCursor(4,0); LCD.print(bank);
        }
        text = String(choiced_foot_on_pacer) + " ";LCD.setCursor(1,0); LCD.print(text);
      }
      if (global_count == 54){
        if (line1_lcd_text != "*Fin Sauvegd") {
          LCD.setCursor(5,0); line1_lcd_text = "*Fin Sauvegd";
          LCD.print(line1_lcd_text);
        }
      }
      if (global_count == 55 or global_count == 56){
        if (line1_lcd_text != "*Prog Change") {
          LCD.setCursor(5,0); line1_lcd_text = "*Prog Change";
          LCD.print(line1_lcd_text);
        }
      }
      LCD.setCursor(0,1);
      if (line2_lcd_text != global_names[global_count]) {
        line2_lcd_text = global_names[global_count];
        LCD.print(line2_lcd_text);
      }
      LCD.setCursor(13,1);
      LCD.print("   ");
      LCD.setCursor(13,1); 
      if (global_count != 55 && global_count != 56) {
        if (val2 == 1399 or val2 == 1400) {
          if (val2 == 1399) {
            LCD.setCursor(10,1);
            LCD.print(":  PRE");
            val2 = 1399;
          }
          else {
            LCD.setCursor(10,1);
            LCD.print(": POST");
            val2 = 1400;
          }
        }
        if (val2 == 1300 or val2 == 1299) {
            if (all_parameters[id][global_count] == 1300) {
              LCD.print("ON "); val2 = 1300;
            }
            else {
              LCD.print("OFF"); val2 = 1299;
            }
        }
        if (all_parameters[id][global_count] < 128) {          
          if (global_count < 18) {
            LCD.print(all_parameters[id][global_count]);
          }
          if (global_count > 17) {
            if (valminimax[1][global_count] == 10) {
              Scale_value_switch(1);
            }
            if (global_count == 22 or global_count == 40 or global_count == 53) {
              Scale_value_switch(2);
            }
            if (global_count != 22 or global_count != 40 or global_count != 53 or valminimax[1][global_count] != 10 or global_count != 42) {
              LCD.print(all_parameters[id][global_count]);
            }
            if (global_count == 42) {
              Scale_value_switch(3);
            }
          }
          val2 = -1;
        } 
      }
      if(global_count == 55) {
        LCD.print(all_parameters[id][55]); 
        val2 = -1;
      }
      if(global_count == 56) {
        LCD.print(all_parameters[id][56]); 
        val2 = -1;
      } 
    break;
    case 2 :
        LCD.setCursor(0,0);
        if (line1_lcd_text != "P  B   AMPERO II") {
          line1_lcd_text  = "P  B   AMPERO II"; 
          LCD.print(line1_lcd_text);
          LCD.setCursor(4,0); LCD.print(bank);
        }
        text = String(choiced_foot_on_pacer) + " ";
        LCD.setCursor(1,0); 
        LCD.print(text);
        LCD.setCursor(0,1);
        if (line2_lcd_text != ampero_quick_names[ampero_count]) {
          line2_lcd_text = ampero_quick_names[ampero_count]; 
          LCD.print(line2_lcd_text);
        }
        LCD.setCursor(13,1); 
        LCD.print("   ");
        LCD.setCursor(13,1);
        if (val2 == 1399 or val2 == 1400) {
            if (val2 == 1399) {LCD.setCursor(10,1); 
              LCD.print(":  PRE"); 
              val2 = 1399;
            }
          else {
            LCD.setCursor(10,1); 
            LCD.print(": POST"); 
            val2 = 1400;
          }
        }
        if (val2 == 1300 or val2 == 1299) {
          if (val2 == 1300) {
            LCD.print("ON "); 
            val2 = 1300;
          }
          else {
            LCD.print("OFF"); 
            val2 = 1299;
          }
        }
        if (all_parameters[id][ampero_quick_values[1][ampero_count]] < 128) {
          LCD.print(all_parameters[id][ampero_quick_values[1][ampero_count]]); 
          val2 = -1;
        }
    break;
    case 3 :
        LCD.setCursor(0,0);
        if (line1_lcd_text != "P  B      TONE_X") {
          line1_lcd_text  = "P  B      TONE_X";
          LCD.print(line1_lcd_text);
          LCD.setCursor(4,0); LCD.print(bank);
        }
        text = String(choiced_foot_on_pacer) + " ";
        LCD.setCursor(1,0); 
        LCD.print(text);
        LCD.setCursor(0,1);
        if (line2_lcd_text != tonex_quick_names[tonex_count]) {
          line2_lcd_text = tonex_quick_names[tonex_count]; 
          LCD.print(line2_lcd_text);
        }
        LCD.setCursor(13,1); 
        LCD.print("   ");
        LCD.setCursor(13,1);
        if (val2 == 1399 or val2 == 1400) {
          if (val2 == 1399) {
            LCD.setCursor(10,1); 
            LCD.print(":  PRE"); 
            val2 = 1399;
          }
          else {
            LCD.setCursor(10,1); 
            LCD.print(": POST"); 
            val2 = 1400;
          }
        }
        if (val2 == 1299 or val2 == 1300) {
          if (val2 == 1300) {
            LCD.print("ON ");
            val2 = 1300;
          }
          else {
            LCD.print("OFF");
            val2 = 1299;
          }
        }
        if (all_parameters[id][tonex_quick_values[1][tonex_count]] < 128) {
          global_count = tonex_quick_values[1][tonex_count];
          //LCD.print(all_parameters[id][global_count]);
          if (valminimax[1][global_count] == 10) {
            Scale_value_switch(1);
          }
          if (global_count == 22 or global_count == 40 or global_count == 53) {
            Scale_value_switch(2);
          }
          if (global_count != 22 or global_count != 40 or global_count != 53 or valminimax[1][global_count] != 10 or global_count != 42) {
            LCD.print(all_parameters[id][global_count]);
          }
          if (global_count == 42) {
            Scale_value_switch(3);
          }
          val2 = -1;
        }
    break;
    case 4 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("Pied :  |Bank:  ");
      LCD.setCursor(7,0);
      LCD.print(choiced_foot_on_pacer);
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
      LCD.print("***** LOAD *****");
      LCD.setCursor(0,1);
      LCD.print("**** PATCHS ****");
    break;
    case 6 :
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print(" Copy PATCH : ");
      LCD.setCursor(0,1);
      LCD.print("To Bk-PATCH : ");
    break;
    case 8 :
      ChoixCopyPatch ();
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
      LCD.print("Copy Scene :    ");
      LCD.setCursor(0,1);
      LCD.print("to b/p/s :      ");
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
      LCD.setCursor(0,0);
      LCD.print("** Copy Scene **");
      LCD.setCursor(0,1);
      LCD.print("*** en cours ***");
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
      switch (data_base_choice){
        case 0 :
          all_parameters[i][m] = val.toInt(); all_data_base_parameters_copy [i][m] = val.toInt();
          //test += String(all_parameters[i][m]) + ", ";
        break;
        case 1 :
        break;
      } 
    }
  //Serial.println(test);
  id = 1;
  return 0;
}

static int db_exec(sqlite3 *db, const char *slq_request) {
  sqlite3_exec(db, slq_request, call, (void*)daTa, &zErrMsg);
  return 0;
}

void saveData() {
  if (db_open("/spiffs/base.db", &db_base)) return;
  slq_request = "UPDATE stomps SET ";
  for (i = 1; i < 57; i++){
    if (i < 56){
      //if (all_parameters[idcopy][i] != all_data_base_parameters_copy[idcopy][i]) {
        slq_request += "'"+String(i)+"'="+String(all_parameters[idcopy][i])+", "; 
      //}
    }
    if (i == 56){
        slq_request += "'"+String(i)+"'="+String(all_parameters[idcopy][i]); 
    }
  }
  if (bank == 1 ) {slq_request += " WHERE stomps_id="+String(idcopy)+";";}
  if (bank == 2 ) {slq_request += " WHERE stomps_id="+String(idcopy+42)+";";}
  if (bank == 3 ) {slq_request += " WHERE stomps_id="+String(idcopy+42*2)+";";}
  if (bank == 4 ) {slq_request += " WHERE stomps_id="+String(idcopy+42*3)+";";}
  if (bank == 5 ) {slq_request += " WHERE stomps_id="+String(idcopy+42*4)+";";}
  if (bank == 6 ) {slq_request += " WHERE stomps_id="+String(idcopy+42*5)+";";}
  db_exec(db_base, slq_request.c_str());
  sqlite3_close(db_base);
  Serial.println(slq_request);
}

void Save_scenes() {
  if (l == 1) { //Data Read Only
    slq_request = "UPDATE stomps SET ";
    for (i = 1; i < 59; i++){
      if (i < 58){
      slq_request += "'"+String(i)+"'="+String(all_parameters[j][i])+", "; 
      } 
      if (i == 58){
      slq_request += "'"+String(i)+"'="+String(all_parameters[j][i]);
      }
    }
    slq_request += " WHERE stomps_id="+String(idcopy)+";";
    Serial.println(slq_request);
  }
  if (l == 2) { // Copy Data Main -> 6 prochains
    if (db_open("/spiffs/base.db", &db_base)) return;
    slq_request = "UPDATE stomps SET ";
    for (i = 1; i < 59; i++){
      if (i < 58){
      slq_request += "'"+String(i)+"'="+String(all_parameters[j][i])+", ";all_data_base_parameters_copy[j][i] = all_parameters[j][i];
      } 
      if (i == 58){
      slq_request += "'"+String(i)+"'="+String(all_parameters[j][i]); all_data_base_parameters_copy[j][i] = all_parameters[j][i];
      }
    }
    slq_request += " WHERE stomps_id="+String(idcopy)+";";
    db_exec(db_base, slq_request.c_str());
    sqlite3_close(db_base);
    Serial.println(slq_request);
  }
}

void readData (){
  data_base_choice = 0;
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
    slq_request = "SELECT * FROM stomps WHERE stomps_id = " + String(o) + ";";
    db_exec(db_base, slq_request.c_str());
    o = o + 1;
  }
  sqlite3_close(db_base);
}
// ------------------------------------------FIN GESTTION BASE DE DONNEE

// ---------------------------------------------------------------ECRANS

#include"RecupPathScene.h"
#include"CopyPathScene.h"

void inti_Encoders(int val1) {
  switch (val1)  {
    case 1 :
      left_encoder.setCount(all_parameters[id][ampero_quick_values[1][ampero_count]]);
      last_value_left_encoder = all_parameters[id][ampero_quick_values[1][ampero_count]];
      right_encoder.setCount(all_parameters[id][tonex_quick_values[1][tonex_count]]);
      last_value_right_encoder = all_parameters[id][tonex_quick_values[1][tonex_count]];
    break;
    case 2 :
      left_encoder.setCount(global_count);
      last_value_left_encoder = global_count;
    break;
    case 3 :
      left_encoder.setCount(all_parameters[id][global_count]);
      last_value_left_encoder = all_parameters[id][global_count];
      right_encoder.setCount(all_parameters[id][global_count]);
      last_value_right_encoder = all_parameters[id][global_count];
    break;
    case 4 :
      right_encoder.setCount(all_parameters[id][global_count]);
      last_value_right_encoder = all_parameters[id][global_count];
    break;
    case 5 :
      right_encoder.setCount(all_parameters[id][global_count]); 
      last_value_right_encoder = all_parameters[id][global_count];
    break;
    case 6 :
      right_encoder.setCount(all_parameters[id][global_count]); 
      last_value_right_encoder = all_parameters[id][global_count];
    break;
    case 7 :
      left_encoder.setCount(encoder1_copy_source);
      last_value_left_encoder = encoder1_copy_source;
      right_encoder.setCount(encoder2_copy_destination);
      last_value_right_encoder = encoder2_copy_destination;
    break;
  }
}

void encoder_1_moved(int valmini, int valmaxi, int pot, byte sel){
  switch (sel) {
    case 0 :
      pot = left_encoder.getCount();
      if (pot > valmaxi) {pot = valmaxi;}
      if (pot < valmini) { pot = valmini; }
      if ( ampero_count < 5 ) {
        midiblabla(ampero_quick_values[0][ampero_count], pot, canal_midi_2);
        all_parameters[id][ampero_quick_values[1][ampero_count]] = pot;
      }
      else {
        if (pot == 1299) {
          midiblabla(ampero_quick_values[0][ampero_count], 0, canal_midi_2); 
          all_parameters[id][ampero_quick_values[1][ampero_count]] = pot;
        }
        if (pot == 1300) {
          midiblabla(ampero_quick_values[0][ampero_count], 127, canal_midi_2);
          all_parameters[id][ampero_quick_values[1][ampero_count]] = pot;
        }
        Serial.println(pot);
      }
      left_encoder.setCount(pot);
      last_value_left_encoder = pot;
      Screens(2, all_parameters[id][ampero_quick_values[1][ampero_count]]);
    break;
    case 1 :
      pot = left_encoder.getCount();
      if (pot > valmaxi) {pot = valmini;}
      if (pot < valmini) {pot = valmaxi;}       
      global_count = pot;
      left_encoder.setCount(pot);
      last_value_left_encoder = pot;
      Screens(1, all_parameters[id][global_count]);
    break;
    case 2 :
      pot = left_encoder.getCount();
      if (pot > valmaxi) {pot = valmini;}
      if (pot < valmini) {pot = valmaxi;}        
      encoder1_copy_source = pot;
      left_encoder.setCount(pot);
      last_value_left_encoder = pot;
      Screens(7, encoder1_copy_source);
    break;
    case 3 :
      pot = left_encoder.getCount();
      if (pot > valmaxi) {pot = valmini;}
      if (pot < valmini) {pot = valmaxi;}        
      encoder1_copy_source = pot;
      left_encoder.setCount(pot);
      last_value_left_encoder = pot;
      CopyPathScene ();
    break;
  }
  previousMillis = millis(); tmp2 = 1;
}

void encoder_2_moved(int valmini, int valmaxi, int pot, byte sel){
  switch (sel) {
    case 0 :
      pot = right_encoder.getCount();
      if (pot > valmaxi) {pot = valmaxi;}
      if (pot < valmini) {pot = valmini;}
      midiblabla(tonex_quick_values[0][tonex_count], pot, canal_midi_1);
      Screens(3, pot);
      all_parameters[id][tonex_quick_values[1][tonex_count]] = pot;
      right_encoder.setCount(pot);
      last_value_right_encoder = pot;
    break;
    case 1 :
      pot = right_encoder.getCount();
      if (pot > valmaxi) {pot = valmaxi;}
      if (pot < valmini) {pot = valmini;}
      if (global_count == 55) {
        program_change(tonex_bank_choice, pot, canal_midi_1);
        all_parameters[id][global_count] = pot;
      }
      if (global_count == 56) {
        program_change(ampero_bank_choice, pot, canal_midi_2);
        all_parameters[id][global_count] = pot;
        }
      if (global_count != 55 or global_count != 56 or global_count != 54){ // Suppression reglage 54
        all_parameters[id][global_count] = pot;
        if (all_parameters[id][global_count] == 1399 or all_parameters[id][global_count] == 1400){
          if (all_parameters[id][global_count] == 1399){
            midiblabla(global_control_change[global_count], 0, canal_midi_1);
          }
          else {
            midiblabla(global_control_change[global_count], 127, canal_midi_1);
          }
        }
        if (global_count > 17 && global_count < all_parameters[id][54]) {
          if (all_parameters[id][global_count] == 1299){
            midiblabla(global_control_change[global_count], 0, canal_midi_1);
          }
          if (all_parameters[id][global_count] == 1300){
            midiblabla(global_control_change[global_count], 127, canal_midi_1);
          }
          if (all_parameters[id][global_count] < 128){
            midiblabla(global_control_change[global_count], pot, canal_midi_1);
          }
        }
        if (global_count > 0 && global_count < 18) {
          if (all_parameters[id][global_count] == 1299){
            midiblabla(global_control_change[global_count], 0, canal_midi_2);
          }
          if (all_parameters[id][global_count] == 1300){
            midiblabla(global_control_change[global_count], 127, canal_midi_2);
          }
          if (all_parameters[id][global_count] < 128){
            midiblabla(global_control_change[global_count], pot, canal_midi_2);
          }
        }
      }        
      right_encoder.setCount(pot);
      last_value_right_encoder = pot;
    break;
    case 2 :
      pot = right_encoder.getCount();
      if (pot > valmaxi) {pot = valmaxi;}
      if (pot < valmini) {pot = valmini;}
      all_parameters[id][42] = pot;
      right_encoder.setCount(pot);
      last_value_right_encoder = pot;
      typereverb();
    break;
    case 3 :
      pot = right_encoder.getCount();
      if (pot > valmaxi) {pot = valmini;}
      if (pot < valmini) {pot = valmaxi;}        
      encoder2_copy_destination = pot;
      right_encoder.setCount(pot);
      last_value_right_encoder = pot;
      Screens(8, encoder2_copy_destination);
    break;
    case 4 :
      pot = right_encoder.getCount();
      if (pot > valmaxi) {pot = valmini;}
      if (pot < valmini) {pot = valmaxi;}        
      encoder2_copy_destination = pot;
      right_encoder.setCount(pot);
      last_value_right_encoder = pot;
      RecupPathScene ();
    break;
  }
  previousMillis = millis(); tmp2 = 1;
}

void common_carroussel(){
  Screens(1, all_parameters[id][global_count]);
  static int encvalMax, encvalMini;
  while (menus == 1) {
    inti_Encoders(2);
    if (global_count == 55){inti_Encoders(4);}
    if (global_count == 56){inti_Encoders(5);}
    if (global_count != 55 && global_count !=56){
      inti_Encoders(6);
    }
    if (last_value_left_encoder != left_encoder.getCount()) {
      encoder_1_moved(1,56,global_count,1); 
    } 
    if (last_value_right_encoder != right_encoder.getCount()) {
      if (global_count != 55 && global_count != 56) {
        if (all_parameters[id][global_count] == 1399 or all_parameters[id][global_count] == 1400){
          if (all_parameters[id][global_count] == 1400) {
            tmp = 1400; 
            encvalMax = 1400; 
            encvalMini = 1399;
          }
          else {
            tmp = 1399; 
            encvalMax = 1400; 
            encvalMini = 1399;
          }
        }
        if (all_parameters[id][global_count] == 1299 or all_parameters[id][global_count] == 1300){
          if (all_parameters[id][global_count] == 1300) {
            tmp = 1300; 
            encvalMax = 1300; 
            encvalMini = 1299;
          }
          else {
            tmp = 1299; 
            encvalMax = 1300; 
            encvalMini = 1299;
          }
        }
        if (all_parameters[id][global_count] < 128){
          if (all_parameters[id][global_count] < 128) {
            tmp = -1; 
            encvalMax = global_control_change_max_values[global_count]; 
            encvalMini = 0; 
          }
        }
      } 
      else {
        if (all_parameters[id][55] < 128 && global_count == 55){
          tonex_bank_choice = 0;
        }
        if (all_parameters[id][56] < 128 && global_count == 56){
          ampero_bank_choice =  0;
        }
        if (all_parameters[id][55] > 127 && all_parameters[id][55] < 151 && global_count == 55){
          tonex_bank_choice = 1;
        }
        if (all_parameters[id][56] > 127 && all_parameters[id][56] < 257 && global_count == 56){
          ampero_bank_choice = 1;
        }
        if (all_parameters[id][56] > 255 && all_parameters[id][56] < 301 && global_count == 56){
          ampero_bank_choice = 2;
        }
        tmp = -1; encvalMax = global_control_change_max_values[global_count];
        encvalMini = 0; 
      }
    if (global_count != 42 && global_count != 54){
      encoder_2_moved(encvalMini,encvalMax,global_count,1);
    }
    if (global_count == 42){
      encoder_2_moved(0,global_control_change_max_values[global_count],global_count,2);
    }    
    Screens(1, tmp);
    }
    if (digitalRead(5) == 0) {
      delay(250);
      if (digitalRead(23) == 0) {
        Screens(10, 0);
        idcopy = id;
        saveData(); delay(250);
        ampero_count = 0; tonex_count = 0; 
        if(global_count<18) {
          initial_state = 2;
        } 
        else{
          initial_state = 3;
        } 
        break;
      }
      ampero_count = 0; tonex_count = 0; 
      if(global_count<18) {
        initial_state = 2;
      } 
      else{
        initial_state = 3;
        } 
      break;
    }
    if (digitalRead(23) == 0) {
      delay(250);
      if (digitalRead(5) == 0) {
        Screens(10, 0);
        idcopy = id;
        saveData(); delay(250);
        ampero_count = 0; tonex_count = 0; 
        if(global_count<18) {
          initial_state = 2;
        } 
        else{
          initial_state = 3;
        } 
      break;
      }
      ampero_count = 0; tonex_count = 0; 
      if(global_count<18) {
        initial_state = 2;
      }
      else{
        initial_state = 3;
      }
      break;
    }
  }
  previousMillis = millis(); tmp2 = 1;
}

void pressed_boutton(byte boutton_choiced, byte menu_choiced) {
  switch (boutton_choiced) {
    case 1 :
      switch (menu_choiced) {
        case 0 :
          if (initial_state == 1){
            ampero_count = 0; initial_state = 0;
          }
          if (ampero_count > 0 && ampero_count < 6) {
            ampero_count = ampero_count+1;
          }
          else {
            ampero_count = 1;
            tonex_count = 1;
          }
          inti_Encoders(1);
          if (all_parameters[id][ampero_quick_values[1][ampero_count]] > 1390){
            if (all_parameters[id][ampero_quick_values[1][ampero_count]] == 1399) {
              tmp = 1399;
              }
            else {
              tmp = 1400;
            }
          }
          if (all_parameters[id][ampero_quick_values[1][ampero_count]] > 1290 && all_parameters[id][ampero_quick_values[1][ampero_count]] < 1305) {
            if (all_parameters[id][ampero_quick_values[1][ampero_count]] == 1300) {
              tmp = 1300;
            }
            else {
              tmp = 1299;
            }
          } 
          if (all_parameters[id][ampero_quick_values[1][ampero_count]] < 1000) {
            tmp = -1;
          }
          if (startscreen != 1) {
            Screens(2, tmp);
          }
          else {
            startscreen = 0;
          }
          //displayColor1(rgbcolor[ampero_count]);
          break;
        case 1 : 
          // Screens(1, all_parameters[id][global_count]);
          global_count = 1;
          common_carroussel();
        break;
      }
    break;
    case 2 :
      switch (menu_choiced) {
        case 0 : 
          if (initial_state == 1){
            tonex_count = 0; 
            initial_state = 0;
          }
          if (tonex_count > 0 && tonex_count < 6) {
            tonex_count = tonex_count+1;
          }
          else {tonex_count = 1; ampero_count = 1;}
          inti_Encoders(1);
          if (all_parameters[id][tonex_quick_values[1][tonex_count]] > 1390){
            if (all_parameters[id][tonex_quick_values[1][tonex_count]] == 1399) {
              tmp = 1399;
            }
            else {
              tmp = 1400;
            }
          }
          if (all_parameters[id][tonex_quick_values[1][tonex_count]] > 1290 && all_parameters[id][tonex_quick_values[1][tonex_count]] > 1305) {
              if (all_parameters[id][tonex_quick_values[1][tonex_count]] == 1300) {
                tmp = 1300;
              }
              else {
                tmp = 1299;
              }
            }
          if (all_parameters[id][tonex_quick_values[1][tonex_count]] > 1000) {
            tmp = -1;
          }
          if (startscreen != 1) {
            Screens(3, tmp);
          } else {
            startscreen = 0;
          }
          // displayColor1(rgbcolor[tonex_count]);
        break;
        case 1 :
          global_count = 18;
          common_carroussel();
        break;
      }
    break;
  }
  previousMillis = millis(); tmp2 = 1;
}

void Save_patch_to_selected_patch(){
  Screens(11,0);
  //int j, k, l;
  l = id_values_for_6_foot[encoder1_copy_source];
  k = id_values_for_6_foot[encoder2_copy_destination];
  for (k ; k < id_values_for_6_foot[encoder2_copy_destination]+7; k++ && l++) {
    if (db_open("/spiffs/base.db", &db_base)) return;
    slq_request = "UPDATE stomps SET ";
    for (i = 1; i < 60; i++){
      if (i < 58){
        slq_request += "'"+String(i)+"'="+String(all_parameters[l][i])+", ";
      }
      if (i == 58){
        slq_request += "'"+String(i)+"'="+String(all_parameters[l][i]); 
      }
    }
    slq_request += " WHERE stomps_id="+String(k)+";";
    db_exec(db_base, slq_request.c_str());
    sqlite3_close(db_base);
    Serial.println(slq_request);
  }
  Screens(5,0);
  //delay(800);
  //esp_restart();
  readData();
}

void init_copy_patch(){
  tmp = 1;
  encoder1_copy_source = 1;
  encoder2_copy_destination = 2;
  inti_Encoders(7);
  while (menus == 2){
    if (tmp == 1) {
      Screens(6, 0);
      CopyPathScene ();
      Screens(8,0); 
      tmp = 0;
    }
    if (last_value_left_encoder != left_encoder.getCount()) {
      encoder_1_moved(1,42,encoder1_copy_source,3);
    } 
    if (last_value_right_encoder != right_encoder.getCount()) {
      encoder_2_moved(1,36,encoder2_copy_destination,3);
    } 
    if (digitalRead(5) == 0) {
      delay(250);
      if (digitalRead(23) == 0) {
        Save_patch_to_selected_patch();
      }
      ampero_count = 0; tonex_count = 0;
      initial_state = 2;
      break;
    }
    if (digitalRead(23) == 0) {
      delay(250);
      if (digitalRead(5) == 0) {
        Save_patch_to_selected_patch();
      }
      ampero_count = 0; tonex_count = 0; 
      initial_state = 3;
      break;
    }
  }
  previousMillis = millis(); tmp2 = 1;
}
                                      //    1 2  3  4  5  6   7  8  9 10 11 12  13 14 15  16  17  18   19  20  21  22  23  24   25  26  27  28  29  30   31  32  33  34  35  36 
//static int id_values_for_6_foot [37] = {0,1,8,15,22,29,36, 43,50,57,64,71,78, 85,92,99,106,113,120, 127,134,141,148,155,162, 169,176,183,190,197,204, 211,218,225,232,239,246};

void CopyDataScene () {
  Screens(14,0);
  if (db_open("/spiffs/base.db", &db_base)) return;
  slq_request = "UPDATE stomps SET ";
  for (i = 1; i < 60; i++){
    if (i < 58){
      slq_request += "'"+String(i)+"'="+String(all_parameters[encoder1_copy_source][i])+", ";
    }
    if (i == 58){
      slq_request += "'"+String(i)+"'="+String(all_parameters[encoder1_copy_source][i]); 
    }
  }
  slq_request += " WHERE stomps_id="+String(encoder2_copy_destination)+";";
  db_exec(db_base, slq_request.c_str());
  sqlite3_close(db_base);
  Serial.println(slq_request);
  Screens(5,0);
  readData();
}

void Init_copy_scenes(){
  int ct = 1;
  tmp = 1;
  encoder1_copy_source = id;
  encoder2_copy_destination = 1;
  inti_Encoders(7);
  while (menus == 2){
    server.handleClient(); 
    if (tmp == 1) {
      Screens(12, 0); 
      CopyPathScene ();
      RecupPathScene ();
      tmp = 0;
    }
    if (last_value_left_encoder != left_encoder.getCount()) {
      encoder_1_moved(1,42,encoder1_copy_source,3);
    } 
    if (last_value_right_encoder != right_encoder.getCount()) {
      encoder_2_moved(1,252,encoder2_copy_destination,4);
    } 
    if (digitalRead(5) == 0) {
      delay(250);
      if (digitalRead(23) == 0) {
        CopyDataScene();
      }
      ampero_count = 0; tonex_count = 0;
      initial_state = 2;
      break;
    }
    if (digitalRead(23) == 0) {
      delay(250);
      if (digitalRead(5) == 0) {
        CopyDataScene();
      }
      ampero_count = 0; tonex_count = 0; 
      initial_state = 3;
      break;
    }
  }
  previousMillis = millis(); tmp2 = 1;
}

void program_change_value(){
  if (all_parameters[id][55] < 128 or all_parameters[id][56] < 128){ampero_bank_choice = 0; tonex_bank_choice = 0;}
  if (all_parameters[id][55] > 127 && all_parameters[id][55] < 151){tonex_bank_choice = 1;}
  if (all_parameters[id][56] > 127 && all_parameters[id][56] < 257){ampero_bank_choice = 1;}
  if (all_parameters[id][56] > 255 && all_parameters[id][56] < 301){ampero_bank_choice = 2;}
}

void first_load(){
  program_change(tonex_bank_choice, all_parameters[id][55], canal_midi_1);
  delay(100);
  program_change(ampero_bank_choice,  all_parameters[id][56], canal_midi_2);
  delay(100);
  for (i = 18; i < all_parameters[id][54]; i++){
    if (all_parameters[id][i] == 1399) {MIDI.sendControlChange(global_control_change[i], 0, canal_midi_1);}
    if (all_parameters[id][i] == 1400) {MIDI.sendControlChange(global_control_change[i], 127, canal_midi_1);}
    if (all_parameters[id][i] == 1299) {MIDI.sendControlChange(global_control_change[i], 0, canal_midi_1);}
    if (all_parameters[id][i] == 1300) {MIDI.sendControlChange(global_control_change[i], 127, canal_midi_1);}
    if (i == 42) {typereverb();}
    else {if (all_parameters[id][i] < 128) {MIDI.sendControlChange(global_control_change[i], all_parameters[id][i], canal_midi_1);}}
    delay(15);
  }
  for (i = 1; i < 18; i++){
    if (i < 14) {
      if (all_parameters[id][i] == 1299) {MIDI.sendControlChange(global_control_change[i], 0, canal_midi_2);}
      else {MIDI.sendControlChange(global_control_change[i], 127, canal_midi_2);}
    if (i > 13) {MIDI.sendControlChange(global_control_change[i], all_parameters[id][i], canal_midi_2);}
    delay(5);
    }
  }
}

void load_Ampero() {
  if (all_parameters[id][i] == 1299) {MIDI.sendControlChange(global_control_change[i], 0, canal_midi_2);}
  if (all_parameters[id][i] == 1300) {MIDI.sendControlChange(global_control_change[i], 127, canal_midi_2);}
  if (i > 13) {MIDI.sendControlChange(global_control_change[i], all_parameters[id][i], canal_midi_2);}
  //Serial.print(i);Serial.print("="); Serial.print(all_parameters[id][i]);Serial.print("|");
}

void load_toneX() {
  if (all_parameters[id][i] == 1399) {MIDI.sendControlChange(global_control_change[i], 0, canal_midi_1);}
  if (all_parameters[id][i] == 1400) {MIDI.sendControlChange(global_control_change[i], 127, canal_midi_1);}
  if (all_parameters[id][i] == 1299) {MIDI.sendControlChange(global_control_change[i], 0, canal_midi_1);}
  if (all_parameters[id][i] == 1300) {MIDI.sendControlChange(global_control_change[i], 127, canal_midi_1);}
  if (all_parameters[id][i] < 128) {if (i != 42) {MIDI.sendControlChange(global_control_change[i], all_parameters[id][i], canal_midi_1);}}
  if (i == 42) {global_count = 42; typereverb();}
  delay(15);
  //Serial.print(i);Serial.print("="); Serial.print(all_parameters[id][i]);Serial.print("|");
}

void load_preset() {
  byte tmp5 = 0;
  if (all_parameters[id][56] != all_parameters[preid][56]) {program_change(ampero_bank_choice,  all_parameters[id][56], canal_midi_2); }
  if (all_parameters[id][55] != all_parameters[preid][55]) {program_change(tonex_bank_choice, all_parameters[id][55], canal_midi_1); }
  
  for (i = 1; i < 18; i++){
    if (all_parameters[id][56] != all_parameters[preid][56]) {
      load_Ampero();
      tmp5 = 1;
    }
    if (all_parameters[id][i] != all_data_base_parameters_copy[preid][i] && tmp5 == 0) {
      load_Ampero(); 
      tmp5 = 0;
    }
  }
  tmp5 = 0;
  //Serial.print(" // "); 
  if (all_parameters[id][55] != all_parameters[preid][55]) {program_change(tonex_bank_choice, all_parameters[id][55], canal_midi_1); }
  delay(30);
  for (i = 18; i < 46; i++){
    if (all_parameters[id][55] != all_parameters[preid][55]) {load_toneX(); tmp5 = 1;}
    if (all_parameters[id][i] != all_data_base_parameters_copy[preid][i] && tmp5 == 0) {load_toneX(); tmp5 = 0;}
  }
  //Serial.println("");
}

void Select() {
  program_change_value();
  if (initial_state == 1) {readData(); program_change_value(); first_load(); old_bank = bank;}
  if (bank != old_bank) {
    Screens(9, 0); 
    readData(); 
    old_bank = bank; line1_lcd_text = "toto"; line2_lcd_text = "toto"; 
    Screens(2, 0);}
}

void handleControlChange(byte channel, byte number, byte value) {
  if (channel == 12){bank = value; Select(); program_change_value(); first_load();}
  //Serial.print("Bank = ");Serial.println(bank);
  if (channel == 16) {
    choiced_foot_on_pacer = 1;
    preid = id; 
    id_init = value; id = value;
    ampero_count = 0; tonex_count = 0; menus = 0;
    Select(); 
    load_preset();
    pressed_boutton(1, menus);
    if (id < 8) {choiced_foot_on_pacer = 1;}
    if (id > 7 && id < 14) {choiced_foot_on_pacer = 2;}
    if (id > 13 && id < 21) {choiced_foot_on_pacer = 3;}
    if (id > 20 && id < 28) {choiced_foot_on_pacer = 4;}
    if (id > 27 && id < 35) {choiced_foot_on_pacer = 5;}
    if (id > 34 && id < 43) {choiced_foot_on_pacer = 6;}
    stomp = 0;
    Screens(4, 0);
    tmp2 = 0; line1_lcd_text = "toto"; line2_lcd_text = "toto";
  }

  if (channel == 15) {
    preid = id;
    stomp = value;
    id = id_init + value;
    ampero_count = 0; tonex_count = 0; menus = 0;
    load_preset();
    pressed_boutton(1, menus);
    Screens(4, 0);
    tmp2 = 0; line1_lcd_text = "toto"; line2_lcd_text = "toto";
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

  button7.attachClick([] () {if(menus == 0) {boutton_choice = 1; pressed_boutton(1, menus);}});
  button8.attachClick([] () {if(menus == 0) {boutton_choice = 2; pressed_boutton(2, menus);}});

  button7.attachLongPressStop([] () {line2_lcd_text = "toto"; menus = 1; boutton_choice = 1; pressed_boutton(1, menus);});
  button8.attachLongPressStop([] () {line2_lcd_text = "toto"; menus = 1; boutton_choice = 2; pressed_boutton(2, menus);});

  button7.attachDoubleClick([] () {line2_lcd_text, line1_lcd_text = "toto"; menus = 2; Init_copy_scenes();});
  button8.attachDoubleClick([] () {line2_lcd_text, line1_lcd_text = "toto"; menus = 2; init_copy_patch();});

  if(SPIFFS.begin(true)){
    File root = SPIFFS.open("/"); bank = 1; id = 1, id_init = id; initial_state = 1; choiced_foot_on_pacer = 1; Select(); preid = 0;}
  else {Serial.println("SPIFFS marche pas");} 

  ESP32Encoder::useInternalWeakPullResistors=NONE;
  left_encoder.attachSingleEdge(15, 4);
  right_encoder.attachSingleEdge(18, 19);
  left_encoder.clearCount();
  left_encoder.setFilter(1023);
  right_encoder.clearCount();
  right_encoder.setFilter(1023);
    
  //previousMillis = millis();

  inti_Encoders(1);

  line2_lcd_text, line1_lcd_text = "toto";
  pressed_boutton(1, menus);
  pressed_boutton(2, menus);
  previousMillis = millis();
  tmp2 = 1;

  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid2, password2);
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print(".");
  }
  Serial.println(".");
  Serial.println("Wifi connecté");
  ip = WiFi.localIP();
  server.on("/",              HomePage);
  server.on("/download", File_Download);
  server.begin();

  LCD.clear();
  LCD.setCursor(7,0);
  LCD.print("IP");
  LCD.setCursor(2,1);
  LCD.print(ip);
  delay(5000);
}
//----------------------------------------------------------------------------------END SETUP

void loop(){
  button7.tick();
  button8.tick();
  if (last_value_left_encoder != left_encoder.getCount()) {
    if (ampero_count == 0) {
      ampero_count = 1;
    }
    encoder_1_moved(values_mini_ampero_quick[ampero_count],values_max_ampero_quick[ampero_count],all_parameters[id][ampero_quick_values[1][ampero_count]],0);
    line2_lcd_text, line1_lcd_text = "toto";
  }  
  if (last_value_right_encoder != right_encoder.getCount()) {
    if (tonex_count == 0) {
      tonex_count = 1;
    }
    if (tonex_count != 0) {
      encoder_2_moved(values_mini_tonex_quick[tonex_count],values_max_tonex_quick[tonex_count],all_parameters[id][tonex_quick_values[1][tonex_count]],0);
    }
    line2_lcd_text, line1_lcd_text = "toto";
  }
  MIDI.read();
  if (initial_state == 2) {
    initial_state = 0;
    menus = 0; 
    line1_lcd_text = "toto"; 
    line2_lcd_text = "toto"; 
    pressed_boutton(1, menus); 
    Screens(2, 0);
  }  
  if (initial_state == 3) {
    initial_state = 0; 
    menus = 0; 
    line1_lcd_text = "toto"; 
    line2_lcd_text = "toto"; 
    pressed_boutton(2, 
    menus); 
    Screens(3, 0);
  }
  if (millis() - previousMillis >= interval && tmp2 == 1) {
    Screens(4, 0); 
    tmp2 = 0; 
    line1_lcd_text = "toto"; 
    line2_lcd_text = "toto";
  }
}

