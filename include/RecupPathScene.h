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
    text2 = String(tx1) + "-" + String(tx2) + "-" + String(tx3-1);
    LCD.print(text2);
}