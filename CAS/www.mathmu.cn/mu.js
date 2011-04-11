var uniqID=0;
function getXMLHttpRequest(){
    if(window.ActiveXObject){
        var ieArr=["Msxml2.XMLHTTP.6.0","Msxml2.XMLHTTP.3.0", "Msxml2.XMLHTTP","Microsoft.XMLHTTP"];                
        for(var i=0;i<ieArr.length;i++)
        {
            var xmlhttp= new ActiveXObject(ieArr[i]);
        }
        return xmlhttp;
    } else if(window.XMLHttpRequest){
        return new XMLHttpRequest();
    }            
}
var xhr;
window.onload = function(){
    // alert("onload function");
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=login&&rand='+parseInt(Math.random()*10000000+1),true);
    xhr.onreadystatechange=function(){
        if(xhr.readyState == 4){
            if(xhr.status == 200){
                uniqID=xhr.responseText;
                //        alert(uniqID);
                var s=document.getElementById("input0").value.match(/\n/g);
                if(s){
                    document.getElementById("input0").rows=s.length+1;
                }
                document.getElementById("all").style.display="block";
                Layer_Loading.style.visibility="hidden";
            }
        }
    }
    xhr.send(null);
}
window.onunload = function(){
    //alert("onunload function");
    var xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=logout&&userID='+uniqID,false);
    xhr.send(null);

}


document.onkeydown = function(event)
{
    var keyEvent=event || window.event;   
    var keyCode=keyEvent.keyCode ? keyEvent.keyCode : keyEvent.which ? keyEvent.which : keyEvent.charCode;   
    if ((keyEvent.shiftKey)&&(keyCode==0x0D)) {sendMessage(); }
}

/*
   document.getElementById("input0").onkeydown = function(event)
   {
   alert('hei  u  press');
   }
   */

var maxID=0;
var maxPicID=0;
var ID=0;

// get the type of browser
var Sys = {};
var ua = navigator.userAgent.toLowerCase();
var s;
(s = ua.match(/msie ([\d.]+)/)) ? Sys.ie = s[1] :
(s = ua.match(/firefox\/([\d.]+)/)) ? Sys.firefox = s[1] :
(s = ua.match(/chrome\/([\d.]+)/)) ? Sys.chrome = s[1] :
(s = ua.match(/opera.([\d.]+)/)) ? Sys.opera = s[1] :
(s = ua.match(/version\/([\d.]+).*safari/)) ? Sys.safari = s[1] : 0;

/*method
  if (Sys.ie) document.write('IE: ' + Sys.ie);
  if (Sys.firefox) document.write('Firefox: ' + Sys.firefox);
  if (Sys.chrome) document.write('Chrome: ' + Sys.chrome);
  if (Sys.opera) document.write('Opera: ' + Sys.opera);
  if (Sys.safari) document.write('Safari: ' + Sys.safari);
  */
