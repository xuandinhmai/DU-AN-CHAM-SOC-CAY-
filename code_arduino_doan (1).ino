// include the library code:
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <EEPROM.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <SoftwareSerial.h>
#include <SPI.h>
SoftwareSerial SIM7600(9, 10); // SoftwareSerial SIM7600(Rx, Tx)
//Khai bao ham con
void _LuuThoiGian(void);
void _DocThoiGian(void);
void _NhanTinNhan(void);
void _GuiTinNhan(String TinNhan);
void _GoiDien(void);
//void _Delay10S(int Dem3);

//khai bao ket noi chan phat xung
#define PWM   8
//khai bao ket noi cac nut nhan
#define MENU    A0
#define TANG    A1
#define GIAM    A2
#define CHEDO   5
#define Relay   3
LiquidCrystal_I2C lcd(0X27, 20, 4);

//khai bao cac bien toan cuc
int tam = 0; //giơi han lan goi canh
signed int TanSo = 50;
int ChuKy;
signed int Duty = 10;
int ChuKyNgat;
int DutyNgat;
int Dem = 0;
int Dem1 = 0;
int Dem2 = 0;
long Dem3 = 0;
char Chuoi[21];
char tinnhan[200];
byte CheDo = 0;
int Chuyenchedo = 1;
String Thaydoichedo = "1" ;
int Doam = 30;
int Doamcaidat = 35;
boolean Trangthai = 0;
signed char Gio = 9, Phut = 10, Giay = 11;
String Menu = "0";
String CdTanSo = "50";
String CdDuty = "10";
String CdDoamcaidat = "35";
String CdGio = "9";
String CdPhut = "10";
String CdGiay = "11";
int chuyendoi = 0;
String DOAM ="10";

ISR (TIMER2_OVF_vect)
{
  //  if(CheDo!=0) return;

  TCNT2 = 156; //cai dat gia tri bat dau dem la 156

  Dem++;
  if (Dem <= DutyNgat)
  {
    digitalWrite(PWM, HIGH);
  }
  else if (Dem < ChuKyNgat)
  {
    digitalWrite(PWM, LOW);
  }
  else
  {
    Dem = 0;
  }
  Dem1++;
  Dem2++;

  if (Dem2 > 30000)
  {
    Doam = 100 - map(analogRead(A3), 0, 1024, 0, 100);

    Serial.println(Doam);
    Serial.println(Duty);
    Serial.println(TanSo);
    Serial.println(Doamcaidat);

    Serial.println(Gio);
    Serial.println(Phut);
    Serial.println(Giay);

    Serial.println(Trangthai);
    Serial.println(Chuyenchedo);
    Serial.println(Menu);
    Dem2 = 0;
  }

  Dem3++;
  if (Dem3 > 150000)
  {
    Dem3 = 0;
  }
}

