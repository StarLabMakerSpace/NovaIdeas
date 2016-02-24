//#include <Marathon.h>
#include <Nova.h> 
uint8_t RUN_State = 0;
uint16_t Km = 0;
float Km_f=0;
float Km_Now = 0;
uint32_t clyc_count = 0;
int motor_speed=100;
uint8_t data_bit = 0;
uint8_t point_bit = 4;
//LineFinder linefinder(S4);
LineFinder linefinder = LineFinder(S4); // C0、C1、S4、S5 
//DigitDisplay dis(S5) ;
DigitDisplay dis =DigitDisplay(S5);// C0、C1、S4、S5 
//Motor motor(M0);
Motor motor = Motor(M0); // M0,M1
//Buzzer buzzer(S1);
Buzzer buzzer = Buzzer(S1);
IRSendRev IR;
void setup()
{
  IR.begin(S0);
  dis.clear();
  Serial.begin(9600);
  Serial.println("Marathon!!!");
}
void loop()
{
    IR_Set_Km();
    CPU_State();
}
void IR_Set_Km(void)
{
  if(IR.available())//判断是否接收到数据
    {
      uint8_t data = IR_Remote(IR.recv());
      if(data != 0)
        buzzer.tone(data*100, 100);
      else
        buzzer.tone(100, 100);
      Serial.println(data);
      switch(data)
      {
        case IR_BUTTON_UP:
          if(motor_speed < 255)motor_speed+=10;
          else motor_speed=255;
        break;
        case IR_BUTTON_DOWN:
          if(motor_speed >= 100)motor_speed-=10;
          else motor_speed =100;
        break;
        case IR_BUTTON_OK:
          if(RUN_State == 0)RUN_State =1;
          else RUN_State = 0;
          dis.clear();
        break;
        case IR_BUTTON_LEFT:
          
        break;
        case IR_BUTTON_RIGHT:
           
        break;
        case IR_BUTTON_SPARK:
          point_bit = data_bit;
          Serial.print(point_bit);
        break;
        case IR_BUTTON_POUND:
          Km = 0;
          point_bit = 4;
          data_bit=0;
          dis.clear();
        break;
        default:
          if(Km<=999)
          {
              Km*=10;
              Km+=data;
              data_bit++;
          }
        break;
      }
      Km_f = Display_Km(Km, point_bit);
    }
}
void CPU_State(void)
{
  switch(RUN_State)
  {
    case 0:
      clyc_count=0;
      dis.displayFloat(Km_f);
      motor.stop();
    break;
    case 1:
      Serial.println("Motor Runing");
      dis.clear();
      dis.displayFloat(Km_Now);
      motor.run(motor_speed);
      Check_Cly();
    break;
    case 2:
      
    break;
  }
}
void Check_Cly(void)
{
  while( linefinder.stateR() == true )
  {
    clyc_count++;
    delay(100);
    Serial.println("linefinder.state1()==true");
  }
  Km_Now = Km_f - clyc_count*0.001;
  if((Km_f*1000 - clyc_count) <= 100)
  {
    RUN_State=0;
  }
}
float Display_Km(uint16_t km, uint8_t Bit)
{
  float Km_fo=0;
  switch(Bit)
  {
    case 0:
      if(km >= 10000)
        Km_fo = km*0.0001;
      else if(km >= 1000)Km_fo = km*0.001;
      else if(km >= 100)Km_fo = km*0.01;
      else if(km >= 10)Km_fo = km*0.1;
      else
        Km_fo = km;
    break;

    case 1:
      if(km >= 1000)
        Km_fo = km*0.001;
      else if(km >= 100)Km_fo = km*0.01;
      else if(km >= 10)Km_fo = km*0.1;
      else
        Km_fo = km;
    break;

    case 2:
      if(km >= 1000)
        Km_fo = km*0.01;
      else if(km >= 100)Km_fo = km*0.1;
      else Km_fo = km;
    break;

    case 3:
      if(km >= 1000)Km_fo = km*0.1;
      else Km_fo = km;
    break;

    case 4:
      Km_fo = km;
    break;
  }
  return Km_fo;
}
uint8_t IR_Remote(uint8_t Data)
{
  switch(Data)
  {
    case 255:
      return 1;
    break;
    case 127:
      return 2;
    break;
    case 191:
      return 3;
    break;
    case 223:
      return 4;
    break;
    case 95:
      return 5;
    break;
    case 159:
      return 6;
    break;
    case 239:
      return 7;
    break;
    case 111:
      return 8;
    break;
    case 175:
      return 9;
    break;
    case 79:
      return 0;
    break;
  }
}