// end of get
function inputKey(event)
{
    idInfo=document.activeElement.id;
    //alert( idInfo);

    var keyEvent=event || window.event;   
    var keyCode=keyEvent.keyCode ? keyEvent.keyCode : keyEvent.which ? keyEvent.which : keyEvent.charCode;   
    if ((keyEvent.shiftKey)&&(keyCode==0x0D)) {return false;}
    else if ((keyEvent.ctrlKey)&&(keyCode==0x56)){return false;}
    else if (keyCode==0x0D)
    {

        if (idInfo.search("input")==-1)
        {
            return false;
        }
        document.getElementById( idInfo).rows++;
        if(Sys.firefox)
        {
     //             document.getElementById( idInfo).value+="\n";
        }
        return true;
    }
    else
    {

        var s=document.getElementById( idInfo).value.match(/\n/g);
        //alert( s.length);
        if(s)
        {
            tmpvalue=document.getElementById( idInfo).value;
            //alert(tmpvalue.length);
            if (keyCode==0x08&&tmpvalue.charAt(tmpvalue.length-1)=="\n")
            {
                document.getElementById( idInfo).rows=s.length;

            }
            else
            {
                document.getElementById( idInfo).rows=s.length+1;

            }
        }
    }
    return true;
}
function sendMessage(){
    idInfo=document.activeElement.id;
    if (idInfo.search("input")==-1)
    {
     if (idInfo.search("inButton")==-1)
     {
        return;
     }
     else
     {
         idInfo=idInfo.replace(/inButton/,"");
          ID=idInfo;
     }
    }
    else
    {
    idInfo=idInfo.replace(/input/,"");
    ID=idInfo;
    }

var img = document.getElementById('img_output'+ID);
if (img)
{
    document.getElementById('output'+ID).removeChild(img);
}



//   img=document.getElementById('img_output_rongyu'+maxID);
              //  document.getElementById('output'+maxID).removeChild(img);
              //  img=document.getElementById('img_browse_rongyu'+maxID);
             //  document.getElementById('browse'+maxID).removeChild(img);
img=document.getElementById('img_output_rongyu'+ID);
if (img)
{
    document.getElementById('output'+ID).removeChild(img);
}
img=document.getElementById('img_browse_rongyu'+ID);
if (img)
{
    document.getElementById('browse'+ID).removeChild(img);
}






    img=document.createElement("img");
    img.src="./images/loading.gif";
    img.id="img_output"+ID;
    
    document.getElementById('output'+ID).appendChild(img);


    img = document.getElementById('img_input'+ID);
if(img)
{
    document.getElementById('browse'+ID).removeChild(img);
}
    img=document.createElement("img");
    img.src="./images/loading.gif";
    img.id="img_input"+ID;

    document.getElementById('browse'+ID).appendChild(img);

                
    var cmd=document.getElementById('input'+ID).value;
    //alert(cmd);
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=exec&&userID='+uniqID+'&&id='+maxPicID+'&&cmd='+encodeURIComponent(cmd),true);
    maxPicID=maxPicID+1;
    xhr.onreadystatechange=getMessage;
    xhr.send(null);
}
function getMessage()
{
    if(xhr.readyState == 4) {
        if(xhr.status == 200) {
            if(ID==maxID)
            {
                var img=document.getElementById('img_output'+ID)
                document.getElementById('output'+ID).removeChild(img);


                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
                img.id="img_output"+ID;
                document.getElementById('output'+ID).appendChild(img);
                maxID=maxID+1;
                
                img=document.getElementById('img_input'+ID)
                document.getElementById('browse'+ID).removeChild(img);


                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[0];
                img.id="img_input"+ID;
                document.getElementById('browse'+ID).appendChild(img);

            


                //<textarea rows="1" cols="95" class="terminals_input" id="input0" value="" onkeydown="return inputKey(event)" ></textarea>



               var tmpTable=document.createElement("table");
               tmpTable.className="terminals_input_help_bg";
               
               var tmpTr=document.createElement("tr");
               var tmpTd=document.createElement("td");
               tmpTd.className="terminals_input_help_bg";
               /*
               var tmpDiv = document.createElement("div");
               tmpDiv.className="terminals_input_help_text";
               tmpDiv.innerHTML="输入：";
               */
                var tmpButton=document.createElement("button");
                tmpButton.className="terminals_input_button"
                tmpButton.id="inButton"+maxID;
                tmpButton.onclick=sendMessage;
                tmpButton.innerHTML="输入";
               
               
               tmpTd.appendChild( tmpButton);
               tmpTr.appendChild(tmpTd);
               
               tmpTd=document.createElement("td");
               tmpTd.className="terminals_input_help_bg";

                var tmpInput=document.createElement("textarea");
                tmpInput.rows="1";
                tmpInput.cols="95";
                tmpInput.id="input"+maxID;
                tmpInput.value="";
                tmpInput.className="terminals_input";
                tmpInput.onkeydown=inputKey;
                
                tmpTd.appendChild(tmpInput);
                tmpTr.appendChild(tmpTd);
                tmpTable.appendChild(tmpTr);
                
                
               tmpTr=document.createElement("tr");
               tmpTd=document.createElement("td");
               tmpTd.className="terminals_input_help_bg";
               
               var tmpDiv = document.createElement("div");
               tmpDiv.className="terminals_input_help_text";
               tmpDiv.innerHTML="预览：";
               
               tmpTd.appendChild(tmpDiv);
               tmpTr.appendChild(tmpTd);
               
               tmpTd=document.createElement("td");
               tmpTd.className="terminals_input_help_bg";
               
               


                tmpDiv=document.createElement("div");
                tmpDiv.className="terminals_output";
                tmpDiv.id="browse"+maxID;


                tmpTd.appendChild(tmpDiv);
               tmpTr.appendChild(tmpTd);
                tmpTable.appendChild(tmpTr);
                
                
                
                
                 tmpTr=document.createElement("tr");
               tmpTd=document.createElement("td");
               tmpTd.className="terminals_input_help_bg";
               
               tmpDiv = document.createElement("div");
               tmpDiv.className="terminals_input_help_text";
               tmpDiv.innerHTML="输出：";
               
               tmpTd.appendChild(tmpDiv);
               tmpTr.appendChild(tmpTd);
               
               tmpTd=document.createElement("td");
               tmpTd.className="terminals_input_help_bg";
		



                tmpDiv=document.createElement("div");
                tmpDiv.className="terminals_output";
                tmpDiv.id="output"+maxID;
              


                  tmpTd.appendChild(tmpDiv);
                tmpTr.appendChild(tmpTd);
                tmpTable.appendChild(tmpTr);

                
                document.getElementById('terminals').appendChild(tmpTable);

                /*
                   var tmpInput=document.createElement("input");
                   tmpInput.type="text";
                   tmpInput.id="input"+maxID;
                   tmpInput.value="";
                   tmpInput.className="terminals_input";
                   document.getElementById('terminals').appendChild(tmpInput);

                   var tmpDiv=document.createElement("div");
                   tmpDiv.className="terminals_output";
                   tmpDiv.id="output"+maxID;
                   document.getElementById('terminals').appendChild(tmpDiv);
                   */
                   
                   
                   /* rongyu  */
                     var   img=document.createElement("img");
    img.id="img_output_rongyu"+maxID;
     img.src="./images/ready.png";
    document.getElementById('output'+maxID).appendChild(img);
    
    img=document.createElement("img");
    img.id="img_browse_rongyu"+maxID;
     img.src="./images/ready.png";
     document.getElementById('browse'+maxID).appendChild(img);
              //   img=document.getElementById('img_output_rongyu'+maxID);
              //  document.getElementById('output'+maxID).removeChild(img);
              //  img=document.getElementById('img_browse_rongyu'+maxID);
             //  document.getElementById('browse'+maxID).removeChild(img);
              
            }
            else
            {
            //    document.getElementById('img_input'+ID).src=xhr.responseText.split("\n")[0];
		//document.getElementById('img_output'+ID).src=xhr.responseText.split("\n")[1];

                var img=document.getElementById('img_output'+ID)
                document.getElementById('output'+ID).removeChild(img);


                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
                img.id="img_output"+ID;
                document.getElementById('output'+ID).appendChild(img);


img=document.getElementById('img_input'+ID)
                document.getElementById('browse'+ID).removeChild(img);


                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[0];
                img.id="img_input"+ID;
                document.getElementById('browse'+ID).appendChild(img);



            }
        }}
}
function getstyle(sname) {
    for (var i=0;i<document.styleSheets.length;i++) {
        var rules;
        if (document.styleSheets[i].cssRules) {
            rules = document.styleSheets[i].cssRules;
        } else {
            rules = document.styleSheets[i].rules;
        }
        for (var j=0;j<rules.length;j++) {
            if (rules[j].selectorText == sname) {
                return rules[j].style;
            }
        }
    }
}

