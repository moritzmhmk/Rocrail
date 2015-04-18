var req;

function openInfo()
{
  console.log("close menu");
  $( "#popupMenu" ).popup( "close" );
  
  console.log("open info");
  $('#popupMenu').on("popupafterclose", function(){$( "#popupInfo" ).popup( "open" )});

//  console.log("open info");
//  $( "#popupInfo" ).popup( "open" );
}


function openThrottle()
{
  $( "#popupThrottle" ).popup( "open" );
}

function closeThrottle()
{
  $( "#popupThrottle" ).popup( "close" );
}

function openMenu()
{
  $('#popupMenu').unbind("popupafterclose");
  $( "#popupMenu" ).popup( "open" );
  
  // Test to save a session structure.
  var E03 = {color: "grey", name: "Spot", size: 46};
  E03.test = "Hallo";
  sessionStorage.setItem("E03", JSON.stringify(E03));

  var cat = JSON.parse(sessionStorage.getItem("E03"));
  console.log(cat.size + " " + cat.color + " " + cat.test );

}

function openOptions()
{
}

function closeOptions()
{
}

function actionSensor(id)
{
  console.log("sensor action on " + id );
  //alert("flip sensor " + id);
 
  // send an XMLHttpRequest
  try {
    req = new XMLHttpRequest();
    req.onreadystatechange = processReqChange;
    req.open("GET", "rocweb.xml?fb_flip="+id.replace("fb_",""), true);
    req.send("");
  } 
    catch(e) {
      alert("exception: " + e);
    }
  }



function actionBlock(id)
{
  localStorage.setItem("block", id);
  document.getElementById("blockTitle").innerHTML = "Block: " + localStorage.getItem("block");
  $( "#popupBlock" ).popup( "open", {positionTo: '#'+id} );
}

function speedUpdate(value) {
  document.getElementById("direction").innerHTML = "" + value + " >";
  $("#direction").css("background","#557755");
}

$(document).on("pagecreate",function(){

  // jQuery events go here...
  $("#speedSlider").on( "slidestop", function( event, ui ) {
  value = this.value;
  speedUpdate(value);} );
  
  $('#locoSelect').change(function() {
    console.log("locoSelect: " + this.value );
  } );

});


function processReqChange() {
//only if req shows "loaded"
if (req.readyState == 4) {
 //alert("response status = "+req.status);
 if (req.status == 0) {
   try {
     xmlDoc = req.responseXML;
     fblist = xmlDoc.getElementsByTagName("fb");
     for (var i = 0; i < fblist.length; i++) {
       var div = document.getElementById("fb_"+fblist[i].getAttribute('id'));
       if( "true" == fblist[i].getAttribute('state') )
         div.style.backgroundImage = "url(sensor_on_1.png)";
       else
         div.style.backgroundImage = "url(sensor_off_1.png)";
     }
   }
   catch(e) {
     alert("exception: " + e);
   }

 }
 if (req.status == 200) {
   // ...processing statements go here...
   alert("response received");
   }
 }
}

function loadPlan() {
 // send an XMLHttpRequest
 try {
   req = new XMLHttpRequest();
   req.onreadystatechange = processReqPlan;
   req.open("GET", "plan.xml", true);
   req.send("");
 } 
   catch(e) {
     alert("exception: " + e);
   }
}

function xml2string(node) {
  if (typeof(XMLSerializer) !== 'undefined') {
     var serializer = new XMLSerializer();
     return serializer.serializeToString(node);
  } else if (node.xml) {
     return node.xml;
  }
}

function processReqPlan() {
//only if req shows "loaded"
if (req.readyState == 4) {
 // only if "OK"
 //alert("response status = "+req.status);
 if (req.status == 0) {
   try {
     xmlDoc = req.responseXML;
     
     fblist = xmlDoc.getElementsByTagName("fb");
     for (var i = 0; i < fblist.length; i++) {
       console.log('sensor: ' + fblist[i].getAttribute('id'));
       var newdiv = document.createElement('div');
       newdiv.setAttribute('id', "fb_"+fblist[i].getAttribute('id'));
       newdiv.setAttribute('onClick', "actionSensor(this.id)");
       newdiv.setAttribute('class', "item");
       newdiv.style.position = "absolute";
       newdiv.style.width    = "32px";
       newdiv.style.height   = "32px";
       newdiv.style.left     = "" + (parseInt(fblist[i].getAttribute('x')) * 32) + "px";
       newdiv.style.top      = "" + (parseInt(fblist[i].getAttribute('y')) * 32) + "px";
       newdiv.innerHTML      = "";
       
       if( "true" == fblist[i].getAttribute('state') )
         newdiv.style.backgroundImage = "url('img/sensor_on_1.png')";
       else
         newdiv.style.backgroundImage = "url('img/sensor_off_1.png')";

       document.body.appendChild(newdiv);
     }
     
     bklist = xmlDoc.getElementsByTagName("bk");
     for (var i = 0; i < bklist.length; i++) {
       console.log('block: ' + bklist[i].getAttribute('id'));
       var newdiv = document.createElement('div');
       newdiv.setAttribute('id', "bk_"+bklist[i].getAttribute('id'));
       newdiv.setAttribute('onClick', "actionBlock(this.id)");
       newdiv.setAttribute('class', "item");
       newdiv.style.position = "absolute";
       newdiv.style.width    = "128px";
       newdiv.style.height   = "32px";
       newdiv.style.lineHeight = "32px";
       newdiv.style.left     = "" + (parseInt(bklist[i].getAttribute('x')) * 32) + "px";
       newdiv.style.top      = "" + (parseInt(bklist[i].getAttribute('y')) * 32) + "px";
       newdiv.innerHTML      = "<label class='itemtext'>"+bklist[i].getAttribute('locid')+"</label>";
       newdiv.style.textAlign= "center";
       
       console.log(xml2string(bklist[i]));
       console.log("block entering=" + bklist[i].getAttribute("entering") + " x,y " + newdiv.style.left + "," + newdiv.style.top );

       if( "true" == bklist[i].getAttribute('entering') )
         newdiv.style.backgroundImage = "url('img/block_enter_1.png')";
       else if( "closed" == bklist[i].getAttribute('state') )
         newdiv.style.backgroundImage = "url('img/block_closed_1.png')";
       else
         newdiv.style.backgroundImage = "url('img/block_occ_1.png')";

       document.body.appendChild(newdiv);
     }
   }
   catch(e) {
     alert("exception: " + e);
   }

 }
 if (req.status == 200) {
   // ...processing statements go here...
   alert("response received");
   }
 }
}


