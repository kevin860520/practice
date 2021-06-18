int col[8]={8,1,2,3,4,5,6,7};//設定led對應的腳位pin
int line[8]={0,9,11,15,12,13,14,10};
int appear[8][8]={0};//設定點亮的LED,0為暗,1為亮
int snack[64]={0};//設定蛇的位置0~63表示位置%8得到X座標(col) , /8得到Y座標(line)
int Length=2;//設定蛇長度
int x_unit = 0b00000001;//設定x單位為1
int y_unit = 0b00001000;//設定y單位為8
int new_Direction = 0;//設定新方向
int Direction = 0;//設定方向
int a=0;//暫存蛇尾位置,若吃到食物則蛇尾繼續亮
int food = -1;//設定食物位置
int dead=0;//判斷死亡
void setup() {
  for(int i = 0 ; i < 8 ; i++){
    pinMode(line[i],OUTPUT);
    pinMode(col[i],OUTPUT);
    digitalWrite(line[i] , HIGH);
    digitalWrite(col[i] , LOW);//所有led設定變暗
  }
  for(int i = 0 ; i < 64 ; i++){
    snack[i] = 100;           //設定所有蛇身位置為100,超過63則不顯示
  }
  snack[0]=62;//初始蛇頭座標(6,7)
  snack[1]=63;//初始蛇身(7,7)
}

void loop() {
  int forward , backward , upward , downward ;
  for(int i = 0 ; i < 64 ; i++){
    if(snack[i]<64){
      appear[snack[i]/8][snack[i]%8]=1;//設定蛇為1,後面點亮
    }
  }

  while(food == -1){                  //若食物不存在則隨機定下個食物
    food = random(64);                
    for(int i = 0 ; i < 64 ;i++){
      if(food == snack[i]){             //與蛇重疊,重新隨機
        food = -1;
      }
    }
  }
  
  appear[food/8][food%8]=1;           //點亮食物
  
  for(int i = 0 ; i < 500 ;){         //i < 500表示共延遲500毫秒,蛇移動一次500毫秒
    forward = analogRead(16);
    backward = analogRead(17);
    upward = analogRead(18);
    downward = analogRead(19);        //500毫秒內持續讀取各方向按鈕電壓
    for(int j = 0 ; j < 8 ; j++){
      for(int k = 0 ; k < 8 ; k++){
        if(appear[j][k]==1){
            digitalWrite(line[j] , LOW); 
            digitalWrite(col[k] , HIGH);
            delay(1);                 //使應亮位置上的led亮1毫秒,然後亮下一個led
            i++;                      //每亮一次讓i跟著+1,直到500為止
            digitalWrite(line[j] , HIGH); 
            digitalWrite(col[k] , LOW);
        }
      }
    }
    if(forward > 200){
     new_Direction = 1;
    }
    else if(backward > 200){
     new_Direction = 0;
    }
    else if(upward > 200){
     new_Direction = 2;
    }
    else if(downward > 200){ //4個方向判斷有電壓就改走向,向前為1,向後為0,向上為2,向下為3
      new_Direction = 3;
    }
  }
  
  a=snack[Length-1];         //記錄下蛇尾位置,若有吃到食物時,則蛇尾繼續亮
  
  for(int i = 0 ; i < 64 ; i++){  
    
    appear[snack[i]/8][snack[i]%8]=0; //暫時把所有位置led清0
    
  }
  
  for(int i = 0 ; i < 63 ; i++){
    if(snack[63-i] < 64){
      snack[63-i] = snack[63-1-i];  //把除蛇頭以外的所有位置往前一個節點位置紀錄
    }
  }
  
  switch(new_Direction){
    case 0:
      if(Direction == 1){
        snack[0]++;
        new_Direction = 1;           //若原走向與新走向反向,則繼續原走向(蛇往前時不能直接回頭)
      }
      else{
        snack[0]--;
      }
      break;
    case 1:
      if(Direction == 0){
        snack[0]--;
        new_Direction = 0;
      }
      else{
        snack[0]++;
      }
      break;
    case 2:
      if(Direction == 3){
        snack[0] = snack[0] + y_unit;
        new_Direction = 3;
      }
      else{
        snack[0] = snack[0] - y_unit;
      }
      break;
    case 3:
      if(Direction == 2){
        snack[0] = snack[0] - y_unit;
        new_Direction = 2;
      }
      else{
        snack[0] = snack[0] + y_unit;
      }
      break;
    default:
      break;
   }
   
   Direction = new_Direction;         //完成後把新走向紀錄為下一次的原走向
   
   if(snack[0] == food){
    snack[Length]=a;
    Length++;
    food = -1;                        //若吃到食物,長度+1,食物重置,把紀錄的原尾巴位置還原
   }
   
   for(int i = 1 ; i < 64 ; i++){     
     if(snack[0] == snack[i]){        //吃到蛇身則死
       dead++;
    }
   }
   if(snack[0] > 63){                 //超出範圍則死(上界)
    dead++;
  }
  if(snack[0] < 0){                   //超出範圍則死(下界)
    dead++;
  }
  if(((snack[0]-snack[1]) == 1)&&(snack[0]/8) != (snack[1]/8)){
    dead++;                           //超出範圍則死(左界)
  }
  if(((snack[0]-snack[1]) == -1)&&(snack[0]/8) != (snack[1]/8)){
    dead++;                           //超出範圍則死(右界)
  }
  if(dead > 0){
    for(int i = 0 ; i < 8 ; i++){
    digitalWrite(line[i] , LOW);      //死掉時,所有led設定變亮
    digitalWrite(col[i] , HIGH);
    }
    while(1);                         //不繼續其他動作
  }
}