function sendMessage_expand(){
         

    var cmd=document.getElementById('input_expand').value;
    cmd='Expand['+cmd+']';
  //  alert(cmd);
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=exec&&userID='+uniqID+'&&id='+maxPicID+'&&cmd='+encodeURIComponent(cmd),true);
    maxPicID=maxPicID+1;
    xhr.onreadystatechange=getMessage_expand;
    xhr.send(null);
    var img=document.getElementById('img_expand');
                document.getElementById('output_expand').removeChild(img);
                img=document.createElement("img");
                img.src="./images/loading.gif";
        //        alert(img.src);
               
                img.id="img_expand";
                document.getElementById('output_expand').appendChild(img);
}
function getMessage_expand(){
    if(xhr.readyState == 4) {
        if(xhr.status == 200) {
               var img=document.getElementById('img_expand');
                document.getElementById('output_expand').removeChild(img);
                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
        //        alert(img.src);
               
                img.id="img_expand";
                document.getElementById('output_expand').appendChild(img);
    
        }
        
  }
 }
function reset_expand()
{

    document.getElementById('input_expand').value="(x-1)^50";
}
     
     
     



function sendMessage_factor(){
         

    var cmd=document.getElementById('input_factor').value;
    cmd='Factor['+cmd+']';
  // alert(cmd);
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=exec&&userID='+uniqID+'&&id='+maxPicID+'&&cmd='+encodeURIComponent(cmd),true);
    maxPicID=maxPicID+1;
    xhr.onreadystatechange=getMessage_factor;
    xhr.send(null);
    var img=document.getElementById('img_factor');
                document.getElementById('output_factor').removeChild(img);
                img=document.createElement("img");
                img.src="./images/loading.gif";
        //        alert(img.src);
               
                img.id="img_factor";
                document.getElementById('output_factor').appendChild(img);

}
function getMessage_factor(){
    if(xhr.readyState == 4) {
        if(xhr.status == 200) {
               var img=document.getElementById('img_factor');
                document.getElementById('output_factor').removeChild(img);
                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
        //        alert(img.src);
               
                img.id="img_factor";
                document.getElementById('output_factor').appendChild(img);
    
        }
        
  }
 }
