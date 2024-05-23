#include <main.h>
#include <lcd.c>
#include <ds1307.c>
#include <dht22.c>
//Dinh nghia cac bien de hoat dong lcd
#define LCD_ENABLE_   PIN_D0                        
#define LCD_RS_PIN    PIN_D1  
#define LCD_RW_PIN    PIN_D2 
#define LCD_DATA4     PIN_D4 
#define LCD_DATA5     PIN_D5 
#define LCD_DATA6     PIN_D6 
#define LCD_DATA7     PIN_D7
//Dinh nghia cac bien phuc vu chuong trinh tao menu len lcd
#define LEN           input(PIN_A1)
#define XUONG         input(PIN_A2)
#define CHON          input(PIN_A3)
#define BACK          input(PIN_A5)
//Khai bao cac bien doc gia tri cua ds1307
int8 sec,min,hrs,day,month,yr,dow;
//Khai bao bien phuc vu chuong trinh tao menu len lcd
int demmenu = 0;
int menu_flag = 0;
int alarm_menu = 0;
//Khai bao bien phuc vu ham cai dat thoi gian
int clock_flag = 0;
int clk = 0;
int8 sec_setting=0,min_setting=0,hrs_setting=0,day_setting=1,month_setting=1,yr_setting=24,dow_setting=2;
//Khai bao bien phuc vu hien thi gia tri vuot nguong
int carry = 0;
unsigned int8 alarm_value[11];
//Ham hien thi DS1307
void Time_display();
//Ham cai dat thoi gian
void time_update();
void time_setting();
void time_set_select_1();
void time_set_select_2();
void dow_update();
void day_update();
void month_update();
//void yr_update();
void hrs_update();
void min_update();
//void sec_update();
//Ham hien thi man hinh do:
void MAIN_SCREEN();
//Ham doc gia tri DHT22
int DHT22_start();
int Read_DTH22();
//Ham tao menu len lcd
void menucapnhat();
void chonmenu();
void menu();
//Ham phuc vu canh bao vuot nguong
void alarm_flag_function();
int alarm_cutoff_mannual(int mannual);
void menu_canh_bao();
void lichsu_canh_bao();
//Ham phuc vu hien thi 3 gia tri canh bao gan nhat len lcd
void writeEEPROM(unsigned int8 address);
void ALARM_HISTORY(unsigned int8 address);
//Ham chinh
void main()
{
   //Cai dat port, khoi tao thiet bi:
   set_tris_d(0x00);
   set_tris_a(0x2E);
   set_tris_b(0x00);
   output_low(DEN);
   output_low(BUZZER);
   lcd_init(); //Khoi tao LCD.
   output_float(Data_Pin);
   ds1307_init(); // khoi tao DS1307
   delay_ms(200);
   //Cai dat thoi gian:
   time_set_select_1();
   time_update();
   delay_ms(300);
   //Hien thi man hinh do:
   chonmenu();
   while(1)
   {
      menu();
   }
}
//Cac ham phuc vu DS1307
////////////////////////////////////////
void Time_display()
{    
      //Truyen len LCD
      lcd_gotoxy(2,1);
      if(dow != 1)
      {
      lcd_putc("Thu ");
      printf(lcd_putc,"%d : ",dow);
      }
      else if(dow == 1)
      {
      lcd_putc("CN   : ");
      }
      lcd_gotoxy(10,1);
      printf(lcd_putc,"%d",day/10);
      printf(lcd_putc,"%d",day%10);
      lcd_putc(47);
      printf(lcd_putc,"%d",month/10);
      printf(lcd_putc,"%d",month%10);
      lcd_putc(47);
      printf(lcd_putc,"20%d",yr/10);
      printf(lcd_putc,"%d ",yr%10);
      lcd_gotoxy(1,2);
      printf(lcd_putc,"%d",hrs/10);
      printf(lcd_putc,"%d",hrs%10);
      lcd_putc(58);
      printf(lcd_putc,"%d",min/10);
      printf(lcd_putc,"%d",min%10);
      lcd_putc(58);
      printf(lcd_putc,"%d",sec/10);
      printf(lcd_putc,"%d",sec%10);
}
//Ham hien thi thoi gian, nhiet do, do am
//////////////////////////////////////////
void MAIN_SCREEN()
{ 
      ds1307_get_date(day,month,yr,dow); 
      ds1307_get_time(hrs,min,sec);
      Time_display();
      DHT22_start();
      Read_DTH22();
      GET_VALUE_DHT22();
      alarm_flag_function();
      DHT22_DISPLAY();
      delay_ms(500); // tao thoi gian tre cho cam bien san sang
}
//Ham phuc vu cai dat thoi gian
void time_update()
{
 while(1)
 {
  if(clock_flag == 1 )
  {
   clock_flag = 0;
  }
  if(CHON == 0 && clock_flag == 0)
  {
  delay_ms(5);
  if(CHON == 0 && clock_flag == 0)
  {
   clk++;
   if(clk>4)
   {
    clk=0;
   }
   time_set_select_2();
   while(CHON == 0 && clock_flag == 0){};
   }
  }
  else if(LEN == 0  && clock_flag == 0)
  {
   delay_ms(5);
   if(LEN == 0  && clock_flag == 0)
   {
    time_setting();
    while(LEN == 0 && clock_flag == 0){};
   }
  }
  else if(XUONG == 0  && clock_flag == 0)
  {
   delay_ms(5);
   if(XUONG == 0  && clock_flag == 0)
   {
    time_setting();
    while(XUONG == 0 && clock_flag == 0){};
   }
  }
  else if(BACK == 0 && clock_flag == 0)
  {
   delay_ms(5);
   if(BACK == 0 && clock_flag == 0)
   {
    ds1307_set_date_time(day_setting,month_setting,yr_setting,dow_setting,hrs_setting,min_setting,sec_setting);
    lcd_cursor_on(0);
    lcd_putc("\f");
    while(BACK == 0 && clock_flag == 0){};
    break;
   }
  }
 }
}
void time_set_select_1()
{
      lcd_gotoxy(1,1);
      if(dow != 1)
      {
      lcd_putc("Thu ");
      printf(lcd_putc,"%d : ",dow_setting);
      }
      else if(dow == 1)
      {
      lcd_putc("CN   : ");
      }
      lcd_cursor_on(1);
      lcd_gotoxy(8,1);
      printf(lcd_putc,"%d",day_setting/10);
      printf(lcd_putc,"%d",day_setting%10);
      lcd_putc(47);
      printf(lcd_putc,"%d",month_setting/10);
      printf(lcd_putc,"%d",month_setting%10);
      lcd_putc(47);
      printf(lcd_putc,"20%d",yr_setting/10);
      printf(lcd_putc,"%d ",yr_setting%10);
      lcd_gotoxy(1,2);
      printf(lcd_putc,"%d",hrs_setting/10);
      printf(lcd_putc,"%d",hrs_setting%10);
      lcd_putc(58);
      printf(lcd_putc,"%d",min_setting/10);
      printf(lcd_putc,"%d",min_setting%10);
      lcd_putc(58);
      printf(lcd_putc,"%d",sec_setting/10);
      printf(lcd_putc,"%d",sec_setting%10);
      lcd_gotoxy(21,2);
      lcd_putc("CAI DAT THOI GIAN");
      lcd_gotoxy(5,1);
      lcd_cursor_on(1);
}
void time_set_select_2()
{
 lcd_gotoxy(21,2);
 lcd_putc("CAI DAT THOI GIAN");
 if(clk==0)
 {
  lcd_gotoxy(5,1);
 }
 else if(clk==1)
 {
  lcd_gotoxy(9,1);
 }
 else if(clk==2)
 {
  lcd_gotoxy(12,1);
 }
 else if(clk==3)
 {
  lcd_gotoxy(2,2);
 }
 else if(clk==4)
 {
  lcd_gotoxy(5,2);
 }
}
void dow_update()
{
 lcd_gotoxy(21,2);
 lcd_putc("CAI DAT THU!       ");
 while(1)
 {
 if(LEN == 0 && clock_flag == 1 )
    {
     delay_ms(5);
     if(LEN == 0 && clock_flag == 1 )
     {
      dow_setting++;
      lcd_gotoxy(5,1);
      printf(lcd_putc,"%d :",dow_setting);
      if(dow_setting > 7)
      {
       dow_setting = 1;
       lcd_gotoxy(6,1);
       lcd_putc("CN:");
      }
      lcd_gotoxy(5,1);
      while(LEN == 0 && clock_flag == 1){};
     }
    }
  else if(CHON== 0 && clock_flag == 1 )
  {
   delay_ms(5);
   if(CHON == 0 && clock_flag == 1 )
   {
   while(CHON == 0 && clock_flag == 1){};
   break;
   }
  }
 }
}
void day_update()
{
 lcd_gotoxy(21,2);
 lcd_putc("CAI DAT NGAY!      ");
 while(1)
 {
 if(LEN == 0 && clock_flag == 1 )
    {
     delay_ms(5);
     if(LEN == 0 && clock_flag == 1 )
     {
      day_setting++;
      if(day_setting > 31)
      {
       day_setting = 1;
      }
      lcd_gotoxy(8,1);
      printf(lcd_putc,"%d",day_setting/10);
      lcd_gotoxy(9,1);
      printf(lcd_putc,"%d",day_setting%10);
      lcd_gotoxy(9,1);
      while(LEN == 0 && clock_flag == 1){};
     }
    }
  else if(XUONG == 0 && clock_flag == 1)
  {
  delay_ms(5);
  if(XUONG == 0 && clock_flag == 1)
  {
   day_setting--;
   if(day_setting < 1)
   {
    day_setting = 31;
   }
   lcd_gotoxy(8,1);
   printf(lcd_putc,"%d",day_setting/10);
   lcd_gotoxy(9,1);
   printf(lcd_putc,"%d",day_setting%10);
   lcd_gotoxy(9,1);
   while(XUONG == 0 && clock_flag == 1){};
  }
  }
  else if(CHON == 0 && clock_flag == 1 )
  {
   delay_ms(5);
   if(CHON == 0 && clock_flag == 1 )
   {
   while(CHON == 0 && clock_flag == 1){};
   break;
   }
  }
 }
}
void month_update()
{
 lcd_gotoxy(21,2);
 lcd_putc("CAI DAT THANG!    ");
 while(1)
 {
 if(LEN == 0 && clock_flag == 1 )
    {
     delay_ms(5);
     if(LEN == 0 && clock_flag == 1 )
     {
      month_setting++;
      if(month_setting > 12)
      {
       month_setting = 1;
      }
      lcd_gotoxy(11,1);
      printf(lcd_putc,"%d",month_setting/10);
      lcd_gotoxy(12,1);
      printf(lcd_putc,"%d",month_setting%10);
      lcd_gotoxy(12,1);
      while(LEN == 0 && clock_flag == 1){};
     }
    }
  else if(CHON == 0 && clock_flag == 1 )
  {
   delay_ms(5);
   if(CHON == 0 && clock_flag == 1 )
   {
    while(CHON == 0 && clock_flag == 1){};
   break;
   }
  }
 }
}
void hrs_update()
{
 lcd_gotoxy(21,2);
 lcd_putc("CAI DAT GIO!       ");
 while(1)
 {
 if(LEN == 0 && clock_flag == 1 )
    {
     delay_ms(5);
     if(LEN == 0 && clock_flag == 1 )
     {
      hrs_setting++;
      if(hrs_setting > 24)
      {
       hrs_setting = 0;
      }
      lcd_gotoxy(1,2);
      printf(lcd_putc,"%d",hrs_setting/10);
      lcd_gotoxy(2,2);
      printf(lcd_putc,"%d",hrs_setting%10);
      lcd_gotoxy(2,2);
      while(LEN == 0 && clock_flag == 1){};
     }
    }
  else if(XUONG == 0 && clock_flag == 1)
  {
  delay_ms(5);
  if(XUONG == 0 && clock_flag == 1)
  {
   if(hrs_setting <= 0)
   {
    hrs_setting = 24;
   }
   else
   {
   hrs_setting--;
   }
    lcd_gotoxy(1,2);
    printf(lcd_putc,"%d",hrs_setting/10);
    lcd_gotoxy(2,2);
    printf(lcd_putc,"%d",hrs_setting%10);
    lcd_gotoxy(2,2);
   while(XUONG == 0 && clock_flag == 1){};
  }
  }
  else if(CHON== 0 && clock_flag == 1 )
  {
   delay_ms(5);
   if(CHON == 0 && clock_flag == 1 )
   {
   while(CHON == 0 && clock_flag == 1){};
   break;
   }
  }
 }
}
void min_update()
{
 lcd_gotoxy(21,2);
 lcd_putc("CAI DAT PHUT!       ");
 while(1)
 {
 if(LEN == 0 && clock_flag == 1 )
    {
     delay_ms(5);
     if(LEN == 0 && clock_flag == 1 )
     {
      min_setting++;
      if(min_setting > 60)
      {
       min_setting = 0;
      }
      lcd_gotoxy(4,2);
      printf(lcd_putc,"%d",min_setting/10);
      lcd_gotoxy(5,2);
      printf(lcd_putc,"%d",min_setting%10);
      lcd_gotoxy(5,2);
      while(LEN == 0 && clock_flag == 1){};
     }
    }
  else if(XUONG == 0 && clock_flag == 1)
  {
  delay_ms(5);
  if(XUONG == 0 && clock_flag == 1)
  {
   if(min_setting <= 0)
   {
    min_setting = 60;
   }
   else
   {
   min_setting--;
   }
    lcd_gotoxy(4,2);
    printf(lcd_putc,"%d",min_setting/10);
    lcd_gotoxy(5,2);
    printf(lcd_putc,"%d",min_setting%10);
    lcd_gotoxy(5,2);
   while(XUONG == 0 && clock_flag == 1){};
  }
  }
  else if(CHON== 0 && clock_flag == 1 )
  {
   delay_ms(5);
   if(CHON == 0 && clock_flag == 1 )
   {
   while(CHON == 0 && clock_flag == 1){};
   break;
   }
  }
 }
}
void time_setting()
{
 clock_flag = 1;
 switch(clk)
 {
  case 0:
         dow_update();
         break;
  case 1:
         day_update();
         break;
  case 2:
         month_update();
         break;
  case 3:
         hrs_update();
         break;
  case 4:
         min_update();
         break;
 }
}
//Cac ham tao menu cho lcd
//////////////////////////////////////////
void menu()
{  
   menu_flag = 0;
   if (LEN == 0 && menu_flag == 0){
   delay_ms(5);
  if(LEN == 0)
    {
    if (demmenu <= 0)    
    { 
     demmenu = 3;  
    }
    else 
    {
    demmenu--;
    }
    menucapnhat();
    while(LEN == 0){};
  }
  }
  else if (XUONG == 0 && menu_flag == 0)
  {
   delay_ms(5);
   if(XUONG == 0)
   {
    if (demmenu >= 3)
    {
    demmenu = 0;  
    }
    else 
    {
    demmenu++;
    }
    menucapnhat();
    while(XUONG == 0){};
   }
  }
  else if (CHON == 0 && menu_flag == 0)
  {
    delay_ms(5);
   if( CHON == 0)
   {
    chonmenu();
    while(CHON == 0){};
   }
  }
}

