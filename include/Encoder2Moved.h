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
        if (global_count > 17 && global_count < 59) {
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
      ChoixTypeReverb();
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