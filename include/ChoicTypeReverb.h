void ChoixTypeReverb() {
  if (all_parameters[id][42] >= 0 && all_parameters[id][42] < 2) {midiblabla(global_control_change[42], 10, canal_midi_1);}
  if (all_parameters[id][42] >= 2 && all_parameters[id][42] < 4) {midiblabla(global_control_change[42], 20, canal_midi_1);}
  if (all_parameters[id][42] >= 4 && all_parameters[id][42] < 6) {midiblabla(global_control_change[42], 55, canal_midi_1);}
  if (all_parameters[id][42] >= 6 && all_parameters[id][42] < 8) {midiblabla(global_control_change[42], 85, canal_midi_1);}
  if (all_parameters[id][42] >= 8 && all_parameters[id][42] < 10) {midiblabla(global_control_change[42], 115, canal_midi_1);}
  //Serial.println(42);
}