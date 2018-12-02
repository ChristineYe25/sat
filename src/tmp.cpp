void collect_buffer(int pos_cls[NUM_VARS][BUF_SIZE], int neg_cls[NUM_VARS][BUF_SIZE], 
  const int var, const int x){
   if (var> 0){
      assert(pos_cls[var][4]>0);
      if (pos_cls[var][0] == 0){
        pos_cls[var][0] = x; 
      }else if (pos_cls[var][1] == 0){
        pos_cls[var][1] = x; 
      }else if (pos_cls[var][2] == 0){
        pos_cls[var][2] = x; 
      }else if (pos_cls[var][3] == 0){
        pos_cls[var][3] = x; 
      }else {
        pos_cls[var][4] = x; 
      }
    }else{
      assert(neg_cls[var][4]>0);
      if (neg_cls[-var][0] == 0){
        neg_cls[-var][0] = x; 
      }else if (neg_cls[-var][1] == 0){
        neg_cls[-var][1] = x; 
      }else if (neg_cls[-var][2] == 0){
        neg_cls[-var][2] = x; 
      }else if (neg_cls[-var][3] == 0){
        neg_cls[-var][3] = x; 
      }else { 
        neg_cls[-var][4] = x; 
      }
    }
}
