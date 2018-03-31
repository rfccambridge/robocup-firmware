
  int ID = 4;
  int speed1 = 20;
  int BUF_SZ = 40;
  int d = 1;


  int br1 = 8;
  int br2 = 7;
  int brspeed = 10;
  int fr1 = 4;
  int fr2 = 2;
  int frspeed = 9;
  int fl1 = 51;
  int fl2 = 49; 
  int flspeed = 45;
  int bl1 = 43; // changed from 42
  int bl2 = 42;
  int blspeed = 46;
  
void setup() {

  
  // put your setup code here, to run once:
 pinMode(br1, OUTPUT); // 8 and 9 and back right 
  pinMode(br2, OUTPUT);
   pinMode(fr1, OUTPUT); // 7 and 6 and front right
  pinMode(fr2, OUTPUT);
   pinMode(fl1, OUTPUT); // 5 and 4 and front left
  pinMode(fl2, OUTPUT);
   pinMode(bl1, OUTPUT); // 3 and 2 a dn back left
  pinMode(bl2, OUTPUT);

  pinMode(frspeed, OUTPUT);
  pinMode(brspeed, OUTPUT);
  pinMode(flspeed, OUTPUT);
  pinMode(blspeed, OUTPUT);
  analogWrite(frspeed, 0);
  analogWrite(brspeed, 0); 
  analogWrite(flspeed, 0);    
  analogWrite(blspeed, 0);   
   Serial2.begin(9600);
  Serial.begin(9600);
  //char receivedChar;
}
void goBackward(){
  analogWrite(fr1, speed1);
  digitalWrite(fr2, HIGH);
  analogWrite(br2, speed1);
  digitalWrite(br1, HIGH);  
  analogWrite(fl1, speed1);   
  digitalWrite(fl2, LOW);    
  analogWrite(bl1, speed1);   
  digitalWrite(bl2, LOW);  
}

void goForward(){
  analogWrite(fr1, speed1);
  digitalWrite(fr2, LOW);
  analogWrite(br2, speed1);
  digitalWrite(br1, LOW);  
  analogWrite(fl1, speed1);   
  digitalWrite(fl2, HIGH);    
  analogWrite(bl1, speed1);   
  digitalWrite(bl2, HIGH);  
}

void goLeft(){
  analogWrite(fr1, speed1);
  digitalWrite(fr2, LOW);
  analogWrite(br2, speed1);
  digitalWrite(br1, HIGH);  
  analogWrite(fl1, speed1);   
  digitalWrite(fl2, LOW);    
  analogWrite(bl1, speed1);   
  digitalWrite(bl2, HIGH);  
}


void goRight(){
  analogWrite(fr1, speed1);
  digitalWrite(fr2, HIGH);
  analogWrite(br2, speed1);
  digitalWrite(br1, LOW);  
  analogWrite(fl1, speed1);   
  digitalWrite(fl2, HIGH);    
  analogWrite(bl1, speed1);   
  digitalWrite(bl2, LOW);  
}

void goForwardRight(){
  analogWrite(fr1, 0);
  digitalWrite(fr2, HIGH);
  analogWrite(br2, speed1*1.5);
  digitalWrite(br1, LOW);  
  analogWrite(fl1, speed1*1.5);   
  digitalWrite(fl2, HIGH);    
  analogWrite(bl1, 0);   
  digitalWrite(bl2, LOW);
}

void goBackLeft(){
  analogWrite(fr1, 0);
  digitalWrite(fr2, HIGH);
  analogWrite(br2, speed1*1.5);
  digitalWrite(br1, LOW);  
  analogWrite(fl1, speed1*1.5);   
  digitalWrite(fl2, HIGH);    
  analogWrite(bl1, 0);   
  digitalWrite(bl2, LOW);  
}


void goBackRight(){
  analogWrite(fr1, speed1*1.5);
  digitalWrite(fr2, HIGH);
  analogWrite(br2, 0);
  digitalWrite(br1, LOW);  
  analogWrite(fl1, 0);   
  digitalWrite(fl2, HIGH);    
  analogWrite(bl1, speed1*1.5);   
  digitalWrite(bl2, LOW);  
}

void goForwardLeft(){
  analogWrite(fr1, speed1*1.5);
  digitalWrite(fr2, LOW);
  analogWrite(br2, 0);
  digitalWrite(br1, LOW);  
  analogWrite(fl1, 0);   
  digitalWrite(fl2, HIGH);    
  analogWrite(bl1, speed1*1.5);   
  digitalWrite(bl2, HIGH);  
}



/*
void goForward(){
  analogWrite(fr1, speed1);
  digitalWrite(fr2, LOW);
  analogWrite(br2, speed1);
  digitalWrite(br1, LOW);  
  analogWrite(fl1, speed1);   
  digitalWrite(4, HIGH);    
  analogWrite(bl1, speed1);   
  digitalWrite(2, HIGH);  
}*/

