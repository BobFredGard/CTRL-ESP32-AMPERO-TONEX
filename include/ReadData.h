void readData (){
  data_base_choice = 0;
  if (db_open("/spiffs/base.db", &db_base)) return; 
  static byte o;
  Serial.print("Bank = "); Serial.println(bank);
  Serial.print("ID INIT = "); Serial.println(id_init);
  if (bank == 1 ) {o = 1;}
  if (bank == 2 ) {o = 1 + 42;}
  if (bank == 3 ) {o = 1 + 42*2;}
  if (bank == 4 ) {o = 1 + 42*3;}
  if (bank == 5 ) {o = 1 + 42*4;}
  if (bank == 6 ) {o = 1 + 42*5;}
  for (i = 1; i < 43; i++) {
    slq_request = "SELECT * FROM stomps WHERE stomps_id = " + String(o) + ";";
    db_exec(db_base, slq_request.c_str());
    o = o + 1;
  }
  sqlite3_close(db_base);
}