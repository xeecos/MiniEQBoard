document.getElementById("defaultOpen").click();
document.getElementById("speed-range").value = localStorage.getItem("speed")||1;
document.getElementById("speed-input").value = Math.pow(2,document.getElementById("speed-range").value-1);
document.getElementById("speed-range").oninput = function () {
    document.getElementById("speed-range").value = this.value;
    document.getElementById("speed-input").value = Math.pow(2,document.getElementById("speed-range").value-1);
    localStorage.setItem("speed", this.value);
}
document.getElementById("current-input").value = document.getElementById("current-range").value = localStorage.getItem("current")||1000;
document.getElementById("current-input").oninput = document.getElementById("current-range").oninput = function () {
    document.getElementById("current-range").value = this.value;
    document.getElementById("current-input").value = this.value;
    localStorage.setItem("current", this.value);
    onCurrent();
}
document.getElementById("ratio-input").value = document.getElementById("ratio-range").value = localStorage.getItem("ratio")||34;
document.getElementById("ratio-input").oninput = document.getElementById("ratio-range").oninput = function () {
    document.getElementById("ratio-range").value = this.value;
    document.getElementById("ratio-input").value = this.value;
    localStorage.setItem("ratio", this.value);
    onRatio();
}
document.getElementById("microsteps-range").value = localStorage.getItem("microsteps")||9;
document.getElementById("microsteps-input").value = Math.pow(2,document.getElementById("microsteps-range").value-1);
document.getElementById("microsteps-range").oninput = function () {
    document.getElementById("microsteps-input").value = Math.pow(2,document.getElementById("microsteps-range").value-1);
    localStorage.setItem("microsteps", this.value);
    onMicrosteps();
}
$(function () {
    window.Translator.translate(localStorage.getItem("lang"));
    document.getElementById("lang-select").value = localStorage.getItem("lang")||"english";
});
document.getElementById("lang-select").oninput = function()
{
    window.Translator.translate(this.value);
    $(function () {
        window.Translator.translate(this.value);
    });
    localStorage.setItem("lang", this.value);
}

function openPage(evt, pageName) {
    let i, tabcontent, tablinks;
    tabcontent = document.getElementsByClassName("tabcontent");
    for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
    }
    tablinks = document.getElementsByClassName("tablinks");
    for (i = 0; i < tablinks.length; i++) {
        tablinks[i].className = tablinks[i].className.replace(" active", "");
    }
    document.getElementById(pageName).style.display = "block";
    evt.currentTarget.className += " active";
}
let waitTime = 1000;
function request(url) {
    return new Promise(resolve => {
        // document.getElementById('appLoading').style.display = 'block';
        let oReq = new XMLHttpRequest();
        oReq.addEventListener("load", function () {
            document.getElementById('appLoading').style.display = 'none'
            resolve(this.responseText);
        });
        oReq.open("GET", url);
        oReq.send();
    });
}
async function onForwardDirection() {
    // document.getElementById('appLoading').style.display = 'block';
    await onSpeed(1);
}
async function onBackwardDirection() {
    // document.getElementById('appLoading').style.display = 'block';
    await onSpeed(-1);
}
function onSpeed(dir) {
    return new Promise(resolve => {
        let speed = document.getElementById('speed-input').value/240.0;
        request(`/speed/set?speed=${speed*dir}`).then(res => {
            resolve(JSON.parse(res))
        });
    })
}
function onMicrosteps() {
    return new Promise(resolve => {
        let microsteps = document.getElementById('microsteps-input').value;
        request(`/stepper/set?microsteps=${microsteps}`).then(res => {
            resolve(JSON.parse(res))
        });
    })
}
function onCurrent() {
    return new Promise(resolve => {
        let current = document.getElementById('current-input').value;
        request(`/stepper/set?current=${current}`).then(res => {
            resolve(JSON.parse(res))
        });
    })
}
function onRatio() {
    return new Promise(resolve => {
        let ratio = document.getElementById('ratio-input').value;
        request(`/stepper/set?ratio=${ratio}`).then(res => {
            resolve(JSON.parse(res))
        });
    })
}