$(document).ready(function(){
    $("#choixAmp").change(function(){
        var valeur = $("#choixAmp").val();
        $.post("/choixAmp",{
            valeurchoixAmp: valeur
        });
    });
    $("#choixCab").change(function(){
        var valeur = $("#choixCab").val();
        $.post("/choixCab",{
            valeurchoixCab: valeur
        });
    });
    $("#choixMic").change(function(){
        var valeur = $("#choixMic").val();
        $.post("/choixMic",{
            valeurchoixMic: valeur
        });
    });
    $("#choixVerb").change(function(){
        var valeur = $("#choixVerb").val();
        $.post("/choixVerb",{
            valeurchoixVerb: valeur
        });
    });
    $("#Drive").change(function(){
        var valeur = $("#Drive").val();
       $.post("/Drive",{
           valeurDrive: valeur
       });
    });
    $("#Bass").change(function(){
        var valeur = $("#Bass").val();
       $.post("/Bass",{
           valeurBass: valeur
       });
    });
    $("#Mid").change(function(){
        var valeur = $("#Mid").val();
       $.post("/Mid",{
           valeurMid: valeur
       });
    });
    $("#Treble").change(function(){
        var valeur = $("#Treble").val();
       $.post("/Treble",{
           valeurTreble: valeur
       });
    });
    $("#Pres").change(function(){
        var valeur = $("#Pres").val();
       $.post("/Pres",{
           valeurPres: valeur
       });
    });
    $("#VolCh").change(function(){
        var valeur = $("#VolCh").val();
       $.post("/VolCh",{
           valeurVolCh: valeur
       });
    });
    $("#Decay").change(function(){
        var valeur = $("#Decay").val();
       $.post("/Decay",{
           valeurDecay: valeur
       });
    });
    $("#Predel").change(function(){
        var valeur = $("#Predel").val();
       $.post("/Predel",{
           valeurPredel: valeur
       });
    });
    $("#Tone").change(function(){
        var valeur = $("#Tone").val();
       $.post("/Tone",{
           valeurTone: valeur
       });
    });
    $("#MixVerb").change(function(){
        var valeur = $("#MixVerb").val();
       $.post("/MixVerb",{
           valeurMixVerb: valeur
       });
    });
    $("#VolAmp").change(function(){
        var valeur = $("#VolAmp").val();
       $.post("/VolAmp",{
           valeurVol: valeur
       });
    });
    $("#VoiceI").change(function(){
        var valeur = $("#VoiceI").val();
       $.post("/VoiceI",{
           valeurVoiceI: valeur
       });
    });
    $("#VoiceII").change(function(){
        var valeur = $("#VoiceII").val();
       $.post("/VoiceII",{
           valeurVoiceII: valeur
       });
    });
    $("#VoiceIII").change(function(){
        var valeur = $("#VoiceIII").val();
       $.post("/VoiceIII",{
           valeurVoiceIII: valeur
       });
    });
    $("#VoiceIV").change(function(){
        var valeur = $("#VoiceIV").val();
       $.post("/VoiceIV",{
           valeurVoiceIV: valeur
       });
    });
    $("#TopoI").change(function(){
        var valeur = $("#TopoI").val();
       $.post("/TopoI",{
           valeurTopoI: valeur
       });
    });
    $("#TopoII").change(function(){
        var valeur = $("#TopoII").val();
       $.post("/TopoII",{
           valeurTopoII: valeur
       });
    });
    $("#TopoIII").change(function(){
        var valeur = $("#TopoIII").val();
       $.post("/TopoIII",{
           valeurTopoIII: valeur
       });
    });
    $("#TopoIV").change(function(){
        var valeur = $("#TopoIV").val();
       $.post("/TopoIV",{
           valeurTopoIV: valeur
       });
    });
    $("#CanalA").change(function(){
        var valeur = $("#CanalA").val();
        $.post("/CanalA",{
           valeurCanalA: valeur
        });
       document.getElementById('choixAmp').disabled = false
       document.getElementById('viewvoicing').style.visibility = "hidden";
       document.getElementById('monnom').style.visibility = "visible";
       document.getElementById('monnom2').style.visibility = "hidden";
    });
    $("#CanalB").change(function(){
        var valeur = $("#CanalB").val();
       $.post("/CanalB",{
           valeurCanalB: valeur
       });
       document.getElementById('choixAmp').disabled = true;
       document.getElementById('viewvoicing').style.visibility = "visible";
       document.getElementById('monnom').style.visibility = "hidden";
       document.getElementById('monnom2').style.visibility = "visible";
    });
    $("#ClassAB").change(function(){
        var valeur = $("#ClassAB").val();
       $.post("/ClassAB",{
           valeurClassAB: valeur
       });
    });
    $("#ClassA").change(function(){
        var valeur = $("#ClassA").val();
       $.post("/ClassA",{
           valeurClassA: valeur
       });
    });
    $("#BoostON").change(function(){
        var valeur = $("#BoostON").val();
       $.post("/BoostON",{
           valeurBoostON: valeur
       });
    });
    $("#BoostOFF").change(function(){
        var valeur = $("#BoostOFF").val();
       $.post("/BoostOFF",{
           valeurBoostOFF: valeur
       });
    });
    $("#Pentode").change(function(){
        var valeur = $("#Pentode").val();
       $.post("/Pentode",{
           valeurPentode: valeur
       });
    });
    $("#Triode").change(function(){
        var valeur = $("#Triode").val();
       $.post("/Triode",{
           valeurTriode: valeur
       });
    });
    $("#VerbON").change(function(){
        var valeur = $("#VerbON").val();
       $.post("/VerbON",{
           valeurVerbON: valeur
       });
    });
    $("#VerbOFF").change(function(){
        var valeur = $("#VerbOFF").val();
       $.post("/VerbOFF",{
           valeurVerbOFF: valeur
       });
    });
    $("#LowON").change(function(){
        var valeur = $("#LowON").val();
       $.post("/LowON",{
           valeurLowON: valeur
       });
    });
    $("#LowOFF").change(function(){
        var valeur = $("#LowOFF").val();
       $.post("/LowOFF",{
           valeurLowOFF: valeur
       });
    });
});

