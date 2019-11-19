window.onload = index()

function index() {
    var timer = document.getElementById("timer");

    var btn1 = document.getElementById("1");
    btn1.onclick = subscribe_1();
    var btn2 = document.getElementById("2");
    btn2.onclick = subscribe_2();

}

function subscribe_1() {
    xhr = new XMLHttpRequest();
    xhr.open('PUT', '127.0.0.1:8080/sub');
    xhr.setRequestHeader('Content-type','application/json; charset=utf-8');
    data = {};
    data.user_id = "";
    data.sub_start_date = new Date();
    data.sub_end_date = new Date(year, month, day, hours, minutes, seconds);
    json = JSON.stringify(data);
    xhr.send(json);
}

function subscribe_2() {
    xhr = new XMLHttpRequest();
}

function load_time() {

}

function tick(timer) {
}