void setup()
{
  // put your setup code here, to run once:
  // set up the LCD's number of columns and rows:

  lcd.init();
  lcd.backlight();
  pinMode(PWM, OUTPUT); //ngo ra
  digitalWrite(PWM, LOW);
  pinMode(MENU, INPUT_PULLUP); //thiet lap tro keo len
  pinMode(TANG, INPUT_PULLUP); //thiet lap tro keo len
  pinMode(GIAM, INPUT_PULLUP); //thiet lap tro keo len
  pinMode(CHEDO, INPUT_PULLUP); //thiet lap tro keo len
  pinMode(A3, INPUT_PULLUP);
  pinMode(A0, OUTPUT); //doc gia tri do am
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW);

  cli();//xoa ngat toan cuc
  TCCR2A = 0;
  TCCR2B = 0;
  TIMSK2 = 0;

  /*
     ta co:
     tan so he thong la 16Mhz
     voi ti le la 1/8
     thi thoi gian de timer2 thuc hien dem len tung so la
     T = 1 * (1/F) * ti_le = 1/16M * 8 = 8/16M = 0.5us
     ma timer2 se dem tu 0 toi 255 tuc la duoc 256 so dem
     nen thoi gian de timer2 hoan thanh 1 vong dem tren la
     Tmax = 256 * 0.5us = 128us
     o day ta muon timer2 hoan thanh vong dem tren chi ton la 50us
goi gia tri timer2 bat dau dem la a
     50us = (256-a) * 0.5
     <=>50/0.5 = 256-a
     <=>100 = 256-a
     <=>a = 256-100 = 156

     ta muon tao thoi gian tre la 0.5s = 500,000us
     thi ta lam nhu sau
     goi so lan timer2 can ngat la b
     b = 500,000us/100us = 5000 lan tran timer2

     muon tao tan su tu 50hz toi 1500hz
     hay chu ky tuong ung la 1/50 toi 1/1500
     hay 20,000us ve 667us
     hay so lan ngat timer2 can thuc hien la
     hay 20,000/50 ve 667/50
     hay 400 lan ngat xuong con 13 lan ngat

     ((1/F)*10^6)/50 = SoLanNgat
     <=> (1/F)*20,000 = SoLanNgat
     <=> 1/F = SoLanNgat/20,000
     <=> F = 20,000/SoLanNgat (1)
     <=> SoLanNgat = 20,000/F (2)

     ap dung cong thuc: Duty = (Ton*100%)/T
     Ton = (Duty*T)/100 (3)
  */
  TCCR2A |= (0 << COM2A1) | (0 << COM2A0) | (0 << COM2B1) | (0 << COM2B0) | (0 << WGM21) | (0 << WGM20); //che do normal
  TCCR2B |= (0 << WGM22) | (0 << CS22) | (1 << CS21) | (0 << CS20); //cai dat ti le la 1/8
  TCNT2 = 156; //cai dat gia tri bat dau dem la 156
  TIMSK2 |= (1 << TOIE2); //cho phep ngat timer xay ra khi bo dem timer dem tran
  sei();//cho phep ngat toan cuc xay ra

  ChuKyNgat = 20000 / TanSo;
  DutyNgat = (Duty * ChuKyNgat) / 100;
  //_DocThoiGian();

  // Arduino giao tiếp với lá chắn SIM900 GSM ở tốc độ truyền 19200
  // Đảm bảo rằng điều đó tương ứng với tốc độ truyền của mô-đun của bạn
  SIM7600.begin (115200);

  // Lệnh AT để đặt SIM900 sang chế độ SMS
  SIM7600.print ("AT + CMGF = 1 \ r");
  delay(100);
  // Đặt mô-đun để gửi dữ liệu SMS ra nối tiếp khi nhận
  SIM7600.print ("AT + CNMI = 2,2,0,0,0 \ r");
  delay(100);
  Serial.begin(9600);
  while (!Serial) {}
  //

}