var prog, maxbank, bankName, patchName, p1, p2, p3, p4, p5, p6
setInterval(function afficheBank() {
    $.get("/loadValues", function(data) {
        var canalbstate = 0
        //Vol=123#Drive=20#...
        data = data.split("#") // ["Vol=123", "Drive=20"]
        data.forEach(element => {
            element = element.split("=") //  ["Vol", "123"]
            var t = "#"
            t += element[0]
            var t1 = t
            t += "T"
            $(t).html(element[1])
            $(t1).val(element[1])
            if (element[1] == 1){$("#"+element[0]).prop("checked", true)}
            document.getElementById('choixAmp').disabled = false;
            document.getElementById('viewvoicing').style.visibility = "hidden"
            document.getElementById('monnom').style.visibility = "visible"
            document.getElementById('monnom2').style.visibility = "hidden"
            if (element[0] == "maxbank") {maxbank = element[1]}
            if (element[0] == "p1") {p1 = element[1]}
            if (element[0] == "p2") {p2 = element[1]}
            if (element[0] == "p3") {p3 = element[1]}
            if (element[0] == "p4") {p4 = element[1]}
            if (element[0] == "p5") {p5 = element[1]}
            if (element[0] == "p6") {p6 = element[1]}
            if (element[0] == "bankName") {bankName = element[1]}
            if (element[0] == "patchName") {patchName = element[1]}
            if (element[0] == "prog") {prog = element[1]}
            if (element[0] == "CanalB" && element[1] == 1) {canalbstate = 1}
            if (canalbstate == 1){
                document.getElementById('choixAmp').disabled = true;
                document.getElementById('viewvoicing').style.visibility = "visible";
                document.getElementById('monnom').style.visibility = "hidden";
                document.getElementById('monnom2').style.visibility = "visible";
            }
           
        });
        var val = "DT25 - "
        val += bankName
        console.log(val)
        $("#DT25").html(val)

        var val4 = prog
        val4 += " - "
        val4 += patchName
        $("#idT").html(val4)

    })
}, 50);

function bankMoins() {
    $.get("/bkMoins")
   // document.getElementById('id05').style.display='block'
    //setTimeout(function() {document.getElementById('id05').style.display='none'}, 750);
    
}

function bankPlus() {
    $.get("/bkPlus")
    //document.getElementById('id06').style.display='block'
    //setTimeout(function() {document.getElementById('id06').style.display='none'}, 750);
    
}

function choixBank() {
    var valeur = $("#choixbank1").val();
       $.post("/choixbank",{choixbank: valeur});
       
    setTimeout(function() {document.getElementById('id07').style.display='none'}, 750)
}

function saveBank() {
    document.getElementById('id04').style.display='block'
    $.post("/savebk")
    setTimeout(function() {document.getElementById('id04').style.display='none'}, 1500)
}


function initBank() {
    $.get("/init" )
    setTimeout(function() {document.getElementById('id02').style.display='none'}, 1500)
}

function cpynew() {
    document.getElementById('id01').style.display='block'
    var val = prog + 6
    $('#maxbank1').val(val)
}

function newBank() {
    var valeurnewBank = document.getElementById('maxbank1').value;    
    $.post("/newBank",{
        valeurnewBank: valeurnewBank
    })
    setTimeout(function() {document.getElementById('id01').style.display='none'}, 1500)
}

function savedb() {
    $.get("/base")
    $.post("/base", function(data1) {
        window.location = data1
        $('#adr1').html(data1)
    })
}

function fermeSet() {
    document.getElementById('id03').style.display='none'
    $('#adr1').html("Choix en attente...")
}

function viewbkName() {    
    document.getElementById('id08').style.display='block'
    $("#bankName1").val(bankName)
}

function saveName() {
    var valeur = document.getElementById("bankName1").value;
    $.post("/bankName",{
        valeurbankName: valeur
    })
    setTimeout(function() {document.getElementById('id08').style.display='none';}, 2500)
}

function viewpatchName() {
    $("#patchName1").val(patchName)
    document.getElementById('id07').style.display='block'
}

function savePatchName() {
    var valeur = document.getElementById("patchName1").value;
    $.post("/patchName",{
        valeurpatchName: valeur
    })
    setTimeout(function() {document.getElementById('id07').style.display='none';}, 2500)
}

function ViewPC(){
    src = "ViewPC.html"
}

function savePedalsNames() {
    //var valeur = document.getElementById("p11").value +  " " + document.getElementById("p21").value + " " +  document.getElementById("p31").value + " " + document.getElementById("p41").value + " " + document.getElementById("p51").value + " " + document.getElementById("p61").value
    $.post("/textValues",{
        valeurpedalname1: document.getElementById("p11").value,
        valeurpedalname2: document.getElementById("p21").value,
        valeurpedalname3: document.getElementById("p31").value,
        valeurpedalname4: document.getElementById("p41").value,
        valeurpedalname5: document.getElementById("p51").value,
        valeurpedalname6: document.getElementById("p61").value
    })
    setTimeout(function() {document.getElementById('id02').style.display='none';}, 2500)
}

function Stomps(){
    $("#p11").val(p1)
    $("#p21").val(p2)
    $("#p31").val(p3)
    $("#p41").val(p4)
    $("#p51").val(p5)
    $("#p61").val(p6)
    document.getElementById('id02').style.display='block'
}