
  int speed1 = 40;
  int BUF_SZ = 40;
  int d = 1;
void setup() {
  // put your setup code here, to run once:
 pinMode(8, OUTPUT); // 8 and 9 and back right 
  pinMode(9, OUTPUT);
   pinMode(7, OUTPUT); // 7 and 6 and front right
  pinMode(6, OUTPUT);
   pinMode(5, OUTPUT); // 5 and 4 and front left
  pinMode(4, OUTPUT);
   pinMode(3, OUTPUT); // 3 and 2 a dn back left
  pinMode(2, OUTPUT);
  analogWrite(7, 0);
  analogWrite(9, 0); 
  analogWrite(5, 0);    
  analogWrite(3, 0);   
   Serial1.begin(9600);
  Serial.begin(9600);
  //char receivedChar;
}
void goBackward(){
  analogWrite(7, speed1);
  digitalWrite(6, HIGH);
  analogWrite(9, speed1);
  digitalWrite(8, HIGH);  
  analogWrite(5, speed1);   
  digitalWrite(4, LOW);    
  analogWrite(3, speed1);   
  digitalWrite(2, LOW);  
}

void goForward(){
  analogWrite(7, speed1);
  digitalWrite(6, LOW);
  analogWrite(9, speed1);
  digitalWrite(8, LOW);  
  analogWrite(5, speed1);   
  digitalWrite(4, HIGH);    
  analogWrite(3, speed1);   
  digitalWrite(2, HIGH);  
}

void goLeft(){
  analogWrite(7, speed1);
  digitalWrite(6, LOW);
  analogWrite(9, speed1);
  digitalWrite(8, HIGH);  
  analogWrite(5, speed1);   
  digitalWrite(4, LOW);    
  analogWrite(3, speed1);   
  digitalWrite(2, HIGH);  
}


void goRight(){
  analogWrite(7, speed1);
  digitalWrite(6, HIGH);
  analogWrite(9, speed1);
  digitalWrite(8, LOW);  
  analogWrite(5, speed1);   
  digitalWrite(4, HIGH);    
  analogWrite(3, speed1);   
  digitalWrite(2, LOW);  
}

void goForwardRight(){
  analogWrite(7, 0);
  digitalWrite(6, HIGH);
  analogWrite(9, speed1*1.5);
  digitalWrite(8, LOW);  
  analogWrite(5, speed1*1.5);   
  digitalWrite(4, HIGH);    
  analogWrite(3, 0);   
  digitalWrite(2, LOW);
}

void goBackLeft(){
  analogWrite(7, 0);
  digitalWrite(6, HIGH);
  analogWrite(9, speed1*1.5);
  digitalWrite(8, LOW);  
  analogWrite(5, speed1*1.5);   
  digitalWrite(4, HIGH);    
  analogWrite(3, 0);   
  digitalWrite(2, LOW);  
}


void goBackRight(){
  analogWrite(7, speed1*1.5);
  digitalWrite(6, HIGH);
  analogWrite(9, 0);
  digitalWrite(8, LOW);  
  analogWrite(5, 0);   
  digitalWrite(4, HIGH);    
  analogWrite(3, speed1*1.5);   
  digitalWrite(2, LOW);  
}

void goForwardLeft(){
  analogWrite(7, speed1*1.5);
  digitalWrite(6, LOW);
  analogWrite(9, 0);
  digitalWrite(8, LOW);  
  analogWrite(5, 0);   
  digitalWrite(4, HIGH);    
  analogWrite(3, speed1*1.5);   
  digitalWrite(2, HIGH);  
}



/*
void goForward(){
  analogWrite(7, speed1);
  digitalWrite(6, LOW);
  analogWrite(9, speed1);
  digitalWrite(8, LOW);  
  analogWrite(5, speed1);   
  digitalWrite(4, HIGH);    
  analogWrite(3, speed1);   
  digitalWrite(2, HIGH);  
}*/

void goRotateRight(){
  analogWrite(7, speed1);
  digitalWrite(6, HIGH);
  analogWrite(9, speed1);
  digitalWrite(8, HIGH);  
  analogWrite(5, speed1);   
  digitalWrite(4, HIGH);    
  analogWrite(3, speed1);   
  digitalWrite(2, HIGH);  
}


void goRotateLeft(){
  analogWrite(7, speed1);
  digitalWrite(6, LOW);
  analogWrite(9, speed1);
  digitalWrite(8, LOW);  
  analogWrite(5, speed1);   
  digitalWrite(4, LOW);    
  analogWrite(3, speed1);   
  digitalWrite(2, LOW);     
}



void stopAll(){
  analogWrite(7, 0);
  digitalWrite(6, LOW);
  analogWrite(9, 0);
  digitalWrite(8, LOW);  
  analogWrite(5, 0);   
  digitalWrite(4, HIGH);    
  analogWrite(3, 0);   
  digitalWrite(2, HIGH);  
}

void makeMove(int* v){
  analogWrite(3, abs(v[0]));
  digitalWrite(2, (v[0] > 0) ? HIGH : LOW);
  analogWrite(5, abs(v[1]));
  digitalWrite(4, (v[1] > 0) ? HIGH : LOW);   
  analogWrite(7, abs(v[2]));
  digitalWrite(6, (v[2] > 0) ? HIGH : LOW);
  analogWrite(9, abs(v[3]));
  digitalWrite(8, (v[3] > 0) ? HIGH : LOW);
 
 
}

void transformation(int* v, int* result){
  int x = -0.707 * (v[0] + v[1]);
  int y = 0.707 * ( v[0] - v[1]);
  result[0] = y + d * v[2];
  result[1] = -1.0 * x + d * v[2];
  result[2] = -1.0 * y + d * v[2];
  result[3] = x + d * v[2];
}


void loop() {
   if (Serial1.available())
  { // If data comes in from XBee, send it out to serial monitor
    String receivedStr = Serial1.readStringUntil('\n');
    char buf[BUF_SZ];
    receivedStr.toCharArray(buf, sizeof(buf));
    int v[3];
    int transformed_v[4];
    char *p = buf;
    for (int i = 0; i < 3; i++){
      v[i] = String(strtok_r(p, ",", &p)).toInt() / 100;
    }
     Serial1.println(v[0]);
      Serial1.println(v[1]);
      Serial1.println(v[2]);
      Serial1.println("TRANSFORMED:");
    if (v[0] == 0 && v[1] == 0 && v[2] == 0){
      stopAll();
    } else {
      transformation(v, transformed_v);
      Serial1.println(transformed_v[0]);
      Serial1.println(transformed_v[1]);
      Serial1.println(transformed_v[2]);
      Serial1.println(transformed_v[3]);
      makeMove(transformed_v);
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
//  digitalWrite(6, HIGH);
//  analogWrite(9, 0);
//  digitalWrite(8, LOW);  
//  analogWrite(5, speed1);   
//  digitalWrite(4, HIGH);    
//  analogWrite(3, 0);   
//  digitalWrite(2, LOW);  
//  Serial.println(" backright "); 
  //delay(3000);
}
