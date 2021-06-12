int col[8]={8,1,2,3,4,5,6,7};
int line[8]={0,9,11,15,12,13,14,10};
int appear[8][8]={0};
int snack[64]={0};
int Length=2;
int x_unit = 0b00000001;
int y_unit = 0b00001000;
int Direction = 0;
int a=0;
int food = 65;
int dead=0;
void setup() {
  // put your setup code here, to run once:
  for(int i = 0 ; i < 8 ; i++){
    pinMode(line[i],OUTPUT);
    pinMode(col[i],OUTPUT);
    digitalWrite(line[i] , HIGH);//所有led設定變暗
    digitalWrite(col[i] , LOW);
  }
  for(int i = 0 ; i < 64 ; i++){
    snack[i] = 100;
  }
  snack[0]=63;
  snack[1]=62;
}

void loop() {
  // put your main code here, to run repeatedly:
  int forward , backward , upward , downward ;
  for(int i = 0 ; i < 64 ; i++){
    if(snack[i]<64){
      appear[snack[i]/8][snack[i]%8]=1;
    }
  }

  while(food>64){
    food = random(64);
    for(int i = 0 ; i < 64 ;i++){
      if(food==snack[i]){
        food = 65;
      }
    }
  }
  
  appear[food/8][food%8]=1;
  for(int i = 0 ; i < 700 ;){ 
    forward = analogRead(16);
    backward = analogRead(17);
    upward = analogRead(18);
    downward = analogRead(19);
    for(int j = 0 ; j < 8 ; j++){
      for(int k = 0 ; k < 8 ; k++){
        if(appear[j][k]==1){
            digitalWrite(line[j] , LOW); 
            digitalWrite(col[k] , HIGH);
            delay(1);
            i++;
            digitalWrite(line[j] , HIGH); 
            digitalWrite(col[k] , LOW);
        }
      }
    }
    if(forward > 200){
     Direction = 1;
    }
    else if(backward > 200){
     Direction = 0;
    }
    else if(upward > 200){
     Direction = 2;
    }
    else if(downward > 200){ //4個方向判斷有電壓就改走向
      Direction = 3;
    }
  }
  a=snack[Length-1];
  for(int i = 0 ; i < 63 ; i++){
    if(snack[i]<64){
      appear[snack[i]/8][snack[i]%8]=0;
    }
    if(snack[63-i] < 64){
      snack[63-i] = snack[63-1-i];
    }
  }
  appear[food/8][food%8]=0;
  switch(Direction){
    case 0:
      snack[0]--;
      break;
    case 1:
      snack[0]++;
      break;
    case 2:
      snack[0]=snack[0] - y_unit;
      break;
    case 3:
      snack[0]=snack[0] + y_unit;//透過此switch得到新蛇頭
      break;
    default:
      break;
   }
   if(snack[0] == food){
    snack[Length]=a;
    Length++;
    food = 65;
   }
   for(int i = 1 ; i < 64 ; i++){
    if(snack[0] == snack[i]){
       dead++;
    }
  }
  if(snack[0] > 63){
    dead++;
  }
  if(snack[0] < 0){
    dead++;
  }
  if(((snack[0]-snack[1])==1)&&(snack[0]/8)!=(snack[1]/8)){
    dead++;
  }
  if(((snack[0]-snack[1])==-1)&&(snack[0]/8)!=(snack[1]/8)){
    dead++;
  }
  if(dead > 0){
    for(int i = 0 ; i < 8 ; i++){
    digitalWrite(line[i] , LOW);//所有led設定變暗
    digitalWrite(col[i] , HIGH);
    }
    while(1);
  }
}
