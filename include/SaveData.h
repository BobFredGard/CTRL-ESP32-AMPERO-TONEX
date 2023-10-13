void saveData() {
  if (db_open("/spiffs/base.db", &db_base)) return;
    slq_request = "UPDATE 'stomps' SET 'stomps_id'="+String(idcopy)+", ";
  if (bank == 1 ) {slq_request = "UPDATE 'stomps' SET 'stomps_id'="+String(idcopy)+", ";}
  if (bank == 2 ) {slq_request = "UPDATE 'stomps' SET 'stomps_id'="+String(idcopy + 42)+", ";}
  if (bank == 3 ) {slq_request = "UPDATE 'stomps' SET 'stomps_id'="+String(idcopy + 42 * 2)+", ";}
  if (bank == 4 ) {slq_request = "UPDATE 'stomps' SET 'stomps_id'="+String(idcopy + 42 * 3)+", ";}
  if (bank == 5 ) {slq_request = "UPDATE 'stomps' SET 'stomps_id'="+String(idcopy + 42 * 4)+", ";}
  if (bank == 6 ) {slq_request = "UPDATE 'stomps' SET 'stomps_id'="+String(idcopy + 42 * 5)+", ";}
  for (i = 1; i < 59; i++){
    if (i < 58){
      //if (all_parameters[idcopy][i] != all_data_base_parameters_copy[idcopy][i]) {
        slq_request += "'"+String(i)+"'="+String(all_parameters[idcopy][i])+", "; 
      //}
    }
    if (i == 58){
        slq_request += "'"+String(i)+"'="+String(all_parameters[idcopy][i]); 
    }
  }
  if (bank == 1 ) {slq_request += " WHERE stomps_id="+String(idcopy)+";";}
  if (bank == 2 ) {slq_request += " WHERE stomps_id="+String(idcopy+42)+";";}
  if (bank == 3 ) {slq_request += " WHERE stomps_id="+String(idcopy+42*2)+";";}
  if (bank == 4 ) {slq_request += " WHERE stomps_id="+String(idcopy+42*3)+";";}
  if (bank == 5 ) {slq_request += " WHERE stomps_id="+String(idcopy+42*4)+";";}
  if (bank == 6 ) {slq_request += " WHERE stomps_id="+String(idcopy+42*5)+";";}
  db_exec(db_base, slq_request.c_str());
  sqlite3_close(db_base);
  Serial.println(slq_request);
}