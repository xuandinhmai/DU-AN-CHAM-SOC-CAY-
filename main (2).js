//API kết nối với firebase
const firebaseConfig = {
  apiKey: "AIzaSyCPK2HuHYSyqecfLIK7w3_Ggojgh3Ka-2Q",
  authDomain: "he-thong-tuoi-nuoc-tu-dong.firebaseapp.com",
  databaseURL: "https://he-thong-tuoi-nuoc-tu-dong-default-rtdb.firebaseio.com",
  projectId: "he-thong-tuoi-nuoc-tu-dong",
  storageBucket: "he-thong-tuoi-nuoc-tu-dong.appspot.com",
  messagingSenderId: "502553564548",
  appId: "1:502553564548:web:83d762000b914baa14f9d1",
  measurementId: "G-P5CB7ZRVEZ"
};
//Hàm kết nối với firebase
firebase.initializeApp(firebaseConfig);

var db = firebase.database();
// đặt các nút theo id btn1...4
var buttonCheDo = document.getElementById("btn4");
var buttonMenu = document.getElementById("btn3");
var buttonTang = document.getElementById("btn2");
var buttonGiam = document.getElementById("btn1");

var Thaydoichedo=1;
var Menu=0;
var Tang=0;
var Giam=0;
var CdGio=9;
var CdPhut=10;
var CdGiay=11;
var CdDACD=35;
var CdTanso=50;
var CdDuty=10;

/////////////// Hien thi che do lam viec
db.ref("/BienChuyenDoi/Chuyenchedo").on("value",function(snapshot){
  var Chuyenchedo = snapshot.val();

});
////////////////////////// nut nhan chuyen che doi 
buttonCheDo.onclick = function(){
  Thaydoichedo++;
  if(Thaydoichedo>3){
    Thaydoichedo=1;
  }
  if (Thaydoichedo==1) {
  document.getElementById("Chuyenchedo").innerHTML = "Đang ở CheDoTuDong";

  }
  else if(Thaydoichedo==2){
    document.getElementById("Chuyenchedo").innerHTML = "Đang ở CheDoTimer";
  }
  else if(Thaydoichedo==3){
    document.getElementById("Chuyenchedo").innerHTML = "Đang ở CheDoOff";
  }
  db.ref("/BienThayDoi").update({"Thaydoichedo":Thaydoichedo});
  setTimeout(
  () => {
    ;
  },
  500
);
};
////////////////////////// nut nhan Menu
buttonMenu.onclick = function(){
  Menu++;
  if(Menu>3){
    Menu=0;
  }
  db.ref("/BienThayDoi").update({"Menu":Menu});
  setTimeout(
  () => {
    ; 
  },
  50
);
};
////////////////////////// nut nhan Tang
buttonTang.onclick = function(){
  
 if(Thaydoichedo==1)
 {
  if(Menu==1)
  {

        CdTanso = CdTanso + 50;
        db.ref("/BienThayDoi").update({"CdTanso":CdTanso});
        if (CdTanso > 1500)
        {
          CdTanso = 50;
        }
  }
  else if(Menu==2)
  {

        CdDuty = CdDuty + 5;
        db.ref("/BienThayDoi").update({"CdDuty":CdDuty});
        if (CdDuty > 95)
        {
          CdDuty = 10;
        }
  }
  else if(Menu==3)
  {

        CdDACD = CdDACD + 1;
        db.ref("/BienThayDoi").update({"CdDACD":CdDACD});
        if (CdDuty > 80)
        {
          CdDuty = 10;
}
  }

 }
 if(Thaydoichedo==2)
 {
  if(Menu==1)
  {
    CdGio=CdGio + 1;
    db.ref("/BienThayDoi").update({"CdGio":CdGio});
    if(CdGio>23)
    {
      CdGio=0;
    }
  }
  else if(Menu==2)
  {
    CdPhut=CdPhut + 1;
    db.ref("/BienThayDoi").update({"CdPhut":CdPhut});
    if(CdPhut>59)
    {
      CdPhut=0;
    }
  }
  else if(Menu==3)
  {
    CdGiay=CdGiay+1;
    db.ref("/BienThayDoi").update({"CdGiay":CdGiay});
    if (CdGiay>59) 
    {
      CdGiay=0;
    }
  }
 }
};
////////////////////////// nut nhan Giam
buttonGiam.onclick = function(){
  
 if(Thaydoichedo==1)
 {
  if(Menu==1)
  {

        CdTanso = CdTanso - 50;
        db.ref("/BienThayDoi").update({"CdTanso":CdTanso});
        if (CdTanso < 50)
        {
          CdTanso = 1500;
        }
  }
  else if(Menu==2)
  {

        CdDuty = CdDuty - 5;
        db.ref("/BienThayDoi").update({"CdDuty":CdDuty});
        if (CdDuty < 10)
        {
          CdDuty = 95;
        }
  }
  else if(Menu==3)
  {

        CdDACD = CdDACD - 1;
        db.ref("/BienThayDoi").update({"CdDACD":CdDACD});
        if (CdDuty < 10)
        {
          CdDuty = 80;
        }
  }

 }
 if(Thaydoichedo==2)
 {
  if(Menu==1)
  {
    CdGio=CdGio - 1;
    db.ref("/BienThayDoi").update({"CdGio":CdGio});
    if(CdGio<0)
    {
      CdGio=23;
    }
  }
  else if(Menu==2)
  {
    CdPhut=CdPhut - 1;
    db.ref("/BienThayDoi").update({"CdPhut":CdPhut});
    if(CdPhut<0)
    {
      CdPhut=59;
    }
  }
  else if(Menu==3)
  {
    CdGiay=CdGiay-1;
    db.ref("/BienThayDoi").update({"CdGiay":CdGiay});
    if (CdGiay<0) 
    {
      CdGiay=59;
    }
  }
 }
};

