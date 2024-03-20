const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>MedConnect IOT</title>
    <style>
      html {
        font-family: Arial;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
      }
      h1 { font-size: 1.5rem; }
      .textareaStyle {
        width: 295px;
        height: 70px;
        border:solid  1px #DCDCDC;
        box-shadow: 0px 0px 3px 0px #DCDCDC;
        margin: 5px auto;
      }
      .divStyle {
        border: 1px solid #DCDCDC;
        border-radius: 10px;
        width: 300px;
        height: 50px;
        margin: 0px auto;
        box-shadow: 0px 2px 3px 0px #DCDCDC;
        position: relative;
      }
      .child {
        width: 300px;
        position: absolute;
        top: 50%;
        left: 50%;
        transform: translate(-50%, -50%);
      }
    </style>
  </head>
  
  <body> 
   
    <div>
        <img width="30%" src="data:image/png;base64,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dSigEAgRgSbxJsjdd99t7rzzTls/bdq0MX/5y19M165dS66vv/71r+all16y8Tp16mSuueYa06JFZlXOkrVVLwLJq2/fvmblSrw0C/nOd75jbrvttrwJkDT/97//2XBvvvmmWbhwofn66/T7ZuvgPbL27dtbEj3++OPNcccdZ4ibpHEisGTJEvPhhx9Gmdtkk03ytu9HHnnE3HjjjVH44cOHmyOPPDI6rtbO1KlTzZlnnmm++iqzIkUZin/3u9+ZbbfdtoyYaygKV4Ru7PLb3/42ay2Rb3zjGylYSyVl+8EHH0w1a9Ys0tOlS5cUSKQkHQ0VGASY2mijjaK8fe9738ub1FNPPZWChRiFR1PJu7/99tunnnvuubw6dXHtIcC6ad68efS75JJL8mYGD/Ks+r700kvzhi/34tixY7PSKdTOkq4//vjj5Sa/RuI1iTHA9dZbL+tx8MADD5irr74661y+Az7Jfv7znxsgGgVr1apVtN+Udv7+97+bww8/3NDqK1beeustM2zYMPOf//yn2CgKtwYRYLukleV+zpLPlQWQZdal+HHWxQoOYDBUEDsdtRo6Ks5EHgXZSOYJ2Ngu0bTeeeedzW677ZY3a+xW/uQnPzHTpk3LG64pXPznP/9pTj/99Cwi5zjfscceaw488EDjxvxYVjx5zT333GO7xiwbu8w//vGPDbv+hxxySFMobrB5bCyksf7665uddtqpXheYhD1hwgSzYgU/AoOvIICQ+/fvb7fxSmvXrl38VKM6brIEyHGTk08+2bz44otm442xaFwO4XjhE088keNq8adXrcKaTpBKxgz5ZOdTnhZtqY381VdfNb/61a+yyO+73/2u+eMf/2g46eELurzmqKOOMmeccYb52c9+Zp599ll7mQ2Wx6NHj7ZE6MfJtV+Ncsd1V1sn9XHcsxJLyNVNpXpYVuaH9RvvucRxcMfxNlVJOZzOXNvVq1cb/tZdd137yxWO53v37m3bSjwM43Nc77333rOXMHxjnn/+edOhQ9P7VF+TJUAi//7775uzzjrLDggnEQrGVgzGU+L1V/QxiYITDWPGjDGTJ0+2NxmfdLvssov55je/abbYYouCuvi0xJiduf/++62e+fPnW8Lea6+9zIknnmi22WabgmTIG4rl/OILfH4tI6eccoqdxEkqtwuz9dZb2/wfdNBBUUNmOW644QZz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V8GFgBAQAaoNCAEhECwCIsBgq14FFwJCQASoNiAEhECwCIgAg616FVwICAERoNqAEBACwSIgAgy26lVwISAERIBqA0JACASLgAgw2KpXwYWAEBABqg0IASEQLAIiwGCrXgUXAkJABKg2IASEQLAIiACDrXoVXAgIARGg2oAQEALBIiACDLbqVXAhIAREgGoDQkAIBIuACDDYqlfBhYAQEAGqDQgBIRAsAiLAYKteBRcCQkAEqDYgBIRAsAiIAIOtehVcCAgBEaDagBAQAsEiIAIMtupVcCEgBESAagNCQAgEi4AIMNiqV8GFgBAQAaoNCAEhECwCIsBgq14FFwJCQASoNiAEhECwCIgAg616FVwICAERoNqAEBACwSIgAgy26lVwISAERIBqA0JACASLgAgw2KpXwYWAEBABqg0IASEQLAIiwGCrXgUXAkJABKg2IASEQLAIiACDrXoVXAgIARGg2oAQEALBIiACDLbqVXAhIAREgGoDQkAIBIuACDDYqlfBhYAQEAGqDQgBIRAsAiLAYKteBRcCQkAEqDYgBIRAsAiIAIOtehVcCAgBEaDagBAQAsEiIAIMtupVcCEgBESAagNCQAgEi4AIMNiqV8GFgBAQAaoNCAEhECwCIsBgq14FFwJCQASoNiAEhECwCIgAg616FVwICAERoNqAEBACwSIgAgy26lVwISAERIBqA0JACASLgAgw2KpXwYWAEBABqg0IASEQLAIiwGCrXgUXAkJABKg2IASEQLAIiACDrXoVXAgIARGg2oAQEALBIiACDLbqVXAhIAREgGoDQkAIBIuACDDYqlfBhYAQEAGqDQgBIRAsAiLAYKteBRcCQkAEqDYgBIRAsAj8P82sr3HpbVFHAAAAAElFTkSuQmCC">
    </div>

  
    <canvas id="myCanvas" width="300" height="150" style="box-shadow: 0px 2px 3px 0px #DCDCDC;"></canvas>
    <br><br>
    <button type="button" id="BTN_Start_Stop" onclick="my_BTN_Start_Stop()">Avvia Monitoraggio</button>
    <p id="show_Info" hidden></p>
    <br><br>
    <div class="divStyle">
      <div class="child">
        <img width="27" height="27" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAYAAAA7MK6iAAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAC4hAAAuIQEHW/z/AAAACXZwQWcAAAAeAAAAHgD4T+E9AAAEI0lEQVRIx52WXWxTZRjHf+9Zt9l1dGMojGlxITLARGRBogkGhpEYjfLhhTo2EtfyockCcoEao4yheGG8MHGJiWOaoKBwY0bQxZg4xobMDJSghsUtsm46JmGjp+1pQdb38YLuC9bT0/2v2vP8n//vfZ/2nPcoEWFM9YbhWuD2voiiClgBFAFh4IKGbyKxcONrWseZRo2GkWfkebcasAlYBniBEeAcwuH+ePhondajY341Bj7oLiwzsjiqYDmp1TOakPXb4mb3FKi7YIkrSx0HFqVqFDg/muCF7fFQzzj4oLuwLCuLDuAe0uvf63Jz5auWNQDwicfju0tldwHzHPReuZng8e3xUI/ap5TLl+ftSrPT29XRHwuvAViQN+sUqFVOGwXOD8TCK1VTXsEWpTiUAfSWtGzVoAxDNWbeyhZX8o+UsUSpejWTRsBQVLkUsgIyj1CKe2fIBeQRA9TsmQfMVKrQACJOrGsPfc769lbue2rdHbUnj33FS73d3P/cs07JUQO4lM41q7SU0uc3Mae8nNING6bUPCUl+J55GndxMSv27UUZhhPwJQM4m861qHrz+OeStWum1OZP+l6wuIwHqiqdgLsMkBY7h1JqPEwSCTw+H96FCycWUlEBwJUznQCUv/UmWTk5tlRBWgwrFvkOGE5lmr9mNR6fj2gwSLD5eHLXFeOLGpvAT7t2c+33P/D4fCzZ6rfjDsdikRajVusbQMqHwKIt1QD0Hvmaf35snTLegsVluIuLiQ8NEbrYzbn6/QAse30P2fn5KXbLp7Va33ABbO77y5V7d5HteHq/PDIxhdWrb+02OebB1jZEhIGW77nS+TNzH3uU6qG/p82JX73qAjAAcubMtuygQ6faiQSDRIJBon195BbNZs7yhyl5Igk+eXLce3bvPtsN5BYVWTDpWGwwjFxPnvdXYKld46qPP6Ks5mV+qX+Xh3bvItvr5VjZg1iDg6TRRSsWLk/+tIzfdLVa3xAhAIzadQ+2tgGw9JUdZHu9mH/2OIGOavCPQaeAAfxW6Awi++0SLre1gQjueXOTC2lNB0VE6gPRUOfka3c8ZvrjkQMgJ1KFXB8eYeTCbxMLSU7ABntiIB55//ard4DrtNajsUi1IN2pogZP3oKJ1lxu70iNRLqtWKSqTmudFgywTWtTJdRG4Np09f4T3zLU3kHvF4f5zzRTcUdUQm2s1To8XVFNfsu8XU2eglWGUj8AbjJTXIusC1jm6VQG26MkYJmnNVIJJDKAJjRSaQdNCwYIRM1mtOwAJJ0XELTsCETN5nRGR4dnTcxsEvROB9SdNTGzyUmmIzCAPxpu0CJvpIQKe/zRcIPTPMdggIBlfiAi70xDfdtvhT7MJCsjMIDfMt8TkboJptTVWOaBTHNsbyc7fZZfWAfgj4bqZ9L/P5FZrj+hbSxaAAAAJXRFWHRjcmVhdGUtZGF0ZQAyMDIzLTA3LTIxVDEzOjM4OjQ3KzA3OjAwEH/56gAAACV0RVh0bW9kaWZ5LWRhdGUAMjAyMy0wNy0yMVQxMzozODo0NyswNzowME/Oj94AAAAASUVORK5CYII="/>
        <label id="" style="font-size:18px;">BPM</label>
        <label id="bpm_Show" style="font-size:36px;"></label>
      </div>
    </div>
    
    
    <br>
    <textarea id="log_BPM" name="log_BPM" class="textareaStyle" readonly></textarea>
    <br>
    <button type="button" onclick="save_To_CSV()">Salva CSV</button>
    <button type="button" onclick="clear_BPM_Log()">Cancella Log</button>
    
    <script>
      var axis_x = 0;
      var axis_y = 0;
      var last_BPM_TimeStamp = "";
      var count_Down = 3;
      var myInterval_Count_Down;
      
      document.getElementById("bpm_Show").innerHTML = "&nbsp;0 BPM";
      
      var P_show_Info = document.getElementById("show_Info");
      
      setInterval(myTimer, 35);

      //------------------------------------------------------------ Processes the data received from the ESP32.
      if (!!window.EventSource) {
        var source = new EventSource('/events');
        
        source.addEventListener('open', function(e) {
          console.log("Dispositivo Connesso");
        }, false);
        
        source.addEventListener('error', function(e) {
          if (e.target.readyState != EventSource.OPEN) {
            console.log("Dispositivo Disconnesso");
            console.log("Errore ", e);
          }
        }, false);
        
        source.addEventListener('message', function(e) {
          console.log("message", e.data);
        }, false);

        source.addEventListener('allDataJSON', function(e) {
          console.log("allDataJSON", e.data);
          
          var obj = JSON.parse(e.data);
          var get_Heartbeat_Signal = obj.heartbeat_Signal;
          var get_BPM_TimeStamp = obj.BPM_TimeStamp;
          var get_BPM_Val = obj.BPM_Val;
          var get_BPM_State = obj.BPM_State;
          //console.log("segnale",get_Heartbeat_Signal);

          if (get_Heartbeat_Signal > 565) get_Heartbeat_Signal = 565;
          if (get_Heartbeat_Signal < 545) get_Heartbeat_Signal = 545;
          
          axis_y = map(get_Heartbeat_Signal, 545, 565, 150, 0); // 150 is the height of the canvas. Look at the canvas height in the "<body>" section.
          
          if (get_BPM_State == true) {
            document.getElementById("bpm_Show").innerHTML = "&nbsp;" + get_BPM_Val + " BPM";
            
            if (last_BPM_TimeStamp != get_BPM_TimeStamp) {
              var my_textarea = document.getElementById('log_BPM');
              var lastLog = my_textarea.textContent;
              if (lastLog == "") {
                document.getElementById('log_BPM').textContent =  "Tempo,BPM\r\n" + get_BPM_TimeStamp + "," + get_BPM_Val;
              }
              else {
                document.getElementById('log_BPM').textContent =  lastLog + "\r\n" + get_BPM_TimeStamp + "," + get_BPM_Val;
              }
              my_textarea.scrollTop = my_textarea.scrollHeight;
            }
            
            last_BPM_TimeStamp = get_BPM_TimeStamp;
          } else {
            document.getElementById("bpm_Show").innerHTML = "&nbsp;0 BPM";
          }
        }, false);
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------Line graph.
      var canvas = document.getElementById('myCanvas');
      var context = canvas.getContext('2d');
      
      drawGraph();
       
      context.beginPath();
      context.lineWidth = 1;
      context.strokeStyle = '#0583F2';
      
      function drawGraph() {
        context.beginPath();
        context.moveTo(1,150);
        context.lineTo(1,0);
        context.moveTo(50,150);
        context.lineTo(50,0);
        context.moveTo(100,150);
        context.lineTo(100,0);
        context.moveTo(150,150);
        context.lineTo(150,0);
        context.moveTo(200,150);
        context.lineTo(200,0);
        context.moveTo(250,150);
        context.lineTo(250,0);
        context.moveTo(299,150);
        context.lineTo(299,0);

        context.moveTo(0,1);
        context.lineTo(300,1);
        context.moveTo(0,25);
        context.lineTo(300,25);
        context.moveTo(0,50);
        context.lineTo(300,50);
        context.moveTo(0,75);
        context.lineTo(300,75);
        context.moveTo(0,100);
        context.lineTo(300,100);
        context.moveTo(0,125);
        context.lineTo(300,125);
        context.moveTo(0,149);
        context.lineTo(300,149);

        context.lineWidth = 0.5;
        context.strokeStyle = '#DCDCDC';
        context.closePath();
        context.stroke();
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------Mostra il segnale del sensore su grafico
      function set_Axis_XY(x,y) {
        context.lineTo(x, y);
        context.stroke();
        if (axis_x > 300) {
           reset_Graph();
        }
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------
      function reset_Graph() {
        axis_x = 0; 
        context.clearRect(0,0,300,150);
        context.closePath();
        drawGraph();
        context.beginPath();
        context.lineWidth = 1;
        context.strokeStyle = '#0583F2';
        context.moveTo(0,axis_y);
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------
      function myTimer() {
        axis_x += 3;
        set_Axis_XY(axis_x, axis_y);
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------Scale from the signal value to the line graph.
      // Source: https://www.arduino.cc/reference/en/language/functions/math/map/ , https://forum.jquery.com/topic/jquery-map-like-arduino-map
      function map( x,  in_min,  in_max,  out_min,  out_max){
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------
      function my_BTN_Start_Stop() {
        reset_Graph();
        
        var my_BTN_Start_Stop_Text = document.getElementById("BTN_Start_Stop").innerHTML;
        if (my_BTN_Start_Stop_Text == "Avvia Monitoraggio") {
          document.getElementById("BTN_Start_Stop").innerHTML = "Ferma Monitoraggio";
          send_BTN_Cmd("START");
          
          count_Down = 3;
          P_show_Info.style.display = "block";
          document.getElementById("show_Info").innerHTML = "Attendi...";
          myInterval_Count_Down = setInterval(myTimer_Count_Down, 1000);
        } else if (my_BTN_Start_Stop_Text == "Ferma Monitoraggio") {
          document.getElementById("BTN_Start_Stop").innerHTML = "Avvia Monitoraggio";
          send_BTN_Cmd("STOP");
        }
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------
      function myTimer_Count_Down() {
        count_Down -= 1;
        document.getElementById("show_Info").innerHTML = "Attendi " + (count_Down + 1);
        if (count_Down < 0) {
          P_show_Info.style.display = "none";
          myStop_Count_Down();
        }
      }
      
      function myStop_Count_Down() {
        clearInterval(myInterval_Count_Down);
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------
      function save_To_CSV() {
        var a = document.createElement('a');
        with (a) {
          href='data:text/csv;base64,' + btoa(document.getElementById('log_BPM').value);
          download='BPMLog.csv';
        }
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------
      function clear_BPM_Log() {
        let dialog_text = "Sicuro di voler eliminare il log?";
        if (confirm(dialog_text) == true) {
          document.getElementById('log_BPM').textContent = "";
        } 
      }
      //------------------------------------------------------------
      
      //------------------------------------------------------------ XMLHttpRequest to submit data.
      function send_BTN_Cmd(state) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "BTN_Comd?BTN_Start_Get_BPM=" + state, true);
        xhr.send();
      }
      //------------------------------------------------------------
    </script>
  </body>
</html>
)=====";