function reset_factor()
{

    document.getElementById('input_factor').value="x^105-1";
}
        
function sendMessage_factorinteger(){
         

    var cmd=document.getElementById('input_factorinteger').value;
    cmd='FactorInteger['+cmd+']';
  // alert(cmd);
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=exec&&userID='+uniqID+'&&id='+maxPicID+'&&cmd='+encodeURIComponent(cmd),true);
    maxPicID=maxPicID+1;
    xhr.onreadystatechange=getMessage_factorinteger;
    xhr.send(null);
    
     var img=document.getElementById('img_factorinteger');
                document.getElementById('output_factorinteger').removeChild(img);
                img=document.createElement("img");
                img.src="./images/loading.gif";
        //        alert(img.src);
               
                img.id="img_factorinteger";
                document.getElementById('output_factorinteger').appendChild(img);
                
}
function getMessage_factorinteger(){
    if(xhr.readyState == 4) {
        if(xhr.status == 200) {
               var img=document.getElementById('img_factorinteger');
                document.getElementById('output_factorinteger').removeChild(img);
                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
        //        alert(img.src);
               
                img.id="img_factorinteger";
                document.getElementById('output_factorinteger').appendChild(img);
    
        }
        
  }
 }
function reset_factorinteger()
{

    document.getElementById('input_factorinteger').value="40!";
}





function sendMessage_solve(){
         

    var cmd=document.getElementById('input_solve').value;
    cmd='Solve['+cmd+']';
  // alert(cmd);
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=exec&&userID='+uniqID+'&&id='+maxPicID+'&&cmd='+encodeURIComponent(cmd),true);
    maxPicID=maxPicID+1;
    xhr.onreadystatechange=getMessage_solve;
    xhr.send(null);
    
    var img=document.getElementById('img_solve');
                document.getElementById('output_solve').removeChild(img);
                img=document.createElement("img");
                img.src="./images/loading.gif";
       //        alert(img.src);
               
                img.id="img_solve";
                document.getElementById('output_solve').appendChild(img);
                
                
}
function getMessage_solve(){
    if(xhr.readyState == 4) {
        if(xhr.status == 200) {
               var img=document.getElementById('img_solve');
                document.getElementById('output_solve').removeChild(img);
                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
       //        alert(img.src);
               
                img.id="img_solve";
                document.getElementById('output_solve').appendChild(img);
    
        }
        
  }
 }
