static byte calculPathScene [3] [43]{
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42},
    {0,1,2,3,4,5,6,7,1,2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7},
    {0,1,1,1,1,1,1,1,2,2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6}
};


void CalculPath (byte cpt) {
    if (calculPathScene  [0] [encoder2_copy_destination - cpt] == encoder2_copy_destination - cpt) {
        tx2 = calculPathScene [2] [calculPathScene [0] [encoder2_copy_destination - cpt]];
        tx3 = calculPathScene [1] [calculPathScene [0] [encoder2_copy_destination - cpt]];
    }
    else {
        tx3 = calculPathScene [1] [calculPathScene [0] [encoder2_copy_destination - cpt]];
    }
}

void RecupPathScene () {
    String text2;
    //Serial.println(encoder2_copy_destination);
    if (encoder2_copy_destination < 43) {
        tx1 = 1;
        CalculPath (0);
    }
    if (encoder2_copy_destination > 42 && encoder2_copy_destination < 85) {
        tx1 = 2;
        CalculPath (42);
    }
    if (encoder2_copy_destination > 84 && encoder2_copy_destination < 127) {
        tx1 = 3;
        CalculPath (42*2);
    }
    if (encoder2_copy_destination > 126 && encoder2_copy_destination < 169) {
        tx1 = 4;
        CalculPath (42*3);
    }
    if (encoder2_copy_destination > 168 && encoder2_copy_destination < 211) {
        tx1 = 5;
        CalculPath (42*4);
    }
    if (encoder2_copy_destination > 210 && encoder2_copy_destination < 253) {
        tx1 = 6;
        CalculPath (42*5);
    }
    
    LCD.setCursor(11,1);
    text2 = String(tx1) + "-" + String(tx2) + "-" + String(tx3);
    LCD.print(text2);
}