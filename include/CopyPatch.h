                                      //    1 2  3  4  5  6   7  8  9 10 11 12  13 14 15  16  17  18   19  20  21  22  23  24   25  26  27  28  29  30   31  32  33  34  35  36 
//static int id_values_for_6_foot [37] = {0,1,8,15,22,29,36, 43,50,57,64,71,78, 85,92,99,106,113,120, 127,134,141,148,155,162, 169,176,183,190,197,204, 211,218,225,232,239,246};

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