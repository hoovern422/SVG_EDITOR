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

function addRowFileLogPanel(imageName, ){

    if (!document.getElementsByTagName) return;

    // Create the cells
    tableBody = document.getElementsById("fileLogPanelBody");
    row=document.createElement("tr");
    cell1 = document.createElement("td");
    cell2 = document.createElement("td");
    cell3 = document.createElement("td");
    cell4 = document.createElement("td");
    cell5 = document.createElement("td");
    cell6 = document.createElement("td");
    cell7 = document.createElement("td");

    // Add a download link for the image
    cell1.innerHTML = imageName;
    cell2.innerHTML = imageName;


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