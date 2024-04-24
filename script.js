var pulse = document.getElementById("pulse");
var activate = document.getElementById("activation");
var heart = document.getElementById("heart");

pulse.addEventListener("click", function() {
    heart.classList.add('pulse');
});
heart.addEventListener("animationend", function(){
    heart.classList.remove('pulse');
});
activate.addEventListener("click", function() {
    heart.classList.toggle('deactivated');
    heart.classList.toggle('activated');
});

var hr_socket = new WebSocket("ws://anything:44242/");
console.log(hr_socket);
