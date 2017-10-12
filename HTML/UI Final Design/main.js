var tasks_list = document.getElementById("tasks").getElementsByTagName("tr");
var add_button = document.getElementById("add_new");
var table = document.getElementById("mytable");

// Clear all selections
function reset_task_choice() {
	for (i=0 ; i < tasks_list.length-1; i++) {
		tasks_list[i].style.background="aliceblue";
        tasks_list[i].cells[5].innerHTML=" ► ";
	}
	return
}

// On task click, either select the task and remove all other task selections or unselect the task.
function task_onclick(e) {
    console.log("task_onclick")
    var task = e.target.parentNode
    if (task.cells[5].innerHTML==" ► ") {   
            console.log("aliceblue")
            //reset_task_choice()
            task.style.background="linear-gradient(to left,lightgreen,white)"
            task.style.backgroundAttachment="fixed"
            task.cells[5].innerHTML="&#10074; &#10074;"
    } else {
            console.log("else")
            task.style.background="aliceblue"
            task.cells[5].innerHTML=" ► ";
    }
    updateCookies();
}

// Apply the task_onclick() to all tasks
function apply_task_onclick() {
    for (i = 0; i < tasks_list.length-1 ; i++) {
        var task = tasks_list[i];
        //task.style.background="aliceblue"
        task.onclick = function (e) {task_onclick(e)};
    }
}

function delete_onclick (e) {
    var i = e.target.parentNode.parentNode.rowIndex;
    if (confirm("Are you sure you want to delete?")){
        table.deleteRow(i);
        updateCookies();
    }
}

// Create add button functionality
add_button.onclick = function (e) {
    add_button.style.visibility = "hidden";
    var table_length = tasks_list.length
    var row = table.insertRow(table_length);
    // Add individual names
    var checkbox_cell = row.insertCell(0);
    var name_cell = row.insertCell(1);
    var time_spent_cell = row.insertCell(2);
    var expected_duration_cell = row.insertCell(3);
    var deadline_cell = row.insertCell(4);
    var confirm_cell = row.insertCell(5);
    var trash_cell = row.insertCell(6);
    //Add cell data
    checkbox_cell.innerHTML = '<input type="checkbox" unchecked="true" onchange=celebrate()>';
    name_cell.innerHTML ='<input type="text" id="name_'+table_length+'" value="Task name">'
    time_spent_cell.innerHTML = "0 minutes";
    expected_duration_cell.innerHTML = '<input type="number" id="duration_cell" value="2">';
    deadline_cell.innerHTML='<input type="datetime-local" id="calendar_cell">';
    confirm_cell.innerHTML='<button id="add">Save</button>';
    //Making the create button set the new row
    var add = document.getElementById("add");
    add.onclick = function(d){
        add_button.style.visibility = "visible";
        trash_cell.innerHTML= '<button onclick=delete()>&#128465;</button>';
        trash_cell.childNodes[0].onclick = function (e) {delete_onclick(e)};
        name_cell.innerHTML = name_cell.firstElementChild.value;
        expected_duration_cell.innerHTML = expected_duration_cell.firstElementChild.value + " hours";
        deadline_cell.innerHTML= deadline_cell.firstElementChild.value.substr(5,2) + "/" +
        deadline_cell.firstElementChild.value.substr(8,2) + "/" +
        deadline_cell.firstElementChild.value.substr(2,2) + ", " +
        deadline_cell.firstElementChild.value.substr(11,5) + "am";

        //converting from 24->12 hour time
        if (deadline_cell.innerHTML.substr(10,2) > 12) {
            var hour = deadline_cell.innerHTML.substr(10,2) - 12;
            deadline_cell.innerHTML = deadline_cell.innerHTML.substr(0,10) + hour + deadline_cell.innerHTML.substr(12,3) + "pm";
        }
        
        confirm_cell.innerHTML=" ► "
        
        //code for showing a little message confirming the add
        var mess = document.getElementById("confirm-message");
        mess.innerHTML= "Successfully added task";
        setTimeout(function(){mess.innerHTML="";},3000);

        //apply task functionality to all rows
        apply_task_onclick();
        
        //update cookies
        updateCookies();
    }
}

