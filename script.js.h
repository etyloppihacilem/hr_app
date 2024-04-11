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
"";

#endif