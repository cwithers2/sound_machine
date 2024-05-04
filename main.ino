#include <SoftwareSerial.h>

#define PIN_RX    6
#define PIN_TX    5

#define PIN_POT   A1
#define PIN_BUT   2

#define CMD_BEG   0x7e
#define CMD_END   0xef
#define CMD_VER   0xff
#define CMD_LEN   0x06
#define CHK_LEN   0x02

void checksum(uint8_t, uint8_t*, uint8_t*);
void command();
void update_volume();
void check_button();

SoftwareSerial mySerial(PIN_RX, PIN_TX); // RX, TX

void checksum(uint8_t cmd_len, uint8_t* cmd_data, uint8_t* chk_data){
  uint16_t checksum = 0;
  //this is the algorithm the knock-off dfplayers use:
  for(int i = 0; i < cmd_len; ++i)
    checksum -= cmd_data[i];
  //serialize the checksum
  chk_data[0] = (uint8_t)(checksum>>8);
  chk_data[1] = (uint8_t)(checksum&0xff);  
}

void command(uint8_t cmd, uint8_t hb, uint8_t lb, bool reply = false){
  uint8_t cmd_data[CMD_LEN] = { CMD_VER, CMD_LEN, cmd, reply, hb, lb };
  uint8_t chk_data[CHK_LEN];
  checksum(CMD_LEN, cmd_data, chk_data);
  mySerial.write(CMD_BEG);
  mySerial.write(cmd_data, CMD_LEN);
  mySerial.write(chk_data, CHK_LEN);
  mySerial.write(CMD_END);
  delay(50);
}

void update_volume(){
  static int last = 0;
  int vol = map(analogRead(PIN_POT), 0, 1023, 30, 0);
  if(abs(last - vol) > 1){
    if(vol == 1)//increase the dead-zone for cheap pots
      --vol;
    Serial.println("Change vol");
    command(0x06, 0, vol, false);
    last = vol;
  }
}

void check_button(){
  static int last = HIGH;
  int state = digitalRead(PIN_BUT);
  if(last == LOW && state == HIGH){
    Serial.println("Next track");
    command(0x01, 0, 0, false); //next track
    command(0x08, 2, 0, false); //loop track
  }
  last = state;
}

void setup(){
  pinMode(PIN_BUT, INPUT_PULLUP);
  Serial.begin(115200); //for console debugging
  mySerial.begin(9600); //communicate with DF Player
  delay(3000); //Allow serials to start
  Serial.println("Hello, world!");
  command(0x11, 0, 1, false); // enable looping
}

void loop(){
  update_volume();
  check_button();
}
