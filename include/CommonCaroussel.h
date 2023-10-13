void CommonCaroussel () {Screens(1, all_parameters[id][global_count]);
  static int encvalMax, encvalMini;
  while (menus == 1) {
    init_Encoders(2);
    if (global_count == 55){init_Encoders(4);}
    if (global_count == 56){init_Encoders(5);}
    if (global_count != 55 && global_count !=56 && global_count !=57 && global_count !=58){
      init_Encoders(6);
    }
    if (last_value_left_encoder != left_encoder.getCount()) {
      encoder_1_moved(1,58,global_count,1); 
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
    if (global_count == 57 or global_count == 58){
      encoder_2_moved(1,12,global_count,1);
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