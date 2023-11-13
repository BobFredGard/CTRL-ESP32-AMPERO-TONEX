#include <Arduino.h>

#include <Parameters.h>
#include <MIDIParameters.h>

#include <Wire.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C LCD(0x3F,16,2); 

#include <SPIFFS.h>

/*#include <WiFi.h>
const char *ssid2 = "HUAWEI-B715-A231";
const char *password2 = "**********";
IPAddress ip;

#include "Network.h"
#include "Sys_Variables.h"
#include "CSS.h"

#include <ESP32WebServer.h>
ESP32WebServer server(80);
#include <ESPmDNS.h>
#include <DataServer.h>*/

#include <sqlite3.h>
sqlite3 *db_base;

#include <OneButton.h>
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
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial2, MIDI, Serial2MIDISettings);
#define MIDI_CHANNEL_OMNI 0

#include "ScaleValueSwitch.h"
#include "DataMIDISent.h"
#include "ChoicTypeReverb.h"
#include "Screens.h"
#include "InitBD.h"
#include "SaveData.h"
#include "ReadData.h"
#include "RecupPathScene.h"
#include "CopyPathScene.h" 
#include "CopyPatch.h"
#include "InitEncoder.h"
#include "Encoder1Moved.h"
#include "Encoder2Moved.h"
#include "CommonCaroussel.h"
#include "PressedBoutton1.h"
#include "PressedBoutton2.h"
#include "SavePatch.h"
#include "InitCopyPatch.h"
#include "SaveScene.h"
#include "InitCopyScene.h"
#include "LoadFirstPreset.h"
#include "LoadPreset.h"
#include "handlControlChange.h"

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

  button7.attachClick([] () {if(menus == 0) {boutton_choice = 1; PressedBoutton1();}});
  button8.attachClick([] () {if(menus == 0) {boutton_choice = 2; PressedBoutton2();}});

  button7.attachLongPressStop([] () {line2_lcd_text = "toto"; menus = 1; boutton_choice = 1; global_count = 1; CommonCaroussel();});
  button8.attachLongPressStop([] () {line2_lcd_text = "toto"; menus = 1; boutton_choice = 2; global_count = 18; CommonCaroussel();});

  button7.attachDoubleClick([] () {line2_lcd_text, line1_lcd_text = "toto"; menus = 2; InitCopyScene();});
  button8.attachDoubleClick([] () {line2_lcd_text, line1_lcd_text = "toto"; menus = 2; InitCopyPatch();});

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

  init_Encoders(1);

  line2_lcd_text, line1_lcd_text = "toto";
  PressedBoutton1();
  PressedBoutton2();
  previousMillis = millis();
  tmp2 = 1;

  /*WiFi.mode(WIFI_AP_STA);
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
  delay(5000);*/
}

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
    PressedBoutton1(); 
    Screens(2, 0);
  }  
  if (initial_state == 3) {
    initial_state = 0; 
    menus = 0; 
    line1_lcd_text = "toto"; 
    line2_lcd_text = "toto"; 
    PressedBoutton2(); 
    Screens(3, 0);
  }
  if (millis() - previousMillis >= interval && tmp2 == 1) {
    Screens(4, 0); 
    tmp2 = 0; 
    line1_lcd_text = "toto"; 
    line2_lcd_text = "toto";
  }
}