void menucapnhat() 
{
  if (demmenu == 0)
  {
    lcd_putc("\f");
    lcd_putc(">MAN HINH DO");
    lcd_gotoxy(1, 2);
    lcd_putc(" CAI DAT NHIET DO");
    lcd_gotoxy(21,1);
    lcd_putc(" CAI DAT DO AM");
    lcd_gotoxy(21,2);
    lcd_putc(" LICH SU CANH BAO");
  }
  else if (demmenu == 1)
  {
    lcd_putc("\f");
    lcd_putc(" MAN HINH DO");
    lcd_gotoxy(1, 2);
    lcd_putc(">CAI DAT NHIET DO");
    lcd_gotoxy(21,1);
    lcd_putc(" CAI DAT DO AM");
    lcd_gotoxy(21,2);
    lcd_putc(" LICH SU CANH BAO");
  }
  else if (demmenu == 2)
  {
    lcd_putc("\f");
    lcd_putc(" MAN HINH DO");
    lcd_gotoxy(1, 2);
    lcd_putc(" CAI DAT NHIET DO");
    lcd_gotoxy(21,1);
    lcd_putc(">CAI DAT DO AM");
    lcd_gotoxy(21,2);
    lcd_putc(" LICH SU CANH BAO");
  }
  else if (demmenu == 3)
  {
    lcd_putc("\f");
    lcd_putc(" MAN HINH DO");
    lcd_gotoxy(1, 2);
    lcd_putc(" CAI DAT NHIET DO");
    lcd_gotoxy(21,1);
    lcd_putc(" CAI DAT DO AM");
    lcd_gotoxy(21,2);
    lcd_putc(">LICH SU CANH BAO");    
  }
}
  
