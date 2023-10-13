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
      CopyPatch ();
    break;
    case 4 :
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
