// Put all onload AJAX calls here, and event listeners
$(document).ready(function() { 
    // On page-load AJAX Example 
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/someendpoint',   //The server endpoint we are connecting to
        data: {
            name1: "Value 1",
            name2: "Value 2"
        },
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            $('#blah').html("On page load, received string '"+data.foo+"' from server");
            //We write the object to the console to show that the request was successful
            console.log(data); 

        },
        fail: function(error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error); 
        }
    });

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function(e){ 
        $('#blah').html("Form has data: "+$('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });
});

//We register a callback function for the click event of
 //the element 'clickMe' - i.e. our button
var count = 0;
var Emoji_pooTitle = "";
var quad01_A2Title = "";
var Emoji_party_A2Title = "";
var Emoji_grinningTitle = "";
var Emoji_shadesTitle = "";
var vestTitle = "";
var hen_and_chicksTitle = "";
var Emoji_pooDescription = "";
var quad01_A2Description = "";
var Emoji_party_A2Description = "";
var Emoji_grinningDescription = "";
var Emoji_shadesDescription = "";
var vestDescription = "";
var hen_and_chicksDescription = "";

function randNum(){
    return (Math.floor(Math.random* 10) + 1);
}

function populateTable(){
    var inputs = ['<a href="Emoji_poo.svg"><img src="Emoji_poo.svg" width="200"></img></a>', '<a href="Emoji_poo.svg">Emoji_poo.svg</a>', "1KB", "0", "2", "6", "1",    '<a href="quad01_A2.svg"><img src="quad01_A2.svg" width="200"></img></a>', '<a href="quad01_A2.svg">quad01_A2.svg</a>', "1KB", "1", "5", "2", "3",    '<a href="Emoji_party_A2.svg"><img src="Emoji_party_A2.svg" width="200"></img></a>', '<a href="Emoji_party_A2.svg">Emoji_party_A2.svg</a>', "3KB", "0", "0", "21", "2",    '<a href="Emoji_grinning.svg"><img src="Emoji_grinning.svg" width="200"></img></a>', '<a href="Emoji_grinning.svg">Emoji_grinning.svg</a>', "1KB", "0", "3", "4", "1",   '<a href="Emoji_shades.svg"><img src="Emoji_shades.svg" width="200"></img></a>', '<a href="Emoji_shades.svg">Emoji_shades.svg</a>', "1KB", "0", "0", "3", "0",  '<a href="vest.svg"><img src="vest.svg" width="200"></img></a>', '<a href="vest.svg">vest.svg</a>', "147KB",   "0", "0", "81", "2",    '<a href="hen_and_chicks.svg"><img src="hen_and_chicks.svg" width="200"></img></a>', '<a href="hen_and_chicks.svg">hen_and_chicks.svg</a>', "78KB", "0", "0", "129", "3",]
    var num = 7;
    for (var i = 0; i < num; i ++){
        addRowToLogPanel(inputs[0 + i*7], inputs[1 + i*7], inputs[2 + i*7], inputs[3 + i*7], inputs[4 + i*7], inputs[5 + i*7], inputs[6 + i*7]);
    }
}

function addDropdownItems(name){
    var menu = document.getElementById('dropdown-content');
    var butt = document.createElement("button");
    butt.innerHTML = name;
    butt.id = name;
    butt.onclick = function(){
        var image = document.getElementById("SVGview");
        image.innerHTML = "<img src=\"" + name + "\" width=\"800\"></img>";
        $("#viewTable tr").remove();
        if (name == "Emoji_poo.svg") createSVGViewTable(name, Emoji_pooTitle, Emoji_pooDescription);
        if (name == "quad01_A2.svg") createSVGViewTable(name, quad01_A2Title, quad01_A2Description);
        if (name == "Emoji_party_A2.svg") createSVGViewTable(name, Emoji_party_A2Title, Emoji_party_A2Description);
        if (name == "Emoji_grinning.svg") createSVGViewTable(name, Emoji_grinningTitle, Emoji_grinningDescription);
        if (name == "Emoji_shades.svg") createSVGViewTable(name, Emoji_shadesTitle, Emoji_shadesDescription);
        if (name == "vest.svg") createSVGViewTable(name, vestTitle, vestDescription);
        if (name == "hen_and_chicks.svg") createSVGViewTable(name, hen_and_chicksTitle, hen_and_chicksDescription);
    }
    menu.appendChild(butt);
}

$(document).ready(function() {
    var image = document.getElementById('SVGview');
    image.innerHTML = "<img src=\"Emoji_poo.svg" + "\" width=\"800\"></img>";
    var inputs = ['<a href="Emoji_poo.svg"><img src="Emoji_poo.svg" width="200"></img></a>', '<a href="Emoji_poo.svg">Emoji_poo.svg</a>', "1KB", "0", "2", "6", "1",    '<a href="quad01_A2.svg"><img src="quad01_A2.svg" width="200"></img></a>', '<a href="quad01_A2.svg">quad01_A2.svg</a>', "1KB", "1", "5", "2", "3",    '<a href="Emoji_party_A2.svg"><img src="Emoji_party_A2.svg" width="200"></img></a>', '<a href="Emoji_party_A2.svg">Emoji_party_A2.svg</a>', "3KB", "0", "0", "21", "2",    '<a href="Emoji_grinning.svg"><img src="Emoji_grinning.svg" width="200"></img></a>', '<a href="Emoji_grinning.svg">Emoji_grinning.svg</a>', "1KB", "0", "3", "4", "1",   '<a href="Emoji_shades.svg"><img src="Emoji_shades.svg" width="200"></img></a>', '<a href="Emoji_shades.svg">Emoji_shades.svg</a>', "1KB", "0", "0", "3", "0",  '<a href="vest.svg"><img src="vest.svg" width="200"></img></a>', '<a href="vest.svg">vest.svg</a>', "147KB",   "0", "0", "81", "2",    '<a href="hen_and_chicks.svg"><img src="hen_and_chicks.svg" width="200"></img></a>', '<a href="hen_and_chicks.svg">hen_and_chicks.svg</a>', "78KB", "0", "0", "129", "3",]
    var inputNames = ["Emoji_poo.svg", "quad01_A2.svg", "Emoji_party_A2.svg", "Emoji_grinning.svg", "Emoji_shades.svg", "vest.svg", "hen_and_chicks.svg"];
    var num = Math.floor(Math.random()*3 + 5);
    for (var i = 0; i < num; i ++){
        addRowToLogPanel(inputs[0 + i*7], inputs[1 + i*7], inputs[2 + i*7], inputs[3 + i*7], inputs[4 + i*7], inputs[5 + i*7], inputs[6 + i*7]);
        addDropdownItems(inputNames[i]);
    }
    createSVGViewTable("Emoji_poo.svg", Emoji_pooTitle, Emoji_pooDescription);
});


function createSVGViewTable(name, newtitle, newdescription){
    if (!document.getElementsByTagName) return;
    var table = document.getElementById('viewTable');
    tabHead = document.getElementsByTagName("thead").item(1);
    headerRow = document.createElement("tr");
    headerCell1 = document.createElement("th");
    headerCell1.width = "450";
    headerCell2 = document.createElement("th");
    headerCell2.width = "450";
    headerCell2.colSpan = "2";
    
    var titleButton = document.createElement("button");
    titleButton.setAttribute('content', 'Title');
    titleButton.setAttribute('class', 'btn');
    titleButton.id = 'titleButton';
    var string = "Title (Click to edit)"
    titleButton.innerHTML = string.bold();

    var descriptionButton = document.createElement("button");
    descriptionButton.setAttribute('content', 'Title');
    descriptionButton.setAttribute('class', 'btn');
    descriptionButton.id = 'descriptionButton';
    string = "Description (Click to edit)"
    descriptionButton.innerHTML = string.bold();

    headerCell1.appendChild(titleButton);
    headerCell2.appendChild(descriptionButton);
    headerRow.appendChild(headerCell1);
    headerRow.appendChild(headerCell2);
    tabHead.appendChild(headerRow);

    var isClicked = 0;
    var isClicked2 = 0;
    titleButton.onclick = function(){
        if (isClicked == 1) return;
        else isClicked = 1;
        var div = document.createElement("div");
        div.id = 'titleEdit';
        var label = document.createElement("label");
        label.for = "entryBox";
        var input = document.createElement("input");
        input.type = "text";
        input.class = "form-control";
        input.id = 'title-entryForm';
        input.value = "";
        input.placeholder = "Enter New Title";
        var div1 = document.createElement("div");
        div1.id = 'titleEdit-submit';
        var submit1 = document.createElement("button");
        submit1.class = "btn btn-secondary";
        submit1.type = 'submit';
        submit1.value = "submit";
        submit1.innerHTML="Change Title";
        submit1.id = 'submit1';
        div1.appendChild(submit1);
        div.appendChild(label);
        div.appendChild(input);
        headerCell1.appendChild(div);
        headerCell1.appendChild(div1);
        submit1.onclick = function(){
            if (name == "Emoji_poo.svg") {
                Emoji_pooTitle = $('#title-entryForm').val();
                $('#titleButton').html(Emoji_pooTitle.bold());
                isClicked = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, Emoji_pooTitle, Emoji_pooDescription);
            }
            if (name == "quad01_A2.svg") {
                quad01_A2Title = $('#title-entryForm').val();
                $('#titleButton').html(quad01_A2Title.bold());
                isClicked = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, quad01_A2Title, quad01_A2Description);
            }
            if (name == "Emoji_grinning.svg") {
                Emoji_grinningTitle = $('#title-entryForm').val();
                $('#titleButton').html(Emoji_grinningTitle.bold());
                isClicked = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, Emoji_grinningTitle, Emoji_grinningDescription);
            }
            if (name == "Emoji_shades.svg") {
                Emoji_shadesTitle = $('#title-entryForm').val();
                $('#titleButton').html(Emoji_shadesTitle.bold());
                isClicked = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, Emoji_shadesTitle, Emoji_shadesDescription);
            }
            if (name == "Emoji_party_A2.svg") {
                Emoji_party_A2Title = $('#title-entryForm').val();
                $('#titleButton').html(Emoji_party_A2Title.bold());
                isClicked = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, Emoji_party_A2Title, Emoji_party_A2Description);
            }
            if (name == "vest.svg") {
                vestTitle = $('#title-entryForm').val();
                $('#titleButton').html(vestTitle.bold());
                isClicked = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, vestTitle, vestDescription);
            }
            if (name == "hen_and_chicks.svg") {
                hen_and_chicksTitle = $('#title-entryForm').val();
                $('#titleButton').html(hen_and_chicksTitle.bold());
                isClicked = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, hen_and_chicksTitle, hen_and_chicksDescription);
            }
        };
    };

    descriptionButton.onclick = function(){
        if (isClicked2 == 1) return;
        else isClicked2 = 1;
        var div = document.createElement("div");
        div.id = 'descriptionEdit';
        var label = document.createElement("label");
        label.for = "entryBox";
        var input = document.createElement("input");
        input.type = "text";
        input.class = "form-control";
        input.id = 'description-entryForm';
        input.value = "";
        input.placeholder = "Enter New Description";
        var div2 = document.createElement("div");
        div2.id = 'DescriptionEdit-submit';
        var submit2 = document.createElement("button");
        submit2.class = "btn btn-secondary";
        submit2.type = 'submit';
        submit2.value = "submit";
        submit2.innerHTML="Change Description";
        submit2.id = 'submit2';
        div2.appendChild(submit2);
        div.appendChild(label);
        div.appendChild(input);
        headerCell2.appendChild(div);
        headerCell2.appendChild(div2);
        submit2.onclick = function(){
            if (name == "Emoji_poo.svg") {
                Emoji_pooDescription = $('#description-entryForm').val();
                $('#descriptionButton').html(Emoji_pooDescription.bold());
                isClicked2 = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, Emoji_pooTitle, Emoji_pooDescription);
            }
            if (name == "quad01_A2.svg") {
                quad01_A2Description = $('#description-entryForm').val();
                $('#descriptionButton').html(quad01_A2Description.bold());
                isClicked2 = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, quad01_A2Title, quad01_A2Description);
            }
            if (name == "Emoji_grinning.svg") {
                Emoji_grinningDescription = $('#description-entryForm').val();
                $('#descriptionButton').html(Emoji_grinningDescription.bold());
                isClicked2 = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, Emoji_grinningTitle, Emoji_grinningDescription);
            }
            if (name == "Emoji_shades.svg") {
                Emoji_shadesDescription = $('#description-entryForm').val();
                $('#descriptionButton').html(Emoji_shadesTitle.bold());
                isClicked2 = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, Emoji_shadesDescription, Emoji_shadesDescription);
            }
            if (name == "Emoji_party_A2.svg") {
                Emoji_party_A2Description = $('#description-entryForm').val();
                $('#descriptionButton').html(Emoji_party_A2Description.bold());
                isClicked2 = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, Emoji_party_A2Description, Emoji_party_A2Description);
            }
            if (name == "vest.svg") {
                vestDescription = $('#description-entryForm').val();
                $('#descriptionButton').html(vestDescription.bold());
                isClicked2 = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, vestTitle, vestDescription);
            }
            if (name == "hen_and_chicks.svg") {
                hen_and_chicksDescription = $('#description-entryForm').val();
                $('#descriptionButton').html(hen_and_chicksDescription.bold());
                isClicked2 = 0;
                $("#viewTable tr").remove();
                createSVGViewTable(name, hen_and_chicksTitle, hen_and_chicksDescription);
            }
        };
    };
    
    tabBody = document.getElementsByTagName("tbody").item(1);
    row1 = document.createElement("tr");
    cell1 = document.createElement("td");
    cell2 = document.createElement("td");
    if (name == "quad01_A2.svg"){
        if (quad01_A2Title == "") textNode1 = document.createTextNode("Example quad01  -  quadratic Bézier commands in path data");
        else textNode1 = document.createTextNode(newtitle);
        if (newdescription == "") textNode2 = document.createTextNode("Picture showing a \"Q\" a \"T\" command, along with annotations showing the control points and end points");
        else textNode2 = document.createTextNode(newdescription);
    }
    if (name != "quad01_A2.svg"){
        if (newtitle == "")textNode1 = document.createTextNode("N/A");
        else textNode1 = document.createTextNode(newtitle);
        if (newdescription == "")textNode2 = document.createTextNode("N/A");
        else textNode2 = document.createTextNode(newdescription);
    }
    cell2.colSpan = "2";

    cell1.appendChild(textNode1);
    cell2.appendChild(textNode2);
    row1.appendChild(cell1);
    row1.appendChild(cell2);
    tabBody.appendChild(row1);

    header2Row = document.createElement("tr");
    header2Cell1 = document.createElement("th");
    header2Cell1.width = "300";
    header2Cell2 = document.createElement("th");
    header2Cell2.width = "300";
    header2Cell3 = document.createElement("th");
    header2Cell3.width = "300";
    header2TextNode1 = document.createTextNode("Component");
    header2TextNode2 = document.createTextNode("Summary");
    header2TextNode3 = document.createTextNode("Other Attributes (Click to view)");
    header2Cell1.appendChild(header2TextNode1);
    header2Cell2.appendChild(header2TextNode2);
    header2Cell3.appendChild(header2TextNode3);
    header2Row.appendChild(header2Cell1);
    header2Row.appendChild(header2Cell2);
    header2Row.appendChild(header2Cell3);
    tabBody.appendChild(header2Row);

    row2 = document.createElement("tr");
    row2Cell1 = document.createElement("td");
    row2Cell2 = document.createElement("td");
    row2Cell3 = document.createElement("td");

    

    if (name == "Emoji_poo.svg"){
        addRowToViewPanel("Circle 1", "cx=1, cy=1, r=1", "1", count);
        count ++;
        for (var i = 0; i < 5; i ++){
            addRowToViewPanel("Path " +  (i+1), "d= M200,300 L400,50 L600,300 L800,550 L1000,300", "1", count);
            count ++;
        }
        addRowToViewPanel("Group 1", "2 child elements", "0", count);
        count ++;
    }
    else if (name == "quad01_A2.svg"){
        addRowToViewPanel("Rectangle 1", "x=1, y=1, width=1, height=1", "3", count);
        count ++;
        addRowToViewPanel("Circle 1", "cx=1, cy=1, r=1", "1", count);
        count ++;
        addRowToViewPanel("Path 1", "d= M200,300 L400,50 L600,300 L800,550 L1000,300", "1", count);
        count ++;
        addRowToViewPanel("Group 1", "1 child", "0", count);
        count ++;
        addRowToViewPanel("Group 2", "3 children", "0", count);
        count ++;
        addRowToViewPanel("Group 3", "2 children", "0", count);
        count ++;
    }
    else if (name == "Emoji_party_A2.svg"){
        for (var i = 0; i < 17; i ++){
            if (i <= 10){
                addRowToViewPanel("Path " + (i+1), "d= M200,300 L400,50 L600,300 L800,550 L1000,300", "1", count);
                count ++;
            }
            else {
                addRowToViewPanel("Path " + (i+1), "d= M200,300 L400,50 L600,300 L800,550 L1000,300", "2", count);
                count ++;
            }
        }
    }
    else if (name == "Emoji_grinning.svg"){
        addRowToViewPanel("Circle 1", "cx=1, cy=1, r=1", "1");
        for (var i = 0; i < 4; i ++){
            addRowToViewPanel("Path " + (i+1), "d= M200,300 L400,50 L600,300 L800,550 L1000,300", "1", count);
            count ++;
        }
        addRowToViewPanel("Group 1", "0 children", "1", count);
        count ++;
    }
    else if (name == "Emoji_shades.svg"){
        for (var i = 0; i < 4; i ++){
            addRowToViewPanel("Path " + (i+1), "d= M200,300 L400,50 L600,300 L800,550 L1000,300", "1", count);
            count ++;
        }
    }
    else if (name == "vest.svg"){
        addRowToViewPanel("Group 1", "1 children", "0", count);
        count ++;
    }
    else if (name == "hen_and_chicks.svg"){
        addRowToViewPanel("Group 1", "2 children", "1", count);
        count ++;
    }
    
}