void chonmenu() 
{ 
  menu_flag = 1;
  switch (demmenu) 
  {
    case 0:
      lcd_putc('\f');
      while(1)
      {
      MAIN_SCREEN();
      if(BACK == 0 && menu_flag == 1)
      {
       delay_ms(5);
       if(BACK == 0)
       {
       menucapnhat();
       while(BACK == 0){};
       break;
      }
      }
      }
      break;
    case 1:
      lcd_putc('\f');
      do
      {
      lcd_gotoxy(1,1);
      lcd_putc("NGUONG NHIET DO:");
      lcd_gotoxy(3,2);
      lcd_putc(223);
      lcd_putc("C ");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"%d",nguong_nhiet_do);
      if(LEN == 0 && menu_flag == 1){
      delay_ms(5);
      if(LEN == 0)
      {
      nguong_nhiet_do++;
      lcd_gotoxy(1,2);
      printf(lcd_putc,"%d",nguong_nhiet_do);
      while(LEN == 0){};
      }
      }
      else if(XUONG == 0 && menu_flag == 1)
      {
       delay_ms(5);
       if(XUONG == 0)
       {
       nguong_nhiet_do--;
       lcd_gotoxy(1,2);
       printf(lcd_putc,"%d",nguong_nhiet_do);
       while(XUONG == 0){};
      }
      }
      }while(BACK == 1 && menu_flag == 1);
      delay_ms(5);
      if(BACK == 0 && menu_flag == 1)
      {
      menucapnhat();
      while(BACK == 0){};
      }
      break;
    case 2:
       lcd_putc('\f');
      do
      {
      lcd_gotoxy(1,1);
      lcd_putc("NGUONG DO AM:");
      lcd_gotoxy(3,2);
      lcd_putc("%");
      lcd_gotoxy(1,2);
      printf(lcd_putc,"%d",nguong_do_am);
      if(LEN == 0 && menu_flag == 1)
      {
      delay_ms(5);
      if(LEN == 0)
      {
      nguong_do_am++;
      lcd_gotoxy(1,2);
      printf(lcd_putc,"%d",nguong_do_am);
      while( LEN == 0){};
      }
      }
      else if(XUONG == 0 && menu_flag == 1)
      {
       delay_ms(5);
       if(XUONG == 0)
       {
       nguong_do_am--;
       lcd_gotoxy(1,2);
       printf(lcd_putc,"%d",nguong_do_am);
       while( XUONG == 0){};
      }
      }
      }while(BACK == 1 && menu_flag == 1);
      delay_ms(5);
      if(BACK == 0 && menu_flag == 1)
      {
      menucapnhat();
      while(BACK == 0){};
      break;
      }
      break;
    case 3:
     lichsu_canh_bao();
     while(1)
     {
     menu_canh_bao();
     if(BACK == 0 && menu_flag == 1)
     {
      delay_ms(5);
      if(BACK == 0)
      {
       menucapnhat();
       while(BACK == 0){};
       break;
      }
     }
     }
     break;
}
}
//Ham phuc vu hien thi gia tri vuot nguong len lcd
int alarm_cutoff_mannual(int mannual)
{
 if(CHON == 0 && menu_flag == 1 && mannual == 1)
  {
   delay_ms(5);
   if(CHON == 0 && mannual == 1)
  {
   return 1;
   while(CHON==0){};
  }
  }
 else 
 {
  return 0;
 }
}
void alarm_flag_function()
{
  if( (carry == 0) && ((temp/10)> nguong_nhiet_do || (humid/10) > nguong_do_am))
  {
  writeEEPROM(0x00);
  carry = 1;       
  output_high(DEN); 
  output_high(BUZZER);
  }
  else if((temp/10) <= nguong_nhiet_do && (humid/10) <= nguong_do_am && carry == 1)
  {
  carry = 0;   
  output_low(DEN);
  output_low(BUZZER);
  }
  else if(alarm_cutoff_mannual(carry) == 1)
  {
  carry = 1;   
  output_low(DEN);
  output_low(BUZZER);
}
}
//Ham phuc vu ghi du lieu canh bao vao eeprom va hien thi len lcd
void writeEEPROM(unsigned int8 address)
{  
   for(unsigned int8 i=0x00;i<=0x0A;i++)
   {
    write_eeprom(i+0x16,read_eeprom(i+0x0B));
   }
   for(unsigned int8 j=0x00;j<=0x0A;j++)
   {
    write_eeprom(j+0x0B,read_eeprom(j));
   }
   write_eeprom(address,nhietdo[0]);
   write_eeprom(address+0x01,nhietdo[1]);
   write_eeprom(address+0x02,doam[0]);
   write_eeprom(address+0x03,doam[1]);
   write_eeprom(address+0x04,dow);
   write_eeprom(address+0x05,day);
   write_eeprom(address+0x06,month);
   write_eeprom(address+0x07,yr);
   write_eeprom(address+0x08,hrs);
   write_eeprom(address+0x09,min);
   write_eeprom(address+0x0A,sec);
}
void ALARM_HISTORY(unsigned int8 address)
{ 
   for(int i=0;i<=10;i++)
   {
    alarm_value[i] = read_eeprom(address+i);
   }
}
void menu_canh_bao()
{
  if (XUONG == 0 && menu_flag == 1)
  {
   delay_ms(5);
  if(XUONG == 0)
   {
    if (alarm_menu >= 2)
    { 
     alarm_menu = 0;  
    }
    else
    { 
      alarm_menu++;  
    }
    lichsu_canh_bao();
    while(XUONG == 0){};
  }
  }
   if (LEN == 0 && menu_flag == 1)
  {
   delay_ms(5);
  if(LEN == 0)
   {
    if (alarm_menu <=0)
    { 
     alarm_menu = 2;  
    }
    else
    { 
      alarm_menu--;  
    }
    lichsu_canh_bao();
    while(LEN == 0){};
  }
  }
}
void lichsu_canh_bao()
{
 switch(alarm_menu)
 {
  case 0:
      lcd_putc('\f');
      ALARM_HISTORY(0x00);
      lcd_gotoxy(1,1);
      lcd_putc("Canh bao 1  :");
      lcd_gotoxy(1,2);
      if(alarm_value[4] != 1)
      {
      lcd_putc("Thu ");
      printf(lcd_putc,"%d: ",alarm_value[4]);
      }
      else if(alarm_value[4] == 1)
      {
      lcd_putc("CN : ");
      }
      printf(lcd_putc,"%d",alarm_value[5]/10);
      printf(lcd_putc,"%d",alarm_value[5]%10);
      lcd_putc(47);
      printf(lcd_putc,"%d",alarm_value[6]/10);
      printf(lcd_putc,"%d",alarm_value[6]%10);
      lcd_putc(47);
      printf(lcd_putc,"20%d",alarm_value[7]/10);
      printf(lcd_putc,"%d",alarm_value[7]%10);
      lcd_gotoxy(28,1);
      printf(lcd_putc,"%d",alarm_value[8]/10);
      printf(lcd_putc,"%d",alarm_value[8]%10);
      lcd_putc(58);
      printf(lcd_putc,"%d",alarm_value[9]/10);
      printf(lcd_putc,"%d",alarm_value[9]%10);
      lcd_putc(58);
      printf(lcd_putc,"%d",alarm_value[10]/10);
      printf(lcd_putc,"%d",alarm_value[10]%10);
      lcd_gotoxy(21, 2);     
      printf(lcd_putc, "%3d%d",alarm_value[0],alarm_value[1]);
      lcd_putc(223); 
      lcd_putc("C ");     
      lcd_gotoxy(30,2);                              
      printf(lcd_putc, "%3d%d",alarm_value[2],alarm_value[3]);
      lcd_putc("%");
      delay_ms(500);
      if(BACK == 0 && menu_flag == 1)
      {
       delay_ms(5);
       if(BACK == 0)
       {
       menucapnhat();
       while(BACK == 0){};
      }
      }
      break;
  case 1:
      lcd_putc('\f');
      ALARM_HISTORY(0x0B);
      lcd_gotoxy(1,1);
      lcd_putc("Canh bao 2  :");
      lcd_gotoxy(1,2);
      if(alarm_value[4] != 1)
      {
      lcd_putc("Thu ");
      printf(lcd_putc,"%d: ",alarm_value[4]);
      }
      else if(alarm_value[4] == 1)
      {
      lcd_putc("CN : ");
      }
      printf(lcd_putc,"%d",alarm_value[5]/10);
      printf(lcd_putc,"%d",alarm_value[5]%10);
      lcd_putc(47);
      printf(lcd_putc,"%d",alarm_value[6]/10);
      printf(lcd_putc,"%d",alarm_value[6]%10);
      lcd_putc(47);
      printf(lcd_putc,"20%d",alarm_value[7]/10);
      printf(lcd_putc,"%d",alarm_value[7]%10);
      lcd_gotoxy(28,1);
      printf(lcd_putc,"%d",alarm_value[8]/10);
      printf(lcd_putc,"%d",alarm_value[8]%10);
      lcd_putc(58);
      printf(lcd_putc,"%d",alarm_value[9]/10);
      printf(lcd_putc,"%d",alarm_value[9]%10);
      lcd_putc(58);
      printf(lcd_putc,"%d",alarm_value[10]/10);
      printf(lcd_putc,"%d",alarm_value[10]%10);
      lcd_gotoxy(21, 2);     
      printf(lcd_putc, "%3d%d",alarm_value[0],alarm_value[1]);
      lcd_putc(223); 
      lcd_putc("C ");     
      lcd_gotoxy(30,2);                              
      printf(lcd_putc, "%3d%d",alarm_value[2],alarm_value[3]);
      lcd_putc("%");
      delay_ms(500);
      if(BACK == 0 && menu_flag == 1)
      {
       delay_ms(5);
       if(BACK == 0)
       {
       menucapnhat();
       while(BACK == 0){};
      }
      }
      break;
  case 2:
      lcd_putc('\f');
      ALARM_HISTORY(0x16);
      lcd_gotoxy(1,1);
      lcd_putc("Canh bao 3  :");
      lcd_gotoxy(1,2);
      if(alarm_value[4] != 1)
      {
      lcd_putc("Thu ");
      printf(lcd_putc,"%d: ",alarm_value[4]);
      }
      else if(alarm_value[4] == 1)
      {
      lcd_putc("CN : ");
      }
      printf(lcd_putc,"%d",alarm_value[5]/10);
      printf(lcd_putc,"%d",alarm_value[5]%10);
      lcd_putc(47);
      printf(lcd_putc,"%d",alarm_value[6]/10);
      printf(lcd_putc,"%d",alarm_value[6]%10);
      lcd_putc(47);
      printf(lcd_putc,"20%d",alarm_value[7]/10);
      printf(lcd_putc,"%d",alarm_value[7]%10);
      lcd_gotoxy(28,1);
      printf(lcd_putc,"%d",alarm_value[8]/10);
      printf(lcd_putc,"%d",alarm_value[8]%10);
      lcd_putc(58);
      printf(lcd_putc,"%d",alarm_value[9]/10);
      printf(lcd_putc,"%d",alarm_value[9]%10);
      lcd_putc(58);
      printf(lcd_putc,"%d",alarm_value[10]/10);
      printf(lcd_putc,"%d",alarm_value[10]%10);
      lcd_gotoxy(21, 2);     
      printf(lcd_putc, "%3d%d",alarm_value[0],alarm_value[1]);
      lcd_putc(223); 
      lcd_putc("C ");     
      lcd_gotoxy(30,2);                              
      printf(lcd_putc, "%3d%d",alarm_value[2],alarm_value[3]);
      lcd_putc("%");
      delay_ms(500);
      if(BACK == 0 && menu_flag == 1)
      {
       delay_ms(5);
       if(BACK == 0)
       {
       menucapnhat();
       while(BACK == 0){};
      }
      }
      break;
 }
}




