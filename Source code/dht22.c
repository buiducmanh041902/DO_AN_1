#define Data_Pin PIN_B0
//Dinh nghia cac bien phuc vu canh bao khi vuot nguong
#define DEN           PIN_B6
#define BUZZER        PIN_B7
unsigned int8 DATA[5];
unsigned int8 nhietdo[2];
unsigned int8 doam[2];
unsigned int16 temp, humid;
//Khai bao cac bien gia tri nguong nhiet do, do am
int8 nguong_nhiet_do=28;
int8 nguong_do_am=80;
///////////////////////////////////////////////////////////////////////////////
int DHT22_start(){
    //----------------------------giao tiep voi DTH22-------------------------
    set_tris_b(0x00);
    output_high(Data_Pin);
    output_low(Data_Pin);
    delay_ms(18); //doi > 18ms
    output_high(Data_Pin); 
    set_tris_b(0x01);
    //----------------------------kiem tra giao tiep DHT22--------------------
    delay_us(40);                     // doi  20-40us thi data xuong 0
    if(!input(Data_Pin))                        
    {
        delay_us(80);                 //doi 80us thi Data len 1
        if(input(Data_Pin))
        {
            delay_us(80);             
            return 1;    
        }
    }
}
int Read_DTH22(){
    //-----------------nhan du lieu----------------------
    for(int i = 0; i<5; i++){
       for(int ii = 0; ii<8; ii++) {
        while(!input(Data_Pin));                          //doi len 1
        delay_us(50);                         
        if(input(Data_Pin))                               //trong >28us ma data van =1 thi se gui bit 1
        {
            DATA[i] =DATA[i]<<1| 1;
            while(input(Data_Pin));
        }
        else  DATA[i] =DATA[i]<<1| 0;
       }
    }
    output_high(Data_Pin);
    //---------------------kiem tra data nhan duoc-------------------
    if(DATA[4] == ( DATA[3]+ DATA[2]+DATA[1]+ DATA[0])) 
    return 1;
}
void GET_VALUE_DHT22()
{ 
  temp = DATA[2];
  temp=((temp*2*2*2*2*2*2*2*2) + DATA[3])&0XFFFF;
  humid = DATA[0];
  temp += 5-15; // sai so nhiet do la +-0.5Celcius
  humid = ((humid*2*2*2*2*2*2*2*2)|DATA[1])&0xFFFF;
  humid += 5; // sai so do am la +-2RH nen lam tron gia tri do am len so nguyen
  nhietdo[0]=(temp/100)%10;
  nhietdo[1]=(temp/10)%10;
  doam[0]=(humid/100)%10;
  doam[1]=(humid/10)%10;
}
void DHT22_DISPLAY()
{  
   lcd_gotoxy(21, 1); 
   lcd_putc("NHIET DO:    ");      
   lcd_putc(223);  
   lcd_putc("C ");                
   lcd_gotoxy(21, 2); 
   lcd_putc("DO AM:       %       ");
   lcd_gotoxy(30, 1); 
   printf(lcd_putc, "%3d%d",nhietdo[0],nhietdo[1]); // xua ra man hinh gia tri nhiet do doc duoc   
   lcd_gotoxy(30,2);                              
   printf(lcd_putc, "%3d%d", doam[0],doam[1]);  
}


