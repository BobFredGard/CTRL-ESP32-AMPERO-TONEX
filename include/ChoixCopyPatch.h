void ChoixCopyPatch () {
    byte tmp3, tmp4;
    String text3;
    LCD.setCursor(13,1);
    if (encoder2_copy_destination < 7) {tmp3 = 1;} 
    if (encoder2_copy_destination > 6 && encoder2_copy_destination < 13) {tmp3 = 2;} 
    if (encoder2_copy_destination > 12 && encoder2_copy_destination < 19) {tmp3 = 3;} 
    if (encoder2_copy_destination > 18 && encoder2_copy_destination < 25) {tmp3 = 4;}
    if (encoder2_copy_destination > 24 && encoder2_copy_destination < 31) {tmp3 = 5;} 
    if (encoder2_copy_destination > 30 && encoder2_copy_destination < 37) {tmp3 = 6;}
    if (encoder2_copy_destination == 1 or encoder2_copy_destination == 7  or encoder2_copy_destination == 13 or encoder2_copy_destination == 19 or encoder2_copy_destination == 25 or encoder2_copy_destination == 31) {tmp4 = 1;}
    if (encoder2_copy_destination == 2 or encoder2_copy_destination == 8  or encoder2_copy_destination == 14 or encoder2_copy_destination == 20 or encoder2_copy_destination == 26 or encoder2_copy_destination == 32) {tmp4 = 2;}
    if (encoder2_copy_destination == 3 or encoder2_copy_destination == 9  or encoder2_copy_destination == 15 or encoder2_copy_destination == 21 or encoder2_copy_destination == 27 or encoder2_copy_destination == 33) {tmp4 = 3;}
    if (encoder2_copy_destination == 4 or encoder2_copy_destination == 10 or encoder2_copy_destination == 16 or encoder2_copy_destination == 22 or encoder2_copy_destination == 28 or encoder2_copy_destination == 34) {tmp4 = 4;}
    if (encoder2_copy_destination == 5 or encoder2_copy_destination == 11 or encoder2_copy_destination == 17 or encoder2_copy_destination == 23 or encoder2_copy_destination == 29 or encoder2_copy_destination == 35) {tmp4 = 5;}
    if (encoder2_copy_destination == 6 or encoder2_copy_destination == 12 or encoder2_copy_destination == 18 or encoder2_copy_destination == 24 or encoder2_copy_destination == 30 or encoder2_copy_destination == 36) {tmp4 = 6;}
    text3 = String(tmp3) + "-" + String(tmp4);
    LCD.print(text3);
}