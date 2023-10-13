void CopyDataScene () {
  Screens(14,0);
  if (db_open("/spiffs/base.db", &db_base)) return;
  Serial.print("ID source = "); Serial.println(encoder1_copy_source);
  Serial.print("ID copie  = "); Serial.println(encoder2_copy_destination);
  slq_request = "UPDATE 'stomps' SET 'stomps_id'="+String(encoder2_copy_destination)+", ";
  for (i = 1; i < 59; i++){
    if (i < 58){
      slq_request += "'"+String(i)+"'="+String(all_parameters[encoder1_copy_source][i])+", ";
    }
    if (i == 58){
      slq_request += "'"+String(i)+"'="+String(all_parameters[encoder1_copy_source][i]); 
    }
  }
  slq_request += " WHERE stomps_id="+String(encoder2_copy_destination)+";";
  db_exec(db_base, slq_request.c_str());
  sqlite3_close(db_base);
  Serial.println(slq_request);
  Screens(5,0);
  readData();
}