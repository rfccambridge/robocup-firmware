 
int ID = 5;
int speed1 = 20;
int BUF_SZ = 40;
int d = 1;
float theta = 75.0 / 2 * PI / 180;



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
}

void stopAll(){
  analogWrite(frspeed, 0);
  digitalWrite(fr1, LOW);
  digitalWrite(fr2, LOW);
   analogWrite(flspeed, 0);
  digitalWrite(fl1, LOW);
  digitalWrite(fl2, LOW);
   analogWrite(brspeed, 0);
  digitalWrite(br1, LOW);
  digitalWrite(br2, LOW);
   analogWrite(blspeed, 0);
  digitalWrite(bl1, LOW);
  digitalWrite(bl2, LOW);  
}

void makeMove(int* v){
  analogWrite(blspeed, min(abs(v[0]), 100));
  digitalWrite(bl1, (v[0] > 0) ? HIGH : LOW);
  digitalWrite(bl2, (v[0] < 0) ? HIGH : LOW);
  analogWrite(flspeed, min(abs(v[1]), 100));
  digitalWrite(fl1, (v[1] > 0) ? HIGH : LOW);
  digitalWrite(fl2, (v[1] < 0) ? HIGH : LOW);   
  analogWrite(frspeed, min(abs(v[2]), 100));
  digitalWrite(fr1, (v[2] > 0) ? HIGH : LOW);
  digitalWrite(fr2, (v[2] < 0) ? HIGH : LOW);
  analogWrite(brspeed, min(abs(v[3]), 100));
  digitalWrite(br1, (v[3] > 0) ? HIGH : LOW);
  digitalWrite(br2, (v[3] < 0) ? HIGH : LOW);
}

void transformation(int* v, int* result){
  int x = v[0];
  int y = v[1];
  result[0] = x * sin(theta) - y * cos(theta) + d * v[2];
  result[1] = -x * sin(theta) - y * cos(theta) + d * v[2];
  result[2] = -x * sin(theta) + y * cos(theta) + d * v[2];
  result[3] = x * sin(theta) + y * cos(theta) + d * v[2];
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
    if (id == -1 || id == ID) {
      if (v[0] == 0 && v[1] == 0 && v[2] == 0){
        stopAll();
      } else {
        transformation(v, transformed_v);
        makeMove(transformed_v);
      }
    }
  }
}
