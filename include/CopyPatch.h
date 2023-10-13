void SelectIdDestination (int cpt) {
    l = id_values_for_6_foot[encoder1_copy_source];
    //Serial.println(l);
}

void CopyPatch () {
    if (encoder1_copy_source == 1) {
        tx1 = 1;
    }
    if (encoder1_copy_source == 2) {
        tx1 = 2;
    }
    if (encoder1_copy_source == 3) {
        tx1 = 3;
    }
    if (encoder1_copy_source == 4) {
        tx1 = 4;
    }
    if (encoder1_copy_source == 5) {
        tx1 = 5;
    }
    if (encoder1_copy_source == 6) {
        tx1 = 6;
    }
    
    //Serial.println(encoder1_copy_source);
    LCD.setCursor(14,0);
    LCD.print(String(tx1));
}