function reset_solve()
{

    document.getElementById('input_solve').value="x^5+x^4+x^3+x^2+x+1";
}



function sendMessage_D(){
         

    var cmd1=document.getElementById('input_D1').value;
    var cmd2=document.getElementById('input_D2').value;
    cmd='D['+cmd1+','+cmd2+']';
 //  alert(cmd);
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=exec&&userID='+uniqID+'&&id='+maxPicID+'&&cmd='+encodeURIComponent(cmd),true);
    maxPicID=maxPicID+1;
    xhr.onreadystatechange=getMessage_D;
    xhr.send(null);
     var img=document.getElementById('img_D');
                document.getElementById('output_D').removeChild(img);
                img=document.createElement("img");
                img.src="./images/loading.gif";
    //           alert(img.src);
               
                img.id="img_D";
                document.getElementById('output_D').appendChild(img);
                
}
function getMessage_D(){
    if(xhr.readyState == 4) {
        if(xhr.status == 200) {
               var img=document.getElementById('img_D');
                document.getElementById('output_D').removeChild(img);
                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
    //           alert(img.src);
               
                img.id="img_D";
                document.getElementById('output_D').appendChild(img);
    
        }
        
  }
 }
function reset_D()
{

    document.getElementById('input_D1').value="f[g[h[x]]]";
    document.getElementById('input_D2').value="x";
}

function sendMessage_integrate(){
         

    var cmd1=document.getElementById('input_integrate1').value;
    var cmd2=document.getElementById('input_integrate2').value;
    cmd='Integrate['+cmd1+','+cmd2+']';
 //  alert(cmd);
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=exec&&userID='+uniqID+'&&id='+maxPicID+'&&cmd='+encodeURIComponent(cmd),true);
    maxPicID=maxPicID+1;
    xhr.onreadystatechange=getMessage_integrate;
    xhr.send(null);
    var img=document.getElementById('img_integrate');
                document.getElementById('output_integrate').removeChild(img);
                img=document.createElement("img");
                img.src="./images/loading.gif";
    //           alert(img.src);
               
                img.id="img_integrate";
                document.getElementById('output_integrate').appendChild(img);

}
function getMessage_integrate(){
    if(xhr.readyState == 4) {
        if(xhr.status == 200) {
               var img=document.getElementById('img_integrate');
                document.getElementById('output_integrate').removeChild(img);
                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
    //           alert(img.src);
               
                img.id="img_integrate";
                document.getElementById('output_integrate').appendChild(img);
    
        }
        
  }
 }
function reset_integrate()
{

    document.getElementById('input_integrate1').value="x^5 (a x+b)^(1/2)";
    document.getElementById('input_integrate2').value="x";
}

function sendMessage_sum(){
         

    var cmd1=document.getElementById('input_sum1').value;
    var cmd2=document.getElementById('input_sum2').value;
    var cmd3=document.getElementById('input_sum3').value;
    var cmd4=document.getElementById('input_sum4').value;
    cmd='Sum['+cmd1+',{'+cmd2+','+cmd3+','+cmd4+'}]';
 //  alert(cmd);
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=exec&&userID='+uniqID+'&&id='+maxPicID+'&&cmd='+encodeURIComponent(cmd),true);
    maxPicID=maxPicID+1;
    xhr.onreadystatechange=getMessage_sum;
    xhr.send(null);
    
    var img=document.getElementById('img_sum');
                document.getElementById('output_sum').removeChild(img);
                img=document.createElement("img");
                img.src="./images/loading.gif";
    //           alert(img.src);
               
                img.id="img_sum";
                document.getElementById('output_sum').appendChild(img);

}
function getMessage_sum(){
    if(xhr.readyState == 4) {
        if(xhr.status == 200) {
               var img=document.getElementById('img_sum');
                document.getElementById('output_sum').removeChild(img);
                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
    //           alert(img.src);
               
                img.id="img_sum";
                document.getElementById('output_sum').appendChild(img);
    
        }
        
  }
 }
