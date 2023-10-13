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
  for (i = 18; i < 59; i++){
    if (all_parameters[id][i] == 1399) {MIDI.sendControlChange(global_control_change[i], 0, canal_midi_1);}
    if (all_parameters[id][i] == 1400) {MIDI.sendControlChange(global_control_change[i], 127, canal_midi_1);}
    if (all_parameters[id][i] == 1299) {MIDI.sendControlChange(global_control_change[i], 0, canal_midi_1);}
    if (all_parameters[id][i] == 1300) {MIDI.sendControlChange(global_control_change[i], 127, canal_midi_1);}
    if (i == 42) {ChoixTypeReverb();}
    else {if (all_parameters[id][i] < 128) {MIDI.sendControlChange(global_control_change[i], all_parameters[id][i], canal_midi_1);}}
    delay(15);
    if (i == 58) {
      if (all_parameters[id][58] == 1299) {
        tmpstomp2 = 0;
      }
      else {
        tmpstomp2 = 1;
      }
    }
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