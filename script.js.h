// converted from script.js using file_to_char.py by etyloppihacilem.
// https://github.com/etyloppihacilem/file_to_char

#pragma once
#ifndef SCRIPT_JS_H
#define SCRIPT_JS_H

static const char script_js_h[] = "var pulse = document.getElementById(\"pulse\");\n"
"var activate = document.getElementById(\"activation\");\n"
"var heart = document.getElementById(\"heart\");\n"
"\n"
"pulse.addEventListener(\"click\", function() {\n"
"    heart.classList.add('pulse');\n"
"});\n"
"heart.addEventListener(\"animationend\", function(){\n"
"    heart.classList.remove('pulse');\n"
"});\n"
"activate.addEventListener(\"click\", function() {\n"
"    heart.classList.toggle('deactivated');\n"
"    heart.classList.toggle('activated');\n"
"});\n"
"\n"
"var hr_socket = new WebSocket(\"ws://anything:9000/\");\n"
"\n"
"hr_socket.onopen = function(e) {\n"
"  console.log(\"[open] Connection established\");\n"
"};\n"
"\n"
"hr_socket.onmessage = function(event) {\n"
"  console.log(`[message] Data received from server: ${event.data}`);\n"
"};\n"
"\n"
"hr_socket.onclose = function(event) {\n"
"  if (event.wasClean) {\n"
"    console.log(`[close] Connection closed cleanly, code=${event.code} reason=${event.reason}`);\n"
"  } else {\n"
"    console.log('[close] Connection died');\n"
"  }\n"
"};\n"
"\n"
"hr_socket.onerror = function(error) {\n"
"  console.log(`[error]`);\n"
"};\n"
"console.log(hr_socket);\n"
"";

#endif