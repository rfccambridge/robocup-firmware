  int speed1 = 20;
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



void loop() {
  goForwardLeft();
  Serial.println(" backleft "); 
  delay(3000);
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
  delay(3000);
}
