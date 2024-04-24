var heart = document.getElementById("heart");
var display = document.getElementById("display");

var pulse_data = [];
var last = "0";
var bpm;
var pulse_us;
var pulse_ms;

heart.style.setProperty('--time',pulse_ms + "ms");
heart.addEventListener("animationiteration", function(){
    heart.style.setProperty('animation-duration', pulse_ms + "ms");
});

setInterval(() => {
    fetch('/data.json'.concat("?last=", last))
        .then( response => response.json() )
        .then( response => {
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
            heart.classList.remove('pulse');
            heart.classList.add('deactivated');
            heart.classList.remove('activated');
            display.innerHTML = "___ Bpm";
            throw (error);
            });
}, 1000);
