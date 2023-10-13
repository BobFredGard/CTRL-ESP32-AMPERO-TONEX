void Save_patch_to_selected_patch(){
  Screens(11,0);
  l = id_values_for_6_foot[encoder1_copy_source];
  k = id_values_for_6_foot[encoder2_copy_destination];
  for (k ; k < id_values_for_6_foot[encoder2_copy_destination]+7; k++ && l++) {
    Serial.print("ID copy = "); Serial.println(l);
    if (db_open("/spiffs/base.db", &db_base)) return;
    slq_request = "UPDATE 'stomps' SET 'stomps_id'="+String(k)+", ";
    for (i = 1; i < 59; i++){
      if (i < 58){
        slq_request += "'"+String(i)+"'="+String(all_parameters[l][i])+", ";
      }
      if (i == 58){
        slq_request += "'"+String(i)+"'="+String(all_parameters[l][i]); 
      }
    }
    slq_request += " WHERE stomps_id="+String(k)+";";
    db_exec(db_base, slq_request.c_str());
    sqlite3_close(db_base);
    Serial.println(slq_request);
    delay(100);
  }
  Screens(5,0);
  //delay(800);
  //esp_restart();
  readData();
}