var element;
var xmlhttpTFX;
var TFX_Step = 0;
//注意进度条依赖 element 模块，否则无法进行正常渲染和功能性操作
layui.use('element', function(){
  element = layui.element;
  
});
function TFX_CallBack(){
    if (xmlhttpTFX.readyState!=4 || xmlhttpTFX.status!=200) {
        return ;
    }
    var TFXres = JSON.parse(xmlhttpTFX.responseText);
    console.log(TFXres);
    var max = 0;
    var maxpos = 0;
    for (var i=0;i<8;i++){
        if (TFXres.outputs[0][i]>max) {
            max = TFXres.outputs[0][i];
            maxpos = i;
        }
    }
    var class_names = ["抬手","放下","手腕右旋","手腕左旋","胳膊左转","胳膊右转","胳膊右回正","胳膊左回正"]
    option_Res.series[0].data = TFXres.outputs[0];
    LSTMres.setOption(option_Res);
    document.getElementById("resTitle").innerHTML = class_names[maxpos];
    document.getElementById("resbtn").classList.remove("layui-btn-disabled");
    element.progress('demo', '100%');
}
function but(){
    switch (TFX_Step) {
        case 0:
            document.getElementById("resTitle").innerHTML = "请比划动作";
            document.getElementById("resbtn").innerHTML = "动作完成";
            element.progress('demo', '30%');
            TFX_Step = 1;
            xmlhttpTFX=new XMLHttpRequest();
            xmlhttpTFX.open("GET","http:\\\\10.102.14.92:2333\\config?AR=S",true);
            xmlhttpTFX.send();
            break;
        case 1:
            document.getElementById("resTitle").innerHTML = "正在识别";
            document.getElementById("resbtn").innerHTML = "重新开始";
            document.getElementById("resbtn").classList.add("layui-btn-disabled");
            element.progress('demo', '60%');
            xmlhttpTFX=new XMLHttpRequest();
            xmlhttpTFX.onreadystatechange = TFX_CallBack;
            xmlhttpTFX.open("GET","http:\\\\10.102.14.92:2333\\config?AR=E",true);
            xmlhttpTFX.send();
            TFX_Step = 2;
            break;
        case 2:
            document.getElementById("resbtn").innerHTML = "开始动作";
            element.progress('demo', '5%');
            TFX_Step = 0;
            break;
        default:
            break;
    }
}

var LSTMres = echarts.init(document.getElementById('LSTMres'));
var option_Res = {
    title: {
        text: '类别概率'
    },
    yAxis: {
        type: 'category',
        data: ['抬手', '放下', '右转手腕', '左转手腕', '左偏胳膊', '右回正', '右偏胳膊', "左回正"]
    },
    xAxis: {
        type: 'value'
    },
    series: [{
        data: [0, 0, 0, 0, 0, 0, 0, 0],
        type: 'bar'
    }]
};

LSTMres.setOption(option_Res);

window.onresize = function() {
    myChart.resize();
    myChart_mag.resize();
    myChart_gyro.resize();
    LSTMres.resize();
};

document.onkeydown = (e)=>{
    var keyCode = e.keyCode || e.which || e.charCode;
    if (keyCode==83){
        but();
    }
};