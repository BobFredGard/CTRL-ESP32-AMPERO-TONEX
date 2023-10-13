static const char* daTa = "Callback function called";
char *zErrMsg = 0;

static int db_open(const char *filename, sqlite3 **db) {
  sqlite3_open(filename, db);
  return 0;
}

static int call(void *daTa, int argc, char **argv, char **azColName) {
  static int m; String test = "Val = ";
    for (m = 0; m<argc; m++){
      String val = argv[m];
      String val2 = azColName[m];
      switch (data_base_choice){
        case 0 :
          all_parameters[i][m] = val.toInt(); all_data_base_parameters_copy [i][m] = val.toInt();
          //test += String(all_parameters[i][m]) + ", ";
        break;
        case 1 :
        break;
      } 
    }
  //Serial.println(test);
  id = 1;
  return 0;
}

static int db_exec(sqlite3 *db, const char *slq_request) {
  sqlite3_exec(db, slq_request, call, (void*)daTa, &zErrMsg);
  return 0;
}