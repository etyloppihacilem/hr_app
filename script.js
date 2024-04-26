var heart = document.getElementById("heart");
var display = document.getElementById("display");
var con = document.getElementById("notconnected");

var pulse_data = [];
var last = "0";
var bpm = 0;
var pulse_us;
var pulse_ms;
const moy = 7;
const ecart_max = 2000000;

heart.style.setProperty('--time',pulse_ms + "ms");
heart.addEventListener("animationiteration", function(){
    heart.classList.remove('pulse');
    heart.style.setProperty('animation-duration', pulse_ms + "ms");
    heart.classList.add('pulse');
});

var error_count = 0;

function get_pulse_us(n = 5) {
    var ret = 0;
    if (n + 1 > pulse_data.length) {
        return 0;
    }
    for (var i = 1; i <= n; i++) {
        ret += pulse_data[pulse_data.length - i] - pulse_data[pulse_data.length - (i + 1)];
    }
    ret /= n;
    return ret;
}

var id_socket = window.setInterval(() => {
    fetch('/data.json'.concat("?last=", last))
        .then( response => response.json() )
        .then( response => {
            error_count = 0;
            pulse_data.push(...response.data);
            pulse_data.sort(function(a, b) {
                return a - b;
            });
            last = pulse_data[pulse_data.length - 1];
            if (pulse_data.length >= moy) {
                if (last_time - pulse_data[pulse_data.length - moy] > moy * ecart_max) {
                    heart.classList.remove('pulse');
                    heart.classList.add('deactivated');
                    heart.classList.remove('activated');
                    display.innerHTML = "___ Bpm";
                    return;
                }
                pulse_us = get_pulse_us(moy);
                if (pulse_us == 0) {
                    heart.classList.remove('pulse');
                    heart.classList.add('deactivated');
                    heart.classList.remove('activated');
                    display.innerHTML = "___ Bpm";
                } else {
                    heart.classList.add('pulse');
                    heart.classList.remove('deactivated');
                    heart.classList.add('activated');
                    bpm = 60000000 / pulse_us;
                    pulse_ms = pulse_us / 1000;
                    display.innerHTML = Math.round(bpm) + " Bpm";
                }
            } else {
                heart.classList.remove('pulse');
                heart.classList.add('deactivated');
                heart.classList.remove('activated');
                display.innerHTML = "___ Bpm";
            }
        })
        .catch(error => {
            error_count += 1;
            if (error_count > 10) {
                window.clearInterval(id_socket);
                console.log("Too many error, stopping now.");
            }
            heart.classList.remove('pulse');
            heart.classList.add('deactivated');
            heart.classList.remove('activated');
            display.innerHTML = "XXX Bpm";
            throw (error);
            });
}, 1000);

var last_time = 0;
var time_error_count = 0
var id_timeout = window.setInterval(() => {
    fetch('/time.json')
        .then( response => response.json())
        .then( response => {
            time_error_count = 0;
            last_time = response[0];
            if (pulse_data.length < moy || last_time - pulse_data[pulse_data.length - moy] > moy * ecart_max) {
                heart.classList.remove('pulse');
                heart.classList.add('deactivated');
                heart.classList.remove('activated');
                display.innerHTML = "___ Bpm";
            }
        })
        .catch(error => {
            time_error_count += 1;
            if (time_error_count > 10) {
                window.clearInterval(id_timeout);
                console.log("Too many error, stopping now.");
            }
            throw (error);
        });
}, 1000);

con.remove();