function reset_sum()
{

    document.getElementById('input_sum1').value="1/n^2";
    document.getElementById('input_sum2').value="n";
    document.getElementById('input_sum3').value="1";
    document.getElementById('input_sum4').value="Infinity";
}




function sendMessage_N(){
         

    var cmd1=document.getElementById('input_N1').value;
    var cmd2=document.getElementById('input_N2').value;
    cmd='N['+cmd1+','+cmd2+']';
 //  alert(cmd);
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=exec&&userID='+uniqID+'&&id='+maxPicID+'&&cmd='+encodeURIComponent(cmd),true);
    maxPicID=maxPicID+1;
    xhr.onreadystatechange=getMessage_N;
    xhr.send(null);
    
    var img=document.getElementById('img_N');
                document.getElementById('output_N').removeChild(img);
                img=document.createElement("img");
                img.src="./images/loading.gif";
    //           alert(img.src);
               
                img.id="img_N";
                document.getElementById('output_N').appendChild(img);
    
    
    
}
function getMessage_N(){
    if(xhr.readyState == 4) {
        if(xhr.status == 200) {
               var img=document.getElementById('img_N');
                document.getElementById('output_N').removeChild(img);
                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
    //           alert(img.src);
               
                img.id="img_N";
                document.getElementById('output_N').appendChild(img);
    
        }
        
  }
 }
function reset_N()
{

    document.getElementById('input_N1').value="Pi-3";
    document.getElementById('input_N2').value="50";
}


function sendMessage_plot(){
         

    var cmd1=document.getElementById('input_plot1').value;
    var cmd2=document.getElementById('input_plot2').value;
    var cmd3=document.getElementById('input_plot3').value;
    var cmd4=document.getElementById('input_plot4').value;
    cmd='Plot['+cmd1+',{'+cmd2+','+cmd3+','+cmd4+'}]';
 //  alert(cmd);
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=exec&&userID='+uniqID+'&&id='+maxPicID+'&&cmd='+encodeURIComponent(cmd),true);
    maxPicID=maxPicID+1;
    xhr.onreadystatechange=getMessage_plot;
    xhr.send(null);
    
    var img=document.getElementById('img_plot');
                document.getElementById('output_plot').removeChild(img);
                img=document.createElement("img");
                img.src="./images/loading.gif";
    //           alert(img.src);
               
                img.id="img_plot";
                document.getElementById('output_plot').appendChild(img);
    
}
function getMessage_plot(){
    if(xhr.readyState == 4) {
        if(xhr.status == 200) {
               var img=document.getElementById('img_plot');
                document.getElementById('output_plot').removeChild(img);
                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
    //           alert(img.src);
               
                img.id="img_plot";
                document.getElementById('output_plot').appendChild(img);
    
        }
        
  }
 }
function reset_plot()
{

    document.getElementById('input_plot1').value="Sin[10x]";
    document.getElementById('input_plot2').value="x";
    document.getElementById('input_plot3').value="-1";
    document.getElementById('input_plot4').value="1";
}


