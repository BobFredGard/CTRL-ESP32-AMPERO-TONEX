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
    //choiced_foot_on_pacer = 1;
    preid = id; 
    id_init = value; id = value;
    ampero_count = 0; tonex_count = 0; menus = 0;
    Select(); 
    load_preset();
    PressedBoutton1();
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
  if (channel == 15 && value != 6) {
    preid = id;
    stomp = value;
    id = id_init + value;
    ampero_count = 0; tonex_count = 0; menus = 0;
    load_preset();
    PressedBoutton1();
    Screens(4, 0);
    tmp2 = 0; line1_lcd_text = "toto"; line2_lcd_text = "toto";
  }
  if (channel == 15 && value == 6) {
    if (tmpstomp2 == 1299) {
      MIDI.sendControlChange(global_control_change[all_parameters[id][58]], 127, canal_midi_2);
      tmpstomp2 = 1300;
    }
    else {
      MIDI.sendControlChange(global_control_change[all_parameters[id][58]], 0, canal_midi_2);
      tmpstomp2 = 1299;
    }
  }
}