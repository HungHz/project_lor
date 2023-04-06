
const firebaseConfig = {
  apiKey: "AIzaSyBqgXixi0zL_MNF6XM-5lEhnDIpDJo0p58",
  authDomain: "mypro-593ab.firebaseapp.com",
  databaseURL: "https://mypro-593ab-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "mypro-593ab",
  storageBucket: "mypro-593ab.appspot.com",
  messagingSenderId: "265908943733",
  appId: "1:265908943733:web:fce89a4f8f828bb33db13c",
  measurementId: "G-Q1F47MYGT0"
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);
firebase.analytics();

//time
function dongho(){
  var date = new Date();
  var gio = date.getHours();
  var phut = date.getMinutes();
  var giay = date.getSeconds();
  var ngay = date.getDate();
  var thang = date.getMonth() + 1;
  var nam = date.getFullYear();

  if(gio<10){
    gio = "0" + gio;
  }
  if(phut<10){
    phut = "0" + phut;
  }
  if(giay<10){
    giay = "0" + giay;
  }
  if(ngay<10){
    ngay = "0" + ngay;
  }
  if(thang<10){
    thang = "0" + thang;
  }

  document.getElementById("oclock").innerHTML = gio + ":" + phut + ":" + giay;
  document.getElementById("current-time").innerHTML = ngay + "/" + thang + "/" + nam;
  setTimeout("dongho()",1000);
}
dongho();
//temp + humi
  var temp = document.getElementById('giatrinhietdo');
  var dbTemp = firebase.database().ref().child('dht').child("temp");
  var humi = document.getElementById('giatridoam');
  var dbHumi = firebase.database().ref().child('dht').child("humi");
  dbTemp.on('value', snap => temp.innerText = snap.val()+ " °C");
  dbHumi.on('value', snap => humi.innerText = snap.val()+ " %");
  

//
  var aqiCoclor = [
    "rgb(0,228,0)",
    "rgb(255,255,0)",
    "rgb(255,126,0)",
    "rgb(255,0,0)",
    "rgb(143,63,151)",
    "rgb(126,0,35)"
  ]

function getAndShowAqi(nameChild, tenGiaTri, tenBox, aqiCoclor, unit){ //(string,string,string,array,string)
  var dbNameIndex = firebase.database().ref().child('air').child(nameChild);
  dbNameIndex.on("value",function(snapshot){
    var nameIndex = Math.round(snapshot.val());
    if(nameIndex<=50) // tot
    {
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[0];
    }
    else if(50<nameIndex && nameIndex<=100){ // trung binh
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[1];
    }
    else if(100<nameIndex && nameIndex<=150){ //kem
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[2];
    }
    else if(150<nameIndex && nameIndex<=200){ //xau
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[3];
    }
    else if(200<nameIndex && nameIndex<=300){ //rat xau
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[4];
    }
    else{
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[5];
    }
  })
}

function getAndShowPm(nameChild, tenGiaTri, tenBox, aqiCoclor, unit){ //(string,string,string,array,string)
  var dbNameIndex = firebase.database().ref().child('air').child(nameChild);
  dbNameIndex.on("value",function(snapshot){
    var nameIndex = Math.round(snapshot.val());
    if(nameIndex<=12) // tot
    {
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[0];
    }
    else if(12<nameIndex && nameIndex<=35){ // trung binh
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[1];
    }
    else if(35<nameIndex && nameIndex<=55){ //kem
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[2];
    }
    else if(55<nameIndex && nameIndex<=150){ //xau
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[3];
    }
    else if(150<nameIndex && nameIndex<=250){ //rat xau
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[4];
    }
    else{
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[5];
    }
  })
}

function getAndShowCo(nameChild, tenGiaTri, tenBox, aqiCoclor, unit){ //(string,string,string,array,string)
  var dbNameIndex = firebase.database().ref().child('air').child(nameChild);
  dbNameIndex.on("value",function(snapshot){
    var nameIndex = snapshot.val().toFixed(2);
    if(nameIndex<=4.4) // tot
    {
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[0];
    }
    else if(4.4<nameIndex && nameIndex<=9.4){ // trung binh
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[1];
    }
    else if(9.4<nameIndex && nameIndex<=12.4){ //kem
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[2];
    }
    else if(12.4<nameIndex && nameIndex<=15.4){ //xau
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[3];
    }
    else if(15.4<nameIndex && nameIndex<=30.4){ //rat xau
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[4];
    }
    else{
      document.getElementById(tenGiaTri).innerText = nameIndex + unit;
      document.getElementById(tenBox).style.background =  aqiCoclor[5];
    }
  })
}
getAndShowPm("pm","giatripm","pm2_5",aqiCoclor," ug/m3")
getAndShowCo("co","giatrico","co",aqiCoclor, " ppm")
getAndShowAqi("aqi","giatriaqi","aqi",aqiCoclor,"")
