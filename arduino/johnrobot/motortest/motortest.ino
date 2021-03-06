#define VERBOSE false
  
  int ID = 8;
  
  int speed1 = 40;
  int BUF_SZ = 40;
  int d = 1;

  unsigned long time_last_command = 0;

  int br1 = 8;
  int br2 = 9;
  int fr1 = 7;
  int fr2 = 6;
  int fl1 = 5;
  int fl2 = 4; 
  int bl1 = 3;
  int bl2 = 2;
  
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
  analogWrite(fr1, 0);
  analogWrite(br2, 0); 
  analogWrite(fl1, 0);    
  analogWrite(bl1, 0);   
   Serial1.begin(9600);
  Serial.begin(9600);
}

void stopAll(){
  analogWrite(fr1, 0);
  digitalWrite(fr2, LOW);
  analogWrite(br2, 0);
  digitalWrite(br1, LOW);  
  analogWrite(fl1, 0);   
  digitalWrite(fl2, HIGH);    
  analogWrite(bl1, 0);   
  digitalWrite(bl2, HIGH);  
}

void makeMove(int* v){
  analogWrite(bl1, min(abs(v[0]), 255));
  digitalWrite(bl2, (v[0] > 0) ? HIGH : LOW);
  analogWrite(fl1, min(abs(v[1]), 255));
  digitalWrite(fl2, (v[1] > 0) ? HIGH : LOW);   
  analogWrite(fr1, min(abs(v[2]), 255));
  digitalWrite(fr2, (v[2] > 0) ? HIGH : LOW);
  analogWrite(br2, min(abs(v[3]), 255));
  digitalWrite(br1, (v[3] > 0) ? HIGH : LOW);
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

    int v[3];
    int transformed_v[4];

    if ((millis() / 1000) % 2 == 0) {
      v[0] = 30;
    } else {
      v[0] = -30;
    }
    v[1] = 0;
    v[2] = 0; 
    transformation(v, transformed_v);
    makeMove(transformed_v);
}
