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
      if (global_count == 57){
        if (line1_lcd_text != "*Stomp Val 1") {
          LCD.setCursor(5,0); line1_lcd_text = "*Stomp Val 1";
          LCD.print(line1_lcd_text);
        }
      }
      if (global_count == 58){
        if (line1_lcd_text != "*Stomp Val 2") {
          LCD.setCursor(5,0); line1_lcd_text = "*Stomp Val 2";
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
              ScaleValueSwitch(1);
            }
            if (global_count == 22 or global_count == 40 or global_count == 53) {
              ScaleValueSwitch(2);
            }
            if (global_count != 22 or global_count != 40 or global_count != 53 or valminimax[1][global_count] != 10 or global_count != 42) {
              LCD.print(all_parameters[id][global_count]);
            }
            if (global_count == 42) {
              ScaleValueSwitch(3);
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
            ScaleValueSwitch(1);
          }
          if (global_count == 22 or global_count == 40 or global_count == 53) {
            ScaleValueSwitch(2);
          }
          if (global_count != 22 or global_count != 40 or global_count != 53 or valminimax[1][global_count] != 10 or global_count != 42) {
            LCD.print(all_parameters[id][global_count]);
          }
          if (global_count == 42) {
            ScaleValueSwitch(3);
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