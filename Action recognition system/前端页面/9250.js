// 基于准备好的dom，初始化echarts实例
window.onresize = function() {
    var obj = document.getElementById("main");
    var style = null;
    if (window.getComputedStyle) {
        style = window.getComputedStyle(obj, null); // 非IE
    } else {
        style = obj.currentStyle; // IE
    }
    console.log("width=" + style.width + "\nheight=" + style.height);
    myChart.resize();
    myChart_mag.resize();
    myChart_gyro.resize();
    LSTMres.resize();
};
var myChart = echarts.init(document.getElementById('main'));

// 指定图表的配置项和数据
var option = {
    title: {
        text: 'MPU9250 加速度表'
    },
    animation:false,
    grid:{
    top:"15%",
    bottom:"10%"
    },
    tooltip: {},
    legend: {
        data:['X轴','Y轴','Z轴'],
        right:"10%"
    },
    xAxis: {
        data: ["","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
        "","","","","","","","","","","","","","","","","","","","",""]
    },
    yAxis: {},
    series: [{
        name: 'X轴',
        type: 'line',
        smooth: true,
        data: []
        ,symbol:"none"
    },{
    name: 'Y轴',
    type: 'line',
    smooth: true,
    data: []
    ,symbol:"none"
    },{
    name: 'Z轴',
    type: 'line',
    smooth: true,
    data: []
    ,symbol:"none"
    }]
};

// 使用刚指定的配置项和数据显示图表。
myChart.setOption(option);
var xmlhttplist;
var currentJSONModel;
var MPUdate_accX=[];
var MPUdate_accY=[];
var MPUdate_accZ=[];
function loadMPUData()
{
    //load list
    xmlhttplist=new XMLHttpRequest();
    xmlhttplist.onreadystatechange = loadMPUData_Callback;
    xmlhttplist.open("GET","http:\\\\10.102.14.92:2333",true);
    xmlhttplist.send();
}
function loadMPUData_Callback(){
if (xmlhttplist.readyState!=4 || xmlhttplist.status!=200) {
    return ;
}
currentJSONModel = JSON.parse(xmlhttplist.responseText);
MPUdate_accX.unshift(currentJSONModel.accX);
MPUdate_accX.pop();
option.series[0].data = MPUdate_accX;
MPUdate_accY.unshift(currentJSONModel.accY);
MPUdate_accY.pop();
option.series[1].data = MPUdate_accY;
MPUdate_accZ.unshift(currentJSONModel.accZ);
MPUdate_accZ.pop();
option.series[2].data = MPUdate_accZ;
myChart.setOption(option);
MPUdate_gyroX.unshift(currentJSONModel.gyroX);
MPUdate_gyroX.pop();
option_gyro.series[0].data = MPUdate_gyroX;
MPUdate_gyroY.unshift(currentJSONModel.gyroY);
MPUdate_gyroY.pop();
option_gyro.series[1].data = MPUdate_gyroY;
MPUdate_gyroZ.unshift(currentJSONModel.gyroZ);
MPUdate_gyroZ.pop();
option_gyro.series[2].data = MPUdate_gyroZ;
myChart_gyro.setOption(option_gyro);
MPUdate_magX.unshift(currentJSONModel.magX);
MPUdate_magX.pop();
option_mag.series[0].data = MPUdate_magX;
MPUdate_magY.unshift(currentJSONModel.magY);
MPUdate_magY.pop();
option_mag.series[1].data = MPUdate_magY;
MPUdate_magZ.unshift(currentJSONModel.magZ);
MPUdate_magZ.pop();
option_mag.series[2].data = MPUdate_magZ;
myChart_mag.setOption(option_mag);
}
for (var i=0;i<100;i++)
MPUdate_accX.push(20);
for (var i=0;i<100;i++)
MPUdate_accY.push(20);
for (var i=0;i<100;i++)
MPUdate_accZ.push(20);
  
// 基于准备好的dom，初始化echarts实例
var myChart_gyro = echarts.init(document.getElementById('gyro'));
// 指定图表的配置项和数据
var option_gyro = {
    title: {
        text: 'MPU9250 陀螺仪表'
    },
    animation:false,
    tooltip: {},
    grid:{
    top:"15%",
    bottom:"10%"
    },
    legend: {
        data:['X轴','Y轴','Z轴'],
        right:"10%"
    },
    xAxis: {
        data: ["","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
        "","","","","","","","","","","","","","","","","","","","",""]
    },
    yAxis: {},
    series: [{
        name: 'X轴',
        type: 'line',
        smooth: true,
        data: []
        ,symbol:"none"
    },{
    name: 'Y轴',
    type: 'line',
    smooth: true,
    data: []
    ,symbol:"none"
    },{
    name: 'Z轴',
    type: 'line',
    smooth: true,
    data: []
    ,symbol:"none"
    }]
};
// 使用刚指定的配置项和数据显示图表。
myChart_gyro.setOption(option_gyro);
var xmlhttplist_gyro;
var currentJSONModel_gyro;
var MPUdate_gyroX=[];
var MPUdate_gyroY=[];
var MPUdate_gyroZ=[];
for (var i=0;i<100;i++)
MPUdate_gyroX.push(20);
for (var i=0;i<100;i++)
MPUdate_gyroY.push(20);
for (var i=0;i<100;i++)
MPUdate_gyroZ.push(20);

