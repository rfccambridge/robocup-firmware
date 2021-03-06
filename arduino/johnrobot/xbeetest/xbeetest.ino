#define VERBOSE true
  
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


class XBEEComms {
  public:
    XBEEComms(): current_ttl_time(0), last_cmd_time(0) {}
    int current_command[3];
    String last_command;
    unsigned long current_ttl_time;  // time to live of last command recieved (milliseconds)
    unsigned long last_cmd_time;     // timestamp of last command (milliseconds)
    
    void update(String &last_command) {
      
      char buf[BUF_SZ];
      last_command.toCharArray(buf, sizeof(buf));
      
      int v[3];
      char *p = buf;
      char *tmp;
      int id = String(strtok_r(p, ",", &p)).toInt();
      for (int i = 0; i < 3; i++){
        tmp = strtok_r(p, ",", &p);
        v[i] = String(tmp).toInt();
      }
      int ttl = String(strtok_r(p, ",", &p)).toInt();
      
      // update timing so we know when to stop
      current_ttl_time = (unsigned long) ttl;
      if (VERBOSE) {
        Serial.print("Currently recorded ttl: ");
        Serial.print(current_ttl_time);
        Serial.print(" Current time: ");
        Serial.println(millis());
      }
      last_cmd_time = millis();
      memcpy(current_command, v, sizeof(int) * 3);
    }

    void get_command(int* cmd) {
      // gets the current valid command, or else return zeros.
      if (millis() > (current_ttl_time + last_cmd_time)) {
        int no_command[3] = {0, 0, 0};
        memcpy(cmd, no_command, sizeof(int) * 3);
        return;
      } else {
        memcpy(cmd, current_command, sizeof(int) * 3);
        return;
      }
    }
};


XBEEComms *comms;

  
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

  comms = new XBEEComms();
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
  int y = 0.707 * (v[0] + v[1]);
  result[0] = -x + d * v[2];
  result[1] = y + d * v[2];
  result[2] = x + d * v[2];
  result[3] = -y + d * v[2];
}


void loop() {

  if (Serial1.available()) {
    String receivedStr = Serial1.readStringUntil('\n');
    comms->update(receivedStr);
  }

  int v[3], transformed_v[4];
  comms->get_command(v);
  if (v[0] == 0 && v[1] == 0 && v[2] == 0){
    stopAll();
  } else {
    transformation(v, transformed_v);
    makeMove(transformed_v);
  }

  
   /*
   if (Serial1.available())
  { // If data comes in from XBee, send it out to serial monitor
    time_last_command = millis();
    
    String receivedStr = Serial1.readStringUntil('\n');
    if (VERBOSE) {
      Serial.print("Received message: ");
      Serial.println(receivedStr);
    }
    char buf[BUF_SZ];
    receivedStr.toCharArray(buf, sizeof(buf));

    int v[3];
    int transformed_v[4];
    char *p = buf;
    char *tmp;
    int id = String(strtok_r(p, ",", &p)).toInt();
    for (int i = 0; i < 3; i++){
      tmp = strtok_r(p, ",", &p);
      v[i] = String(tmp).toInt();
    }

    if (VERBOSE) {
      Serial.print("Parsed to: ");
      char debugMsg[30];
      sprintf(debugMsg, "%d, %d, %d", v[0], v[1], v[2]);
      Serial.print("ID ");
      Serial.print(id);
      Serial.print(" MoveCommand: ");
      Serial.println(debugMsg);
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

  // don't keep running if we don't have commands within 500 ms
  if (millis() - time_last_command > 500) {
    stopAll();
 }
 */
}