void loop()
{

  // put your main code here, to run repeatedly:


  while (1)
  {
batdau3:
    while (CheDo == 0)
    {

      //      _NhanTinNhan();
      lcd.setCursor(0, 0);
      lcd.print("1.CHE DO TU DONG");
      lcd.setCursor(0, 1);
      sprintf(Chuoi, "TAN SO:%04d[hz]", TanSo);
      lcd.print(Chuoi);
      lcd.setCursor(0, 2);
      sprintf(Chuoi, "DUTY:%02d", Duty);
      lcd.print(Chuoi); lcd.print("%");
      lcd.setCursor(0, 3);
      sprintf(Chuoi, "DO AM:%02d", DOAM.toInt());
      lcd.print(Chuoi); lcd.print("%");
      sprintf(Chuoi, " DACD:%02d", Doamcaidat);
      lcd.print(Chuoi); lcd.print("%");
      Chuyenchedo = 1;
       
      // nhận dữ liệu từ pi 
      if (Serial.available() > 0)// khi có tín hiệu từ pi
      {
        Thaydoichedo = Serial.readStringUntil('\n');//đọc cho đến khi gặp /n
        Menu = Serial.readStringUntil('\n');
        CdTanSo = Serial.readStringUntil('\n');
        CdDuty = Serial.readStringUntil('\n');
        CdDoamcaidat = Serial.readStringUntil('\n');
        CdGio = Serial.readStringUntil('\n');
        CdPhut = Serial.readStringUntil('\n');
        CdGiay = Serial.readStringUntil('\n');
        DOAM = Serial.readStringUntil('\n');
      }
      Trangthai = 1;
      //////////////////
      chuyendoi = 0;
      if (DOAM.toInt() < Doamcaidat)
      {

        digitalWrite(Relay, HIGH);
        if (Dem3 > 149500)
        {
      sprintf(tinnhan, "DANG TUOI TU DONG \n DO AM:%02d%% ", DOAM, '%');
          _GuiTinNhan(tinnhan);
        }
      }
      else
      {
        digitalWrite(Relay, LOW);
        Trangthai = 0;
      }



      if (digitalRead(MENU) == 0)
      {
        while (digitalRead(MENU) == 0);
        CheDo = 1; //chuyen sang che do cai dat
        chuyendoi = 1;
        lcd.clear();
      }
      ////////////////////////////
      if (chuyendoi == 0)
      {
        if (Menu == "1")
        {

          CheDo = Menu.toInt(); //chuyen sang che do cai dat
          lcd.clear();
        }
      }
      //////////////////////
      if (digitalRead(CHEDO) == 0)
      {
        while (digitalRead(CHEDO) == 0);
        lcd.clear();
        goto batdau1;
      }
      else if (Thaydoichedo == "2")
      {
        lcd.clear();
        goto batdau1;
      }
    }
    while (CheDo == 1)
    {
      lcd.setCursor(0, 0);
      lcd.print("CAI DAT TAN SO");
      lcd.setCursor(0, 1);
      sprintf(Chuoi, "TAN SO:%04d[hz]", TanSo);
      lcd.print(Chuoi);

      if (Serial.available() > 0)
      {
        Thaydoichedo = Serial.readStringUntil('\n');
        Menu = Serial.readStringUntil('\n');
        CdTanSo = Serial.readStringUntil('\n');
        CdDuty = Serial.readStringUntil('\n');
        CdDoamcaidat = Serial.readStringUntil('\n');
        CdGio = Serial.readStringUntil('\n');
        CdPhut = Serial.readStringUntil('\n');
        CdGiay = Serial.readStringUntil('\n');
        DOAM = Serial.readStringUntil('\n');
      }
      ////////////////////////////
      if (Menu == "1")
      {
        TanSo = CdTanSo.toInt();
      }

      ///////////////////////

      if ((digitalRead(TANG) == 0))
      {
        while (digitalRead(TANG) == 0);
        TanSo = TanSo + 50;
        if (TanSo > 1500)
        {
          TanSo = 50;
        }
        ChuKyNgat = 20000 / TanSo;
        DutyNgat = (Duty * ChuKyNgat) / 100;
      }
      else if (digitalRead(GIAM) == 0)
      {
        while (digitalRead(GIAM) == 0);
        TanSo = TanSo - 50;
        if (TanSo < 50)
        {
          TanSo = 1500;
        }
        ChuKyNgat = 20000 / TanSo;
        DutyNgat = (Duty * ChuKyNgat) / 100;
      }
      else if (digitalRead(MENU) == 0)
      {
        while (digitalRead(MENU) == 0);
        CheDo = 2;
        lcd.clear();
        ChuKyNgat = 20000 / TanSo;
        DutyNgat = (Duty * ChuKyNgat) / 100;
      }
      //////////////////////////
      if (chuyendoi == 0)
      {
        if (Menu == "2")
        {
          //while (Menu == "0" );
          CheDo = Menu.toInt(); //chuyen sang che do cai dat
          lcd.clear();
          ChuKyNgat = 20000 / TanSo;
          DutyNgat = (Duty * ChuKyNgat) / 100;
        }
      }

    }

    while (CheDo == 2)
    {
      lcd.setCursor(0, 0);
      lcd.print("CAI DAT DUTY");
      lcd.setCursor(0, 1);
      sprintf(Chuoi, "DUTY:%02d", Duty);
      lcd.print(Chuoi); lcd.print("%");
      if (Serial.available() > 0)
      {
Thaydoichedo = Serial.readStringUntil('\n');
        Menu = Serial.readStringUntil('\n');
        CdTanSo = Serial.readStringUntil('\n');
        CdDuty = Serial.readStringUntil('\n');
        CdDoamcaidat = Serial.readStringUntil('\n');
        CdGio = Serial.readStringUntil('\n');
        CdPhut = Serial.readStringUntil('\n');
        CdGiay = Serial.readStringUntil('\n');
        DOAM = Serial.readStringUntil('\n');
      }
      ////////////////////////////
      if (Menu == "3")
      {
        Duty = CdDuty.toInt();
      }
      /////////////////////////////////////
      if (digitalRead(TANG) == 0)
      {
        while (digitalRead(TANG) == 0);
        Duty = Duty + 5;
        if (Duty > 95) Duty = 10;
        DutyNgat = (Duty * ChuKyNgat) / 100;
      }
      else if (digitalRead(GIAM) == 0)
      {
        while (digitalRead(GIAM) == 0);
        Duty = Duty - 5;
        if (Duty < 10) Duty = 95;
        DutyNgat = (Duty * ChuKyNgat) / 100;
      }
      else if (digitalRead(MENU) == 0)
      {
        while (digitalRead(MENU) == 0);
        lcd.clear();
        DutyNgat = (Duty * ChuKyNgat) / 100;
        CheDo = 3;
      }
      /////////////////////////////////
      if (chuyendoi == 0)
      {
        if (Menu == "3")
        {
          lcd.clear();
          DutyNgat = (Duty * ChuKyNgat) / 100;
          CheDo = Menu.toInt(); //chuyen sang che do cai dat
        }
      }
      ///////////////////////////////////////
    }
    while (CheDo == 3)
    {
      lcd.setCursor(0, 0);
      lcd.print("CAI DAT DO AM CD");
      lcd.setCursor(0, 1);
      sprintf(Chuoi, "DACD:%02d", Doamcaidat);
      lcd.print(Chuoi); lcd.print("%");
      if (Serial.available() > 0)
      {
        Thaydoichedo = Serial.readStringUntil('\n');
        Menu = Serial.readStringUntil('\n');
        CdTanSo = Serial.readStringUntil('\n');
        CdDuty = Serial.readStringUntil('\n');
        CdDoamcaidat = Serial.readStringUntil('\n');
        CdGio = Serial.readStringUntil('\n');
        CdPhut = Serial.readStringUntil('\n');
        CdGiay = Serial.readStringUntil('\n');
        DOAM = Serial.readStringUntil('\n');
      }
      //////////////////////////////
      if (Menu == "3")
      {
        Doamcaidat = CdDoamcaidat.toInt();
      }
      ////////////////////////////////////////

      if (digitalRead(TANG) == 0)
      {
        while (digitalRead(TANG) == 0);
        Doamcaidat++;
        if (Doamcaidat > 80) Doamcaidat = 10;

      }
      else if (digitalRead(GIAM) == 0)
      {
        while (digitalRead(GIAM) == 0);
        Doamcaidat--;
        if (Doamcaidat < 10) Doamcaidat = 80;
      }
      else if (digitalRead(MENU) == 0)
      {
        while (digitalRead(MENU) == 0);
        lcd.clear();
        CheDo = 0;
      }
      ////////////////////////
      if (chuyendoi == 0)
      {
        if (Menu == "0")
        {
          //while (digitalRead(MENU) == 0);
          lcd.clear();
          CheDo = Menu.toInt(); //chuyen sang che do cai dat
        }
      }
    }
  }

  //////////////////

  while (1)
  {
batdau1:
    while (CheDo == 0)
    {

      //      _NhanTinNhan();


      Trangthai = 1;
      chuyendoi = 0;
      if (Dem1 >= 30000)
      {
        Giay--;
        Dem1 = 0;
        if (Giay < 0 )
        {
          Phut--;
          Giay = 59;
          if (Phut < 0)
          {
            Gio--;
            Phut = 59;

          }
        }

      }
      if (Serial.available() > 0)
      {
        Thaydoichedo = Serial.readStringUntil('\n');
        Menu = Serial.readStringUntil('\n');
        CdTanSo = Serial.readStringUntil('\n');
        CdDuty = Serial.readStringUntil('\n');
        CdDoamcaidat = Serial.readStringUntil('\n');
        CdGio = Serial.readStringUntil('\n');
        CdPhut = Serial.readStringUntil('\n');
        CdGiay = Serial.readStringUntil('\n');
        DOAM = Serial.readStringUntil('\n');
      }
      _LuuThoiGian();
      Chuyenchedo = 2;
      digitalWrite(Relay, HIGH);
      if (Gio == 0 && Phut == 0 && Giay == 0)
      {
        Gio = 9; Phut = 10; Giay = 11;
        digitalWrite(Relay, LOW);
        goto batdau3;
      }

      lcd.setCursor(0, 0);
      lcd.print("2.CHE DO TIMER");
      lcd.setCursor(0, 1);
      sprintf(Chuoi, "DO AM: %02d%%", Doam, '%');
      lcd.print(Chuoi);
      lcd.setCursor(0, 2);
      sprintf(Chuoi, "GIO HEN: %02d:%02d:%02d ", Gio, Phut, Giay);
      lcd.print(Chuoi);
      if (digitalRead(MENU) == 0)
      {
        while (digitalRead(MENU) == 0);
        lcd.clear();
        CheDo = 1;
        chuyendoi = 1;
      }
      //////////////////////////////////
      if (Menu == "1")
      {
        lcd.clear();
        CheDo = Menu.toInt();
      }
      ///////////////////////////
      if (digitalRead(CHEDO) == 0)
      {
        while (digitalRead(CHEDO) == 0);
        lcd.clear();
        goto batdau4;
      }
      else if (Thaydoichedo == "3")
      {
        lcd.clear();
        goto batdau2;
      }

    }

    while (CheDo == 1) //che do cai dat gio
    {

      lcd.setCursor(0, 0);
      lcd.print("CAI DAT GIO");
      lcd.setCursor(0, 1);
      sprintf(Chuoi, "GIA TRI:%02d", Gio);
      lcd.print(Chuoi);
      if (Serial.available() > 0)
      {
        Thaydoichedo = Serial.readStringUntil('\n');
        Menu = Serial.readStringUntil('\n');
        CdTanSo = Serial.readStringUntil('\n');
        CdDuty = Serial.readStringUntil('\n');
        CdDoamcaidat = Serial.readStringUntil('\n');
        CdGio = Serial.readStringUntil('\n');
        CdPhut = Serial.readStringUntil('\n');
        CdGiay = Serial.readStringUntil('\n');
        DOAM = Serial.readStringUntil('\n');
      }
      ////////////////////////
      if (Menu == "1")
      {
        Gio = CdGio.toInt();
      }
      /////////////////////////
      if (digitalRead(TANG) == 0)
      {
        while (digitalRead(TANG) == 0);
        Gio++;
        if (Gio > 23)
        {
          Gio = 0;
        }
      }
      else if (digitalRead(GIAM) == 0)
      {
        while (digitalRead(GIAM) == 0);
        Gio--;
        if (Gio < 0)
        {
          Gio = 23;
        }
      }
else if (digitalRead(MENU) == 0)
      {
        while (digitalRead(MENU) == 0);

        lcd.clear();
        CheDo = 2;
        _LuuThoiGian();
      }
      ////////////////////////////

      if (chuyendoi == 0)
      {
        if (Menu == "2")
        {

          CheDo = Menu.toInt(); //chuyen sang che do cai dat
          lcd.clear();

        }
      }



    }

    while (CheDo == 2) //che do cai dat phut
    {
      lcd.setCursor(0, 0);
      lcd.print("CAI DAT PHUT");
      lcd.setCursor(0, 1);
      sprintf(Chuoi, "GIA TRI:%02d", Phut);
      lcd.print(Chuoi);
      ///////////////////////////

      if (Serial.available() > 0)
      {
        Thaydoichedo = Serial.readStringUntil('\n');
        Menu = Serial.readStringUntil('\n');
        CdTanSo = Serial.readStringUntil('\n');
        CdDuty = Serial.readStringUntil('\n');
        CdDoamcaidat = Serial.readStringUntil('\n');
        CdGio = Serial.readStringUntil('\n');
        CdPhut = Serial.readStringUntil('\n');
        CdGiay = Serial.readStringUntil('\n');
        DOAM = Serial.readStringUntil('\n');
      }
      if (Menu == "2")
      {
        Phut = CdPhut.toInt();
      }
      /////////////////////////

      if (digitalRead(TANG) == 0)
      {
        while (digitalRead(TANG) == 0);
        Phut++;
        if (Phut > 59)
        {
          Phut = 0;
        }
      }
      else if (digitalRead(GIAM) == 0)
      {
        while (digitalRead(GIAM) == 0);
        Phut--;
        if (Phut < 0)
        {
          Phut = 59;
        }
      }
      else if (digitalRead(MENU) == 0)
      {
        while (digitalRead(MENU) == 0);
        CheDo = 3;
        lcd.clear();
        _LuuThoiGian();
      }
      //////////////////////////
      if (chuyendoi == 0)
      {
        if (Menu == "3")
        {
          CheDo = Menu.toInt();
          lcd.clear();
          _LuuThoiGian();
        }
      }
      /////////////////////////////////////
    }

    while (CheDo == 3)
    {

      lcd.setCursor(0, 0);
      lcd.print("CAI DAT GIAY");
      lcd.setCursor(0, 1);
      sprintf(Chuoi, "GIA TRI:%02d", Giay);
      lcd.print(Chuoi);
      lcd.setCursor(0, 3);

      {
        Thaydoichedo = Serial.readStringUntil('\n');
        Menu = Serial.readStringUntil('\n');
        CdTanSo = Serial.readStringUntil('\n');
        CdDuty = Serial.readStringUntil('\n');
        CdDoamcaidat = Serial.readStringUntil('\n');
        CdGio = Serial.readStringUntil('\n');
        CdPhut = Serial.readStringUntil('\n');
        CdGiay = Serial.readStringUntil('\n');
        DOAM = Serial.readStringUntil('\n');
      }
      //////////////////////////////
      if (Menu == "3")
      {
        //Doamcaidat = CdDoamcaidat.toInt();
        Giay = CdGiay.toInt();
      }
      ////////////////////////////////////////

      if (digitalRead(TANG) == 0)
      {
        while (digitalRead(TANG) == 0);
        Giay++;
        if (Phut > 59)
        {
          Giay = 0;
        }
      }
      else if (digitalRead(GIAM) == 0)
      {
        while (digitalRead(GIAM) == 0);
        Giay--;
        if (Phut < 0)
        {
          Giay = 59;
        }
      }
      else if (digitalRead(MENU) == 0)
      {
        while (digitalRead(MENU) == 0);
        lcd.clear();
        CheDo = 0;
      }
      ////////////////////////
      if (chuyendoi == 0)
      {
        if (Menu == "0")
        {
          //while (digitalRead(MENU) == 0);
          lcd.clear();
          CheDo = Menu.toInt(); //chuyen sang che do cai dat
        }
      }
    }

  }
  /////////////////////////
  ////////////////
  while (1)
  {

batdau2:
    Trangthai = 0;
    digitalWrite(Relay, LOW);
    lcd.setCursor(0, 0);
    lcd.print("3.CHE DO OFF");
    lcd.setCursor(0, 1);
    lcd.print(Menu );
    Chuyenchedo = 3;
    _GoiDien();
    if (Serial.available() > 0)
    {
      Thaydoichedo = Serial.readStringUntil('\n');
      Menu = Serial.readStringUntil('\n');
      CdTanSo = Serial.readStringUntil('\n');
      CdDuty = Serial.readStringUntil('\n');
      CdDoamcaidat = Serial.readStringUntil('\n');
      CdGio = Serial.readStringUntil('\n');
      CdPhut = Serial.readStringUntil('\n');
      CdGiay = Serial.readStringUntil('\n');
      DOAM = Serial.readStringUntil('\n');
    }

    // _NhanTinNhan();
    if (Thaydoichedo == "1")
    {
      lcd.clear();
      goto batdau3;
    }
  }
  /////////////////////////
  ////////////////
  while (1)
  {

batdau4:
    Trangthai = 0;
    digitalWrite(Relay, LOW);
    lcd.setCursor(0, 0);
    lcd.print("3.CHE DO OFF");
    lcd.setCursor(0, 1);
    lcd.print(Menu );
    Chuyenchedo = 3;
    _GoiDien();
    if (Serial.available() > 0)
    {
      Thaydoichedo = Serial.readStringUntil('\n');
      Menu = Serial.readStringUntil('\n');
      CdTanSo = Serial.readStringUntil('\n');
      CdDuty = Serial.readStringUntil('\n');
      CdDoamcaidat = Serial.readStringUntil('\n');
      CdGio = Serial.readStringUntil('\n');
      CdPhut = Serial.readStringUntil('\n');
      CdGiay = Serial.readStringUntil('\n');
      DOAM = Serial.readStringUntil('\n');
    }

    // _NhanTinNhan();
    if (digitalRead(CHEDO) == 0)
    {
      while (digitalRead(CHEDO) == 0);
      lcd.clear();
      goto batdau3;
    }

  }
}


