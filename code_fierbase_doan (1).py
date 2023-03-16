#Khai báo các thư viện thời gian, firebase, gửi mail.
import serial
import time
import pyrebase
import yagmail
#API kết nối với firebase
config= {
"apiKey": "AIzaSyCPK2HuHYSyqecfLIK7w3_Ggojgh3Ka-2Q",
"authDomain": "he-thong-tuoi-nuoc-tu-dong.firebaseapp.com",
"databaseURL":"https://he-thong-tuoi-nuoc-tu-dong-default-rtdb.firebaseio.com" ,
"storageBucket": "he-thong-tuoi-nuoc-tu-dong.appspot.com"
}
 #Hàm kết nối với firebase
firebase = pyrebase.initialize_app(config)
db = firebase.database()
# khởi tạo đối tượng giao tiếp serial giữa arduino và pi
ser = serial.Serial('/dev/ttyACM1',9600)
# xóa dữ liệu trước đó
ser.reset_input_buffer()
print("Serial OK")
# Tạo biến toàn cục
Doam=25
Duty=50
tanso=100
Thaydoichedo=1
Tang = 0
Giam = 0
Menu = 1
CdGio=5;
CdPhut=5;
CdGiay=5;
CdDACD=35;
CdTanso=50;
CdDuty=10;
GUIMAIL=1;
Gmail="nguyenvannam1582k1@gmail.com"
try:
    while True:
        if ser.in_waiting > 0: #nếu nhận tín hiệu gửi từ arduino  
            Doam = int(ser.readline().decode('utf-8').rstrip()) # đọc dữ liệu từ arduino bỏ các ký tự /n, khoảng trắng... và ép về kiểu nguyên
            print("Doam: "+str(Doam))
            Duty = int(ser.readline().decode('utf-8').rstrip())
            print("Duty: "+str(Duty))
            TanSo=int(ser.readline().decode('utf-8').rstrip())
            print("TanSo: "+str(TanSo))
            
            Doamcaidat=int(ser.readline().decode('utf-8').rstrip())
            print("Doamcaidat: "+str(Doamcaidat))
            Gio=int(ser.readline().decode('utf-8').rstrip())
            print("Gio: "+str(Gio))
            Phut=int(ser.readline().decode('utf-8').rstrip())
            print("Phut: "+str(Phut))
            Giay=int(ser.readline().decode('utf-8').rstrip())
            print("Giay: "+str(Giay))
            Trangthai=int(ser.readline().decode('utf-8').rstrip())
            print("Trangthai: "+str(Trangthai))
            Chuyenchedo=int(ser.readline().decode('utf-8').rstrip())
            print("Chuyenchedo: "+str(Chuyenchedo))
            Menu=int(ser.readline().decode('utf-8').rstrip())
            print(str(Menu))
            # tạo dữ liệu trên firebase
            dulieu1 = {"Doamcaidat":Doamcaidat ,"TanSo":TanSo,"Duty":Duty}
            dulieu2 = {"Giay":Giay ,"Gio":Gio,"Phut":Phut}
            dulieu3 = {"Trangthai":Trangthai}
            dulieu4 = {"Chuyenchedo":Chuyenchedo}
            dulieu5 = {"Thaydoichedo":Thaydoichedo,"Menu":Menu,"CdGio":CdGio,"CdPhut":CdPhut,"CdGiay":CdGiay,"CdDACD":CdDACD,"CdDuty":CdDuty,"CdTanso":CdTanso,"Gmail":Gmail}
            
            # lấy dữ liệu về trong key cần lấy giá trị
            Menutam = db.child("BienThayDoi").child("Menu").get()
            Menu=Menutam.val()
            # các biến dùng để hiển thị lên web.app
            Thaydoichedotam = db.child("BienThayDoi").child("Thaydoichedo").get() # lấy về key cần lấy giá trị
            Thaydoichedo=Thaydoichedotam.val() #lấy giá trị từ key
            CdDACDtam = db.child("BienThayDoi").child("CdDACD").get()
            CdDACD=CdDACDtam.val()
            CdDutytam = db.child("BienThayDoi").child("CdDuty").get()
            CdDuty=CdDutytam.val()
            CdTansotam = db.child("BienThayDoi").child("CdTanso").get()
            CdTanso=CdTansotam.val()
            CdGiotam = db.child("BienThayDoi").child("CdGio").get()
            CdGio=CdGiotam.val()
            CdPhuttam = db.child("BienThayDoi").child("CdPhut").get()
            CdPhut=CdPhuttam.val()
            CdGiaytam = db.child("BienThayDoi").child("CdGiay").get()
            CdGiay=CdGiaytam.val()
            # lấy mail để gửi
            Gmailtam=db.child("BienThayDoi").child("Gmail").get()
            Gmail=Gmailtam.val()
            DOAMtam = db.child("Data").child("doam").get()
            DOAM = DOAMtam.val()
            print("Doam : " + str(DOAM))
            
            
            
            if(Doam>Doamcaidat):
                if(GUIMAIL==1):
                    password = "" # khỏi tạo biến passwword
                    with open("/home/pi/.local/share/.email_password","r") as f: # mở file đã lưu passwword để đọc
                        password=f.read()
                    yag = yagmail.SMTP('caoquamanh@gmail.com',password)
                    yag.send(to=Gmail,
                             subject="HE THONG TUOI NUOC TU DONG",
                             contents="Dam hien tai: "+str(Doam)+"% Do am dat yeu cau")
                    print("da gui mail")
                GUIMAIL=0;
            elif(Doam<=Doamcaidat):
                if(GUIMAIL==0):
                    password = ""
                    with open("/home/pi/.local/share/.email_password","r") as f:
                        password=f.read()
                    yag = yagmail.SMTP('caoquamanh@gmail.com',password)
                    yag.send(to=Gmail,
                             subject="HE THONG TUOI NUOC TU DONG",
                             contents="Dam hien tai: "+str(Doam)+"% Do am thap")
                    print("da gui mail")
                
                GUIMAIL=1
            
            print("Thaydoichedo: " +str(Thaydoichedo)+ " Menu: "+str(Menu)+ " Tang: "+str(Tang)+ " Giam: "+str(Giam) )
                    
            
            # Gửi dữ liệu qua arduino
            ser.write((str(Thaydoichedo)+"\n").encode('utf-8'))
            ser.write((str(Menu)+"\n").encode('utf-8'))
            ser.write((str(CdTanso)+"\n").encode('utf-8'))
            ser.write((str(CdDuty)+"\n").encode('utf-8'))
            ser.write((str(CdDACD)+"\n").encode('utf-8'))
            ser.write((str(CdGio)+"\n").encode('utf-8'))
            ser.write((str(CdPhut)+"\n").encode('utf-8'))
            ser.write((str(CdGiay)+"\n").encode('utf-8'))
            ser.write((str(DOAM)+"\n").encode('utf-8'))
        
            # Khởi tạo dữ liệu trên firebase set liên tục
            db.child("BienChuyenDoi").set(dulieu4)
            db.child("CheDoOff").set(dulieu3)
            db.child("CheDoTimer").set(dulieu2)
            db.child("CheDoTuDong").set(dulieu1)
    db.child("BienThayDoi").set(dulieu5)# dat lai key database thu cong 
            
except KeyboardInterrupt:
    ser.close()
