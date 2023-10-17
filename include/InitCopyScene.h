                  //    1 2  3  4  5  6   7  8  9 10 11 12  13 14 15  16  17  18   19  20  21  22  23  24   25  26  27  28  29  30   31  32  33  34  35  36 
//static int  [37] = {0,1,8,15,22,29,36, 43,50,57,64,71,78, 85,92,99,106,113,120, 127,134,141,148,155,162, 169,176,183,190,197,204, 211,218,225,232,239,246};

void InitCopyScene(){
  int ct = 1;
  tmp = 1;
  encoder1_copy_source = id;
  encoder2_copy_destination = 1;
  init_Encoders(7);
  while (menus == 2){
    //server.handleClient(); 
    if (tmp == 1) {
      Screens(12, 0); 
      CopyPathScene ();
      RecupPathScene ();
      tmp = 0;
    }
    if (last_value_left_encoder != left_encoder.getCount()) {
      encoder_1_moved(1,42,encoder1_copy_source,4);
    } 
    if (last_value_right_encoder != right_encoder.getCount()) {
      encoder_2_moved(1,252,encoder2_copy_destination,4);
    } 
    if (digitalRead(5) == 0) {
      delay(250);
      if (digitalRead(23) == 0) {
        CopyDataScene();
      }
      ampero_count = 0; tonex_count = 0;
      initial_state = 2;
      break;
    }
    if (digitalRead(23) == 0) {
      delay(250);
      if (digitalRead(5) == 0) {
        CopyDataScene();
      }
      ampero_count = 0; tonex_count = 0; 
      initial_state = 3;
      break;
    }
  }
  previousMillis = millis(); tmp2 = 1;
}