void PressedBoutton1 () {
    if (initial_state == 1){
        ampero_count = 0; initial_state = 0;
    }
        if (ampero_count > 0 && ampero_count < 6) {
        ampero_count = ampero_count+1;
    }
    else {
        ampero_count = 1;
        tonex_count = 1;
    }
        init_Encoders(1);
    if (all_parameters[id][ampero_quick_values[1][ampero_count]] > 1390){
        if (all_parameters[id][ampero_quick_values[1][ampero_count]] == 1399) {
            tmp = 1399;
            }
        else {
            tmp = 1400;
        }
    }
    if (all_parameters[id][ampero_quick_values[1][ampero_count]] > 1290 && all_parameters[id][ampero_quick_values[1][ampero_count]] < 1305) {
        if (all_parameters[id][ampero_quick_values[1][ampero_count]] == 1300) {
        tmp = 1300;
        }
        else {
            tmp = 1299;
        }
    } 
    if (all_parameters[id][ampero_quick_values[1][ampero_count]] < 1000) {
        tmp = -1;
    }
    if (startscreen != 1) {
        Screens(2, tmp);
    }
    else {
        startscreen = 0;
    }
    //displayColor1(rgbcolor[ampero_count]);
}