// 基于准备好的dom，初始化echarts实例
var myChart_mag = echarts.init(document.getElementById('mag'));

// 指定图表的配置项和数据
var option_mag = {
    title: {
        text: 'MPU9250 磁力计'
    },
    animation:false,
    tooltip: {},
    legend: {
        data:['X轴','Y轴','Z轴'],
        right:"10%"
    },
    grid:{
    top:"15%",
    bottom:"10%"
    },
    xAxis: {
        data: ["","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
        "","","","","","","","","","","","","","","","","","","","",""]
    },
    yAxis: {},
    series: [{
        name: 'X轴',
        type: 'line',
        smooth: true,
        data: []
        ,symbol:"none"
    },{
    name: 'Y轴',
    type: 'line',
    smooth: true,
    data: []
    ,symbol:"none"
    },{
    name: 'Z轴',
    type: 'line',
    smooth: true,
    data: []
    ,symbol:"none"
    }]
};
// 使用刚指定的配置项和数据显示图表。
myChart_mag.setOption(option_mag);
var xmlhttplist_mag;
var currentJSONModel_mag;
var MPUdate_magX=[];
var MPUdate_magY=[];
var MPUdate_magZ=[];
for (var i=0;i<100;i++)
MPUdate_magX.push(20);
for (var i=0;i<100;i++)
MPUdate_magY.push(20);
for (var i=0;i<100;i++)
MPUdate_magZ.push(20);


var RecardState = 0;
document.onkeydown = (e)=>{
    var keyCode = e.keyCode || e.which || e.charCode;
    if (keyCode<49||keyCode>56) if (keyCode!=81) return;
    console.log(keyCode);
    var btmBegin = document.getElementById("recardBtm_begin");
    var btmStop = document.getElementById("recardBtm_stop");
    var recardTitle = document.getElementById("recardText");
    if (keyCode>=49&&keyCode<=56){
        recardTitle.innerHTML = `正在录制 ${keyCode-48}`;
        btmBegin.hidden = true;
        btmStop.hidden = false;
    }else{
        recardTitle.innerHTML = '录制结束';
        btmBegin.hidden = false;
        btmStop.hidden = true;
    }
    xmlhttpFC=new XMLHttpRequest();
    xmlhttpFC.open("GET",`http:\\\\10.102.14.92:2333\\config?AC=${keyCode}`,true);
    xmlhttpFC.send();
};
  
layui.use('slider', function(){
var slider = layui.slider;
//渲染
slider.render({
    elem: '#FilterSlideTestACC'  //绑定元素
    ,value: 999
    ,min:10
    ,max:999
    ,change:(value)=>{
    console.log(value);
    xmlhttpFC=new XMLHttpRequest();
    xmlhttpFC.open("GET",`http:\\\\10.102.14.92:2333\\config?FC=A${value}`,true);
    xmlhttpFC.send();
    }
});
slider.render({
    elem: '#FilterSlideTestGYR'  //绑定元素
    ,value: 999
    ,min:10
    ,max:999
    ,change:(value)=>{
    console.log(value);
    xmlhttpFC=new XMLHttpRequest();
    xmlhttpFC.open("GET",`http:\\\\10.102.14.92:2333\\config?FC=G${value}`,true);
    xmlhttpFC.send();
    }
});
slider.render({
    elem: '#FilterSlideTestMAG'  //绑定元素
    ,value: 999
    ,min:10
    ,max:999
    ,change:(value)=>{
    console.log(value);
    xmlhttpFC=new XMLHttpRequest();
    xmlhttpFC.open("GET",`http:\\\\10.102.14.92:2333\\config?FC=M${value}`,true);
    xmlhttpFC.send();
    }
});
});
  
setInterval(()=>{loadMPUData()},30);


var xmlhttpTF;
function CallModel()
{
    //load list
    var data='{"instances":[[[2449.0, -8958.0, 14177.0, 182.0, 189.0, 754.0], [2214.0, -8878.0, 14115.0, -110.0, 490.0, 874.0]]]}';
    xmlhttpTF=new XMLHttpRequest();
    xmlhttpTF.onreadystatechange = CallBack_TF;
    xmlhttpTF.open("GET","http://10.102.14.92:8501/v1/models/SuBandv2",true);
    //xmlhttpTF.setRequestHeader("Content-Type", "application/json");
    //xmlhttpTF.setRequestHeader("Access-Control-Allow-Origin", "*");
    xmlhttpTF.send(data);
}
function CallBack_TF()
{
    if (xmlhttpTF.readyState!=4 || xmlhttpTF.status!=200) {
        return ;
    }
    console.log(CallBack_TF.responseText);
}

var xmlhttpTFX;
var ActionRecognition_status = 0;
function ActionRecognition_start(){
    if (ActionRecognition_status==0){
        document.getElementById("ActionRecognition").innerHTML = "动作结束，开始识别";
        ActionRecognition_status = 1;
        xmlhttpTFX=new XMLHttpRequest();
        xmlhttpTFX.open("GET","http:\\\\10.102.14.92:2333\\config?AR=S",true);
        xmlhttpTFX.send();
    }else{
        ActionRecognition_status = 0;
        document.getElementById("ActionRecognition").innerHTML = "开始动作";
        xmlhttpTFX=new XMLHttpRequest();
        xmlhttpTFX.open("GET","http:\\\\10.102.14.92:2333\\config?AR=E",true);
        xmlhttpTFX.send();
    }
}