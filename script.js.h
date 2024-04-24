// converted from script.js using file_to_char.py by etyloppihacilem.
// https://github.com/etyloppihacilem/file_to_char

#pragma once
#ifndef SCRIPT_JS_H
#define SCRIPT_JS_H

static const char script_js_h[] = "var pulse=document.getElementById(\"pulse\");var activate=document.getElementById(\"activation\");var heart=document.getElementById(\"heart\");pulse.addEventListener(\"click\",function(){heart.classList.add('pulse');});heart.addEventListener(\"animationend\",function(){heart.classList.remove('pulse');});activate.addEventListener(\"click\",function(){heart.classList.toggle('deactivated');heart.classList.toggle('activated');});var hr_socket=new WebSocket(\";";

#endif