
// Input range - 00 to 99.
BYTE bcd2bin(BYTE bcd_value)
{
BYTE temp;

temp = bcd_value;
// Shifting upper digit right by 1 is same as multiplying by 8.
temp >>= 1;
// Isolate the bits for the upper digit.
temp &= 0x78;

// Now return: (Tens * 8) + (Tens * 2) + Ones

return(temp + (temp >> 2) + (bcd_value & 0x0f));
}
BYTE bin2bcd(BYTE binary_value)
{
BYTE temp;
BYTE retval;

temp = binary_value;
retval = 0;

while(1)
{
// Get the tens digit by doing multiple subtraction
// of 10 from the binary value.
if(temp >= 10)
{
temp -= 10;
retval += 0x10;
}
else // Get the ones digit by adding the remainder.
{
retval += temp;
break;
}
}

return(retval);
}

void ds1307_init(void)
{
BYTE initsec = 0;
BYTE initmin=0;
BYTE inithr=0;
BYTE initdow=0;
BYTE initday=0;
BYTE initmth=0;
BYTE inityear=0;
i2c_start();
i2c_write(0xD0);
i2c_write(0x00);
i2c_start();
i2c_write(0xD1);      // RD from RTC
initsec  = bcd2bin(i2c_read() & 0x7f);
initmin  = bcd2bin(i2c_read() & 0x7f);
inithr   = bcd2bin(i2c_read() & 0x3f);
initdow  = bcd2bin(i2c_read() & 0x7f);   // REG 3
initday  = bcd2bin(i2c_read() & 0x3f);   // REG 4
initmth  = bcd2bin(i2c_read() & 0x1f);   // REG 5
inityear = bcd2bin(i2c_read(0));         // REG 6
i2c_stop();
//delay_us(3);

i2c_start();
i2c_write(0xD0);      // WR to RTC
i2c_write(0x00);      // REG 0
i2c_write(bin2bcd(initsec));      // Start oscillator with current "seconds value
i2c_write(bin2bcd(initmin));      // REG 1
i2c_write(bin2bcd(inithr));       // REG 2
i2c_write(bin2bcd(initdow));      // REG 3
i2c_write(bin2bcd(initday));      // REG 4
i2c_write(bin2bcd(initmth));      // REG 5
i2c_write(bin2bcd(inityear));     // REG 6
i2c_write(0x90);      //  squarewave output pin 1Hz
i2c_stop();
}

void ds1307_set_date_time(BYTE day, BYTE mth, BYTE year, BYTE dow, BYTE hr, BYTE min, BYTE sec)
{
sec &= 0x7F;
hr &= 0x3F;

i2c_start();
i2c_write(0xD0);              // I2C write address
i2c_write(0x00);              // Start at REG 0 - Seconds
i2c_write(bin2bcd(sec));      // REG 0
i2c_write(bin2bcd(min));      // REG 1
i2c_write(bin2bcd(hr));       // REG 2
i2c_write(bin2bcd(dow));      // REG 3
i2c_write(bin2bcd(day));      // REG 4
i2c_write(bin2bcd(mth));      // REG 5
i2c_write(bin2bcd(year));     // REG 6
i2c_write(0x90);              // REG 7 - 1Hz squarewave output pin
i2c_stop();
}

void ds1307_get_date(BYTE &day, BYTE &mth, BYTE &year, BYTE &dow)
{
i2c_start();
i2c_write(0xD0);
i2c_write(0x03);              // Start at REG 3 - Day of week
i2c_start();
i2c_write(0xD1);
dow  = bcd2bin(i2c_read() & 0x7f);   // REG 3
day  = bcd2bin(i2c_read() & 0x3f);   // REG 4
mth  = bcd2bin(i2c_read() & 0x1f);   // REG 5
year = bcd2bin(i2c_read(0));         // REG 6
i2c_stop();
}

void ds1307_get_time(BYTE &hr, BYTE &min, BYTE &sec)
{
i2c_start();
i2c_write(0xD0);
i2c_write(0x00);                     // Start at REG 0 - Seconds
i2c_start();
i2c_write(0xD1);
sec = bcd2bin(i2c_read() & 0x7f);
min = bcd2bin(i2c_read() & 0x7f);
hr  = bcd2bin(i2c_read(0) & 0x3f);
i2c_stop();
}