////////////////////////////////////////////////////////////
db.ref("/BienThayDoi/CdDACD").on("value",function(snapshot){
  var htCdDACD = snapshot.val();
  document.getElementById("CdDACD").innerHTML = htCdDACD;
});
db.ref("/BienThayDoi/CdTanso").on("value",function(snapshot){
  var htCdTanso= snapshot.val();
  document.getElementById("CdTanso").innerHTML =htCdTanso ;
});
db.ref("/BienThayDoi/CdDuty").on("value",function(snapshot){
  var htCdDuty= snapshot.val();
  document.getElementById("CdDuty").innerHTML =htCdDuty ;
});
db.ref("/BienThayDoi/CdGio").on("value",function(snapshot){
  var htCdTGio= snapshot.val();
  document.getElementById("CdGio").innerHTML =htCdTGio ;
});
db.ref("/BienThayDoi/CdPhut").on("value",function(snapshot){
  var htCdPhut= snapshot.val();
  document.getElementById("CdPhut").innerHTML =htCdPhut ;
});
db.ref("/BienThayDoi/CdGiay").on("value",function(snapshot){
  var htCdGiay= snapshot.val();
  document.getElementById("CdGiay").innerHTML =htCdGiay ;
});

///////////////////////////////////////////////
// lấy dữ liệu từ firebase cập nhật lên webapp
// lấy dữ liệu từ cơ sở dữ liệu thời gian thực database 
//ref(): 
db.ref("/Data/doam").on("value",function(snapshot){
  var Doam = snapshot.val();
  //cập nhật hiển thị trên web trong file index.html với thông số id ="Doam"
document.getElementById("Doam").innerHTML = Doam;
});
db.ref("/CheDoTuDong/Doamcaidat").on("value",function(snapshot){
  var Doamcaidat = snapshot.val();
  document.getElementById("Doamcaidat").innerHTML = Doamcaidat;
});
db.ref("/CheDoTuDong/Duty").on("value",function(snapshot){
  var Duty = snapshot.val();
  document.getElementById("Duty").innerHTML = Duty;
});
db.ref("/CheDoTuDong/TanSo").on("value",function(snapshot){
  var Tanso = snapshot.val();
  document.getElementById("Tanso").innerHTML = Tanso;
});
db.ref("/CheDoTimer/Gio").on("value",function(snapshot){
  var Gio = snapshot.val();
  document.getElementById("Gio").innerHTML = Gio;
})
;db.ref("/CheDoTimer/Phut").on("value",function(snapshot){
  var Phut = snapshot.val();
  document.getElementById("Phut").innerHTML = Phut;
});
db.ref("/CheDoTimer/Giay").on("value",function(snapshot){
  var Giay = snapshot.val();
  document.getElementById("Giay").innerHTML = Giay;
});
db.ref("/CheDoOff/Trangthai").on("value",function(snapshot){
  var Trangthai = snapshot.val();
  document.getElementById("Trangthai").innerHTML = Trangthai;
});
db.ref("/CheDoOff/Trangthai").on("value",function(snapshot){
  var Trangthai = snapshot.val();
  document.getElementById("Trangthai").innerHTML = Trangthai;
});
/////////////////////////////////////////////////////////////////////