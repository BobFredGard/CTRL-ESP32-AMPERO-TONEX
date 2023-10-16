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
  if (all_parameters[id][i] < 128) {
    if (i != 42) {
      MIDI.sendControlChange(global_control_change[i], all_parameters[id][i], canal_midi_1);
    }
  }
  if (i == 42) {
    global_count = 42;
    ChoixTypeReverb();
  }
  delay(16);
  //Serial.print(i);Serial.print("="); Serial.print(all_parameters[id][i]);Serial.print("|");
}

void load_preset() {
  if (all_parameters[id][58] == 1299) {
    tmpstomp2 = 1299;
  }
  else {
    tmpstomp2 = 1300;
  }
  
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

  if (all_parameters[id][58] == tmpstomp2) {
    if (tmpstomp2 == 1299) {
      MIDI.sendControlChange(global_control_change[58], 0, canal_midi_2);
    }
    else {
      MIDI.sendControlChange(global_control_change[58], 127, canal_midi_2);
    }
  }

  tmp5 = 0;
  //Serial.print(" // "); 
  if (all_parameters[id][55] != all_parameters[preid][55]) {program_change(tonex_bank_choice, all_parameters[id][55], canal_midi_1); }
  delay(50);
  for (i = 18; i < 46; i++){
    if (all_parameters[id][55] != all_parameters[preid][55]) {
      load_toneX();
      tmp5 = 1;
    }
    if (all_parameters[id][i] != all_data_base_parameters_copy[preid][i] && tmp5 == 0) {
      load_toneX();
      tmp5 = 0;
    }
  }
  
  //Serial.println("");
}