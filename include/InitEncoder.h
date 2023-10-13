void init_Encoders(int val1) {
  switch (val1)  {
    case 1 :
      left_encoder.setCount(all_parameters[id][ampero_quick_values[1][ampero_count]]);
      last_value_left_encoder = all_parameters[id][ampero_quick_values[1][ampero_count]];
      right_encoder.setCount(all_parameters[id][tonex_quick_values[1][tonex_count]]);
      last_value_right_encoder = all_parameters[id][tonex_quick_values[1][tonex_count]];
    break;
    case 2 :
      left_encoder.setCount(global_count);
      last_value_left_encoder = global_count;
    break;
    case 3 :
      left_encoder.setCount(all_parameters[id][global_count]);
      last_value_left_encoder = all_parameters[id][global_count];
      right_encoder.setCount(all_parameters[id][global_count]);
      last_value_right_encoder = all_parameters[id][global_count];
    break;
    case 4 :
      right_encoder.setCount(all_parameters[id][global_count]);
      last_value_right_encoder = all_parameters[id][global_count];
    break;
    case 5 :
      right_encoder.setCount(all_parameters[id][global_count]); 
      last_value_right_encoder = all_parameters[id][global_count];
    break;
    case 6 :
      right_encoder.setCount(all_parameters[id][global_count]); 
      last_value_right_encoder = all_parameters[id][global_count];
    break;
    case 7 :
      left_encoder.setCount(encoder1_copy_source);
      last_value_left_encoder = encoder1_copy_source;
      right_encoder.setCount(encoder2_copy_destination);
      last_value_right_encoder = encoder2_copy_destination;
    break;
  }
}