// Xay dung chuong trinh con
void _LuuThoiGian(void)
{
  EEPROM.write(0, Gio); //luu gia tri gio tai dia chi la 0
  EEPROM.write(1, Phut);
  EEPROM.write(2, Giay);
}

void _DocThoiGian(void)
{
  Gio = EEPROM.read(0); //doc gia tri gio tai dia chi la 0
  Phut = EEPROM.read(1);
  Giay = EEPROM.read(2);

  if (Gio > 254 || Gio < 0 || Phut < 0 || Giay < 0) //neu gia tri doc duoc la gia tri rac
  {
    Gio = 0;
    Phut = 0;
    Giay = 0;
  }
}

void _GuiTinNhan(String TinNhan)
{
  SIM7600.println("AT+CMGF=1");    // Đặt module ở chế độ văn bản
  delay(100);
  SIM7600.println("AT+CMGS=\"+0338484496\""); //số điện thoai người nhận
  delay(100);
  SIM7600.println(TinNhan);   // Nội dung gửi sms
  delay(100);
  SIM7600.println((char)26);  // ASCII code of CTRL+Z
  delay(100);
  SIM7600.println();
  delay(1000);
}
// Hàm gọi điện
void _GoiDien(void)
{
  SIM7600.println("ATD0338484496;");
  delay(100);
}
