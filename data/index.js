function showValue1()
{
    var rangeInput1 = document.getElementById("myRange1");
    var rangeValue1 = document.getElementById("rangeValue1");
    rangeValue1.textContent = rangeInput1.value;
}

function showValue2()
{
    var rangeInput2 = document.getElementById("myRange2");
    var rangeValue2 = document.getElementById("rangeValue2");
    rangeValue2.textContent = rangeInput2.value;
}

function showValue3() 
{
    var rangeInput3 = document.getElementById("myRange3");
    var rangeValue3 = document.getElementById("rangeValue3");
    rangeValue3.textContent = rangeInput3.value;
}

function showValue4() 
{
    var rangeInput4 = document.getElementById("myRange4");
    var rangeValue4 = document.getElementById("rangeValue4");
    rangeValue4.textContent = rangeInput4.value;
}

function offFonction()
{
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "off", true);
    xhttp.send();
}
$(document).ready(function(){
    $("#valider").click(function(){
        var valeur1 = $("#myRange1").val();
        var valeur2 = $("#myRange2").val();
        var valeur3 = $("#myRange3").val();
        var valeur4 = $("#myRange4").val();
        $.post("position",{
            position1 : valeur1,
            position2 : valeur2,
            position3 : valeur3,
            position4 : valeur4
        });
    });
});