function addRowToViewPanel(info1, info2, info3, i){
    tabBody=document.getElementsByTagName("tbody").item(1);
    row=document.createElement("tr");
    cell1 = document.createElement("td");
    cell2 = document.createElement("td");
    cell3 = document.createElement("td");
    textnode1=document.createTextNode(info1);
    textnode2=document.createTextNode(info2);
    var otherAttributes = document.createElement("button");
    var division = document.createElement("div");
    division.id = 'otherAttributes' + i;
    otherAttributes.setAttribute('content', info3);
    otherAttributes.setAttribute('class', 'btn');
    otherAttributes.innerHTML = info3;
    cell1.appendChild(textnode1);
    cell2.appendChild(textnode2);
    cell3.appendChild(otherAttributes);
    cell3.appendChild(division);
    row.appendChild(cell1);
    row.appendChild(cell2);
    row.appendChild(cell3);
    tabBody.appendChild(row);
     otherAttributes.onclick = function(){
        if (info3 == "0") return;
        if (info1.includes("Rectangle")){
            $('#otherAttributes' + i).html("fill=\"none\" | stroke=\"blue\" | stroke-width=\"1\"");
        }
        if (info1.includes("Circle")){
            $('#otherAttributes' + i).html("fill=\"none\"");
        }
        if (info1.includes("Path")){
            if (info3 == "1"){
                $('#otherAttributes' + i).html("fill=\"none\"");
            }
            if (info3 == "2"){
                $('#otherAttributes' + i).html("fill=\"none\" | stroke=\"#888888\"");
            }
            if (info3 == "3"){
                $('#otherAttributes' + i).html("fill=\"none\" | stroke=\"#888888\" | stroke-width=\"2\"");
            }
        }
        if (info1.includes("Group")){
            $('#otherAttributes' + i).html("id=\"path5\"");
        }
    };
}

function addRowToLogPanel(row1, row2, row3, row4, row5, row6, row7){
    if (!document.getElementsByTagName) return;
    tabBody=document.getElementsByTagName("tbody").item(0);
    row=document.createElement("tr");
    cell1 = document.createElement("td");
    cell2 = document.createElement("td");
    cell3 = document.createElement("td");
    cell4 = document.createElement("td");
    cell5 = document.createElement("td");
    cell6 = document.createElement("td");
    cell7 = document.createElement("td");
    cell1.innerHTML = row1;
    cell2.innerHTML = row2;
    textnode3=document.createTextNode(row3);
    textnode4=document.createTextNode(row4);
    textnode5=document.createTextNode(row5);
    textnode6=document.createTextNode(row6);
    textnode7=document.createTextNode(row7);
    cell3.appendChild(textnode3);
    cell4.appendChild(textnode4);
    cell5.appendChild(textnode5);
    cell6.appendChild(textnode6);
    cell7.appendChild(textnode7);
    row.appendChild(cell1);
    row.appendChild(cell2);
    row.appendChild(cell3);
    row.appendChild(cell4);
    row.appendChild(cell5);
    row.appendChild(cell6);
    row.appendChild(cell7);
    tabBody.appendChild(row); 
};