void goRotateRight(){
  analogWrite(frspeed, speed1);
  digitalWrite(fr2, HIGH);
  digitalWrite(fr1, LOW);
  analogWrite(brspeed, speed1);
  digitalWrite(br1, LOW);
  digitalWrite(br2, HIGH);  
  analogWrite(flspeed, speed1);   
  digitalWrite(fl1, LOW);
  digitalWrite(fl2, HIGH);
  analogWrite(blspeed, speed1);   
  digitalWrite(bl2, LOW);  
  digitalWrite(bl1, HIGH);
}


void goRotateLeft(){
  analogWrite(frspeed, speed1);
  digitalWrite(fr2, LOW);
  digitalWrite(fr1, HIGH);
  analogWrite(brspeed, speed1);
  digitalWrite(br1, HIGH);
  digitalWrite(br2, LOW);  
  analogWrite(flspeed, speed1);   
  digitalWrite(fl2, HIGH);
  digitalWrite(fl1, LOW);
  analogWrite(blspeed, speed1);   
  digitalWrite(bl2, HIGH);  
  digitalWrite(bl1, LOW);  
}



void stopAll(){
  analogWrite(fr1, 0);
  digitalWrite(fr2, LOW);
  analogWrite(br2, 0);
  digitalWrite(br1, LOW);  
  analogWrite(fl1, 0);   
  digitalWrite(fl2, LOW);    
  analogWrite(bl1, 0);   
  digitalWrite(bl2, LOW);  
}

void makeMove(int* v){
//Serial.print(v[0], DEC);    

  analogWrite(blspeed, min(abs(v[0]), 255));
  digitalWrite(bl1, (v[0] > 0) ? LOW : HIGH);
  digitalWrite(bl2, (v[0] > 0) ? HIGH : LOW);
  analogWrite(flspeed, min(abs(v[1]), 255));
  digitalWrite(fl2, (v[1] > 0) ? LOW : HIGH);
  digitalWrite(fl1, (v[1] > 0) ? HIGH : LOW);   
  analogWrite(frspeed, min(abs(v[2]), 255));
  digitalWrite(fr1, (v[2] > 0) ? LOW : HIGH);
  digitalWrite(fr2, (v[2] > 0) ? HIGH : LOW);
  analogWrite(brspeed, min(abs(v[3]), 255));
  digitalWrite(br1, (v[3] > 0) ? LOW : HIGH);
  digitalWrite(br2, (v[3] > 0) ? HIGH : LOW);
}

void transformation(int* v, int* result){
  int x = 0.707 * (v[0] - v[1]);
  int y = 0.707 * ( v[0] + v[1]);
  result[0] = -x + d * v[2];
  result[1] = y + d * v[2];
  result[2] = x + d * v[2];
  result[3] = -y + d * v[2];
}


void loop() {
//  goRotateRight();
   if (Serial2.available())
  { // If data comes in from XBee, send it out to serial monitor
    String receivedStr = Serial2.readStringUntil('\n');
    char buf[BUF_SZ];
    receivedStr.toCharArray(buf, sizeof(buf));
    int v[3];
    int transformed_v[4];
    char *p = buf;
    int id = String(strtok_r(p, ",", &p)).toInt();
    for (int i = 0; i < 3; i++){
      v[i] = String(strtok_r(p, ",", &p)).toInt();
    }
    Serial.print(id);
    Serial.print(v[0]);
    Serial.print(v[1]);
    Serial.print(v[2]);
    Serial.println();
//    Serial1.println("OHLOLOLO");
//    Serial1.println(v[0]);
//      Serial1.println(v[1]);
//      Serial1.println(v[2]);
//      Serial1.println("TRANSFORMED:");
    if (id == -1 || id == ID) {
      if (v[0] == 0 && v[1] == 0 && v[2] == 0){
        stopAll();
      } else {
        transformation(v, transformed_v);
        makeMove(transformed_v);
      }
    }
  }
    
    
//    if(receivedChar == 'c')
//    {
//      stopAll();
//    }
//    else if(receivedChar == 'w')
//    {
//      goForward();
//    }
//    else if(receivedChar == 's')
//    {
//      goBackward();
//    }
//    else if(receivedChar == 'a')
//    {
//      goLeft();
//    }
//    else if(receivedChar == 'd')
//    {
//      goRight();
//    }
//    else if(receivedChar == 'q')
//    {
//      goRotateLeft();
//    }
//    else if(receivedChar == 'e')
//    {
//      goRotateRight();
//    }
//  }

  
//  goForwardLeft();
//  Serial.println(" backleft "); 
//  delay(3000);
  // put your main code here, to run repeatedly:
//   analogWrite(7, 0);
//  digitalWrite(fr2, HIGH);
//  analogWrite(br2, 0);
//  digitalWrite(br1, LOW);  
//  analogWrite(fl1, speed1);   
//  digitalWrite(fl2, HIGH);    
//  analogWrite(3, 0);   
//  digitalWrite(2, LOW);  
//  Serial.println(" backright "); 
  //delay(3000);
}
