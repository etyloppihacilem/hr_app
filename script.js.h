// converted from script.js using file_to_char.py by etyloppihacilem.
// https://github.com/etyloppihacilem/file_to_char

#pragma once
#ifndef SCRIPT_JS_H
#define SCRIPT_JS_H

static const char script_js_h[] = "var heart = document.getElementById(\"heart\");\n"
"var display = document.getElementById(\"display\");\n"
"\n"
"var pulse_data = [];\n"
"var last = \"0\";\n"
"var bpm;\n"
"var pulse_us;\n"
"var pulse_ms;\n"
"\n"
"heart.style.setProperty('--time',pulse_ms + \"ms\");\n"
"heart.addEventListener(\"animationiteration\", function(){\n"
"    heart.style.setProperty('animation-duration', pulse_ms + \"ms\");\n"
"});\n"
"\n"
"setInterval(() => {\n"
"    fetch('/data.json'.concat(\"?last=\", last))\n"
"        .then( response => response.json() )\n"
"        .then( response => {\n"
"            pulse_data.push(...response);\n"
"            pulse_data.sort(function(a, b) {\n"
"                return a - b;\n"
"            });;\n"
"            last = pulse_data[pulse_data.length - 1];\n"
"            if (pulse_data.length >= 3) {\n"
"                heart.classList.add('pulse');\n"
"                heart.classList.remove('deactivated');\n"
"                heart.classList.add('activated');\n"
"                pulse_us = (((pulse_data[pulse_data.length - 1] - pulse_data[pulse_data.length - 2]) + (pulse_data[pulse_data.length - 2] - pulse_data[pulse_data.length - 3])) / 2)\n"
"                bpm = 60000000 / pulse_us;\n"
"                pulse_ms = pulse_us / 1000;\n"
"                display.innerHTML = Math.round(bpm) + \" Bpm\";\n"
"            }else {\n"
"                heart.classList.remove('pulse');\n"
"                heart.classList.add('deactivated');\n"
"                heart.classList.remove('activated');\n"
"            }\n"
"        })\n"
"        .catch(error => {\n"
"            heart.classList.remove('pulse');\n"
"            heart.classList.add('deactivated');\n"
"            heart.classList.remove('activated');\n"
"            throw (error);\n"
"            });\n"
"}, 1000);\n"
"";

#endif