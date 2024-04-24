var heart = document.getElementById("heart");
var display = document.getElementById("display");

var pulse_data = [];
var last = "0";
var bpm = 0;
var pulse_us;
var pulse_ms;

heart.style.setProperty('--time',pulse_ms + "ms");
heart.addEventListener("animationiteration", function(){
    heart.style.setProperty('animation-duration', pulse_ms + "ms");
});

var error_count = 0;

var id_socket = window.setInterval(() => {
    fetch('/data.json'.concat("?last=", last))
        .then( response => response.json() )
        .then( response => {
            error_count = 0;
            pulse_data.push(...response);
            pulse_data.sort(function(a, b) {
                return a - b;
            });;
            last = pulse_data[pulse_data.length - 1];
            if (pulse_data.length >= 3) {
                heart.classList.add('pulse');
                heart.classList.remove('deactivated');
                heart.classList.add('activated');
                pulse_us = (((pulse_data[pulse_data.length - 1] - pulse_data[pulse_data.length - 2]) + (pulse_data[pulse_data.length - 2] - pulse_data[pulse_data.length - 3])) / 2)
                bpm = 60000000 / pulse_us;
                pulse_ms = pulse_us / 1000;
                display.innerHTML = Math.round(bpm) + " Bpm";
            }else {
                heart.classList.remove('pulse');
                heart.classList.add('deactivated');
                heart.classList.remove('activated');
                display.innerHTML = "___ Bpm";
            }
        })
        .catch(error => {
            error_count += 1;
            if (error_count > 1) {
                window.clearInterval(id_socket);
                console.log("Too many error, stopping now.");
            }
            heart.classList.remove('pulse');
            heart.classList.add('deactivated');
            heart.classList.remove('activated');
            display.innerHTML = "___ Bpm";
            throw (error);
            });
}, 1000);


// var graph = document.getElementById('graph');
// var c = graph.getContext('2d');
// const c_width = graph.getBoundingClientRect().width;
// const c_height = graph.getBoundingClientRect().height;
// const w_it = c_width / 60;
// var graph_data = [];
//
// var id_graph = window.setInterval(() => {
//     c.clearRect(0, 0, c_width, c_height);
//     c.lineWidth = 1;
//     c.strokeStyle = '#ff0000';
//     c.font = 'italic 8pt sans-serif';
//     c.textAlign = "center";
//     graph_data.push(bpm);
//     while (graph_data.length > 60)
//         graph_data.shift();
//     var max = Math.max(...graph_data);
//     c.beginPath();
//     c.moveTo(0, c_height - (graph_data[0] / max) * c_height);
//     for (var i = 1; i < graph_data.length; i++) {
//         c.lineTo(i * w_it, c_height - (graph_data[i] / max) * c_height);
//     }
//     c.stroke();
// }, 1000);
//