function sendMessage_series(){
         

    var cmd1=document.getElementById('input_series1').value;
    var cmd2=document.getElementById('input_series2').value;
    var cmd3=document.getElementById('input_series3').value;
    var cmd4=document.getElementById('input_series4').value;
    cmd='Series['+cmd1+',{'+cmd2+','+cmd3+','+cmd4+'}]';
 //  alert(cmd);
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=exec&&userID='+uniqID+'&&id='+maxPicID+'&&cmd='+encodeURIComponent(cmd),true);
    maxPicID=maxPicID+1;
    xhr.onreadystatechange=getMessage_series;
    xhr.send(null);
    
    var img=document.getElementById('img_series');
                document.getElementById('output_series').removeChild(img);
                img=document.createElement("img");
                img.src="./images/loading.gif";
    //           alert(img.src);
               
                img.id="img_series";
                document.getElementById('output_series').appendChild(img);
    
}
function getMessage_series(){
    if(xhr.readyState == 4) {
        if(xhr.status == 200) {
               var img=document.getElementById('img_series');
                document.getElementById('output_series').removeChild(img);
                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
    //           alert(img.src);
               
                img.id="img_series";
                document.getElementById('output_series').appendChild(img);
    
        }
        
  }
 }
function reset_series()
{

    document.getElementById('input_series1').value="1/(1-x)";
    document.getElementById('input_series2').value="x";
    document.getElementById('input_series3').value="0";
    document.getElementById('input_series4').value="50";
}


function sendMessage_prod(){
         

    var cmd1=document.getElementById('input_prod1').value;
    var cmd2=document.getElementById('input_prod2').value;
    var cmd3=document.getElementById('input_prod3').value;
    var cmd4=document.getElementById('input_prod4').value;
    cmd='Prod['+cmd1+',{'+cmd2+','+cmd3+','+cmd4+'}]';
 //  alert(cmd);
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=exec&&userID='+uniqID+'&&id='+maxPicID+'&&cmd='+encodeURIComponent(cmd),true);
    maxPicID=maxPicID+1;
    xhr.onreadystatechange=getMessage_prod;
    xhr.send(null);
    
    var img=document.getElementById('img_prod');
                document.getElementById('output_prod').removeChild(img);
                img=document.createElement("img");
                img.src="./images/loading.gif";
    //           alert(img.src);
               
                img.id="img_prod";
                document.getElementById('output_prod').appendChild(img);
    
}
function getMessage_prod(){
    if(xhr.readyState == 4) {
        if(xhr.status == 200) {
               var img=document.getElementById('img_prod');
                document.getElementById('output_prod').removeChild(img);
                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
    //           alert(img.src);
               
                img.id="img_prod";
                document.getElementById('output_prod').appendChild(img);
    
        }
        
  }
 }
function reset_prod()
{

    document.getElementById('input_prod1').value="1-(4 x^2)/(Pi^2 (2k-1)^2)";
    document.getElementById('input_prod2').value="k";
    document.getElementById('input_prod3').value="1";
    document.getElementById('input_prod4').value="Infinity";
}

function sendMessage_nextprime(){
         

    var cmd=document.getElementById('input_nextprime').value;
    cmd='NextPrime['+cmd+']';
  // alert(cmd);
    xhr=getXMLHttpRequest();
    xhr.open('GET','core.php?type=exec&&userID='+uniqID+'&&id='+maxPicID+'&&cmd='+encodeURIComponent(cmd),true);
    maxPicID=maxPicID+1;
    xhr.onreadystatechange=getMessage_nextprime;
    xhr.send(null);
    
     var img=document.getElementById('img_nextprime');
                document.getElementById('output_nextprime').removeChild(img);
                img=document.createElement("img");
                img.src="./images/loading.gif";
        //        alert(img.src);
               
                img.id="img_nextprime";
                document.getElementById('output_nextprime').appendChild(img);
                
}
function getMessage_nextprime(){
    if(xhr.readyState == 4) {
        if(xhr.status == 200) {
               var img=document.getElementById('img_nextprime');
                document.getElementById('output_nextprime').removeChild(img);
                img=document.createElement("img");
                img.src=xhr.responseText.split("\n")[1];
        //        alert(img.src);
               
                img.id="img_nextprime";
                document.getElementById('output_nextprime').appendChild(img);
    
        }
        
  }
 }
function reset_nextprime()
{

    document.getElementById('input_nextprime').value="100!";
}
