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

hr_socket.onopen = function(e) {
  console.log("[open] Connection established");
};

hr_socket.onmessage = function(event) {
  console.log(`[message] Data received from server: ${event.data}`);
};

hr_socket.onclose = function(event) {
  if (event.wasClean) {
    console.log(`[close] Connection closed cleanly, code=${event.code} reason=${event.reason}`);
  } else {
    console.log('[close] Connection died');
  }
};

hr_socket.onerror = function(error) {
  console.log(`[error]`);
};
console.log(hr_socket);
