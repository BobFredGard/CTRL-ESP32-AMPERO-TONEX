void InitCopyPatch(){
  tmp = 1;
  encoder1_copy_source = choiced_foot_on_pacer;
  encoder2_copy_destination = 2;
  init_Encoders(7);
  while (menus == 2){
    if (tmp == 1) {
      Screens(6, 0);
      CopyPatch ();
      Screens(8, encoder2_copy_destination);
      tmp = 0;
    }
    if (last_value_left_encoder != left_encoder.getCount()) {
      encoder_1_moved(1,6,encoder1_copy_source,3);
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