apply_task_onclick()

function celebrate() {
    document.getElementById("celly").style.visibility = "visible";
    setTimeout(function(){document.getElementById("celly").style.visibility = "hidden"},3000);
    updateCookies();
}

function updateCookies() {
    setCookie("rows",tasks_list.length-1,30);    
    for (var i = 0; i < tasks_list.length-1; i++) {
        setCookie(i+"check",tasks_list[i].cells[0].childNodes[0].checked,30);
        setCookie(i+"task",tasks_list[i].cells[1].innerHTML,30);
        setCookie(i+"timespent",tasks_list[i].cells[2].innerHTML,30); 
        setCookie(i+"expectedtime",tasks_list[i].cells[3].innerHTML,30);   
        setCookie(i+"deadline",tasks_list[i].cells[4].innerHTML,30);   
        setCookie(i+"status",tasks_list[i].cells[5].innerHTML,30);
        //location.reload();
    }
}

function setCookie(cname,cvalue,exdays) {
    var d = new Date();
    d.setTime(d.getTime() + (exdays*24*60*60*1000));
    var expires = "expires=" + d.toGMTString();
    document.cookie = cname + "=" + cvalue + ";" + expires + ";path=/";
}

function getCookie(cname) {
    var name = cname + "=";
    var decodedCookie = decodeURIComponent(document.cookie);
    var ca = decodedCookie.split(';');
    for(var i = 0; i < ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}

function checkCookie() {
    var rownum = getCookie("rows");
    for (var i = 0; i < rownum; i++) {
        var time = new Date(getCookie(i+"deadline").substr(0,8));
        var tomorrow = new Date(new Date().getTime() + 24 * 60 * 60 * 1000);
        var week = new Date(new Date().getTime() + 7 * 24 * 60 * 60 * 1000);        
        var table_length = tasks_list.length
        var row = table.insertRow(table_length);
        var checkbox_cell = row.insertCell(0);
        var name_cell = row.insertCell(1);
        var time_spent_cell = row.insertCell(2);
        var expected_duration_cell = row.insertCell(3);
        var deadline_cell = row.insertCell(4);
        var confirm_cell = row.insertCell(5); 
        var trash_cell = row.insertCell(6);    
        checkbox_cell.innerHTML = '<input type="checkbox" onchange=celebrate()>';
        if (getCookie(i+"check") == "true") {
            checkbox_cell.childNodes[0].checked = "true";
        } else {
            checkbox_cell.childNodes[0].unchecked = "true";
        }
        name_cell.innerHTML = getCookie(i+"task");
        time_spent_cell.innerHTML = getCookie(i+"timespent");
        expected_duration_cell.innerHTML = getCookie(i+"expectedtime");
        deadline_cell.innerHTML = getCookie(i+"deadline");
        confirm_cell.innerHTML = getCookie(i+"status");
        if (confirm_cell.innerHTML != "►") {
            tasks_list[i].style.background="linear-gradient(to left,lightgreen,white)";
            tasks_list[i].style.backgroundAttachment="fixed"
        } else {
            tasks_list[i].style.background="aliceblue"
        }
        trash_cell.innerHTML= '<button onclick=delete()>&#128465;</button>';
        trash_cell.childNodes[0].onclick = function (e) {delete_onclick(e)};
        
        if ((document.URL.includes("index")) && (!(time < tomorrow) || (getCookie(i+"check") == "true"))) {
            row.style.display = "none";
        }
        if ((document.URL.includes("thisweek")) && (!(time < week) || (getCookie(i+"check") == "true"))) {
            row.style.display = "none";
        } 
        if ((document.URL.includes("alltasks")) && (getCookie(i+"check") == "true")) {
            row.style.display = "none";
        }
        if ((document.URL.includes("completedtasks")) && (getCookie(i+"check") != "true")) {
            row.style.display = "none";
        }
    }
    apply_task_onclick();
}








