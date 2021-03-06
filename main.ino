
#include "ChainableLED.h"
#include "pitches.h"  //音符库
#define NUM_OF_LED  1
#define YELLOW_INTERVAL 200

#define X -1
//板子上Ax是D（14+x）
//clk_pin A5 data_pin A4


ChainableLED leds(19, 18, NUM_OF_LED);

int sw1pin = 6; //黄灯闪烁开关
int sw2pin = 7; //这个是顺序模式开关
int sw3pin = 15;
int buzzlepin=10;

//乐谱
int note[]={X,X,NOTE_E6,NOTE_D6,NOTE_G6,NOTE_G6,NOTE_A6,NOTE_A6,NOTE_B6,NOTE_G6,NOTE_D6,
NOTE_E6,NOTE_D6,NOTE_G6,NOTE_G6,NOTE_A6,NOTE_A6,NOTE_B6,NOTE_A6,NOTE_E6,NOTE_D6,NOTE_G6,
NOTE_G6,NOTE_A6,NOTE_A6,NOTE_B6,NOTE_G6,NOTE_D6,NOTE_E6,NOTE_D6,NOTE_G6,NOTE_G6,NOTE_A6,NOTE_G6};

int dura[]={2,2,1,1,2,2,2,2,2,2,2,1,1,2,2,2,2,2,4,1,1,2,2,2,2,2,2,2,1,1,2,2,2,8};
//
int tune_2[] = //简谱频率
{
  NOTE_F4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F6,NOTE_F6,NOTE_F6,
  NOTE_F5,NOTE_F5,NOTE_F5,NOTE_F5,NOTE_FH1,NOTE_FH1,NOTE_FH1,NOTE_FH1,
  NOTE_FH2,NOTE_FH2,NOTE_FH2,NOTE_FH2,NOTE_FH2,NOTE_FH2,NOTE_FH2,NOTE_FH2,
  NOTE_F5,NOTE_F4,NOTE_F3,NOTE_F1,NOTE_F2,NOTE_0,NOTE_F2,NOTE_F6,
  NOTE_F5,NOTE_0,NOTE_F4,NOTE_0,NOTE_F3,NOTE_0,NOTE_F3,NOTE_F3,NOTE_F5,NOTE_0,NOTE_F4,NOTE_F3,NOTE_F2,NOTE_0,NOTE_F2,NOTE_FH4,
  NOTE_FH3, NOTE_FH4, NOTE_FH3, NOTE_FH4, NOTE_F2, NOTE_0,NOTE_F2,NOTE_FH4, NOTE_FH3, NOTE_FH4, NOTE_FH3, NOTE_FH4, NOTE_F2, NOTE_0,NOTE_F2,NOTE_F6,
  NOTE_F5,NOTE_0,NOTE_F4,NOTE_0,NOTE_F3,NOTE_0,NOTE_F3,NOTE_F3,NOTE_F5,NOTE_0,NOTE_F4,NOTE_F3,NOTE_F2,NOTE_0,NOTE_F2,NOTE_FH4,
  NOTE_FH3, NOTE_FH4, NOTE_FH3, NOTE_FH4, NOTE_F2, NOTE_0,NOTE_F2,NOTE_FH4,NOTE_FH3, NOTE_FH4, NOTE_FH3, NOTE_FH4,
};
float durt_2[]=                   //根据简谱列出各节拍
{
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5
};

int length;//这里定义一个变量，后面用来表示共有多少个音符 

//
int len=sizeof(note)/sizeof(int);
int infraredpin=2;
void setup() {
  pinMode(sw1pin, INPUT);
  pinMode(sw2pin, INPUT);
  pinMode(sw3pin,INPUT);
  pinMode(infraredpin,INPUT);
  leds.init();//初始化时已经设置灭
  leds.setColorRGB(0, 0, 0, 0);
  pinMode(buzzlepin,OUTPUT);
  for(int i=0;i<len;++i)
  {
    tone(buzzlepin,note[i]);
    delay(200*dura[i]);
  }
  noTone(buzzlepin);
  length = sizeof(tune_2)/sizeof(tune_2[0]);
}
int key1val = 0;
int key2val = 0;
int key3val = 0;

int count1 = 0;
int count2 = 0;
void loop() {

  //控制信号

  if (!digitalRead(sw1pin)) {
    key1val = 1;
    if (key2val) {
      key2val = 0;
      count2 = 0;
    }
  }
  if (!digitalRead(sw2pin)) {
    key2val = 1;
    if (key1val) {
      key1val = 0;
      count1 = 0;
    }
  }
  if(!digitalRead(infraredpin)){
    for(int x=0;x<length;x++)
  {
    tone(buzzlepin,tune_2[x]);
    if(x%2)
    leds.setColorRGB(0, 255, 0,0);
    else
    leds.setColorRGB(0,0, 0, 0);
    if(!digitalRead(sw3pin))break;
    delay(500*durt_2[x]);  
  }
    noTone(buzzlepin);
    leds.setColorRGB(0,0, 0, 0);
    }
  if (key1val && !((count1 / YELLOW_INTERVAL) % 2))
    leds.setColorRGB(0, 255, 0, 180);
  if (key1val && ((count1 / YELLOW_INTERVAL) % 2))
    leds.setColorRGB(0, 0, 0, 0);
  if (key1val)
    ++count1;
  //正常切换
  if (key2val) {
    switch ((count2/500)%5) {
      case 0:
      case 1:
      leds.setColorRGB(0, 255, 0, 0); break;
      case 2:
        leds.setColorRGB(0, 255, 0, 180); break;
      case 3:
      case 4:
        leds.setColorRGB(0, 0, 0, 255);break;
    }
    ++count2;
  }
  


}

