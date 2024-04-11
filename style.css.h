// converted from style.css using file_to_char.py by etyloppihacilem.
// https://github.com/etyloppihacilem/file_to_char

#pragma once
#ifndef STYLE_CSS_H
#define STYLE_CSS_H

static const char style_css_h[] = "body {\n"
"    background-color: #24252d;\n"
"    height: 100%;\n"
"    overflow: hidden;\n"
"    margin: 0;\n"
"}\n"
"#main {\n"
"    margin:10vw 5vw 0 5vw;\n"
"    background-color: #2b2b35;\n"
"    border-radius: 30px;\n"
"    height: 100vh;\n"
"    display: flex;\n"
"    flex-direction: column;\n"
"    align-items: center;\n"
"}\n"
"\n"
"#heart {\n"
"    width:25vw;\n"
"    height:auto;\n"
"    margin: 2vw;\n"
"    /* filter: drop-shadow(0px 0px 5px #ff005788); */\n"
"}\n"
"\n"
".deactivated {\n"
"    opacity:50%;\n"
"    transition-duration: 250ms;\n"
"    transition-timing-function: ease-in-out;\n"
"}\n"
"\n"
".activated {\n"
"    opacity: 90%;\n"
"    transition-duration: 250ms;\n"
"    transition-timing-function: ease-in-out;\n"
"}\n"
"\n"
".pulse {\n"
"    animation: pulse 1s ease-in-out 1;\n"
"}\n"
"\n"
"@keyframes pulse {\n"
"    from {\n"
"        opacity: 90%;\n"
"        width: 25vw;\n"
"        margin: 2vw;\n"
"        filter: drop-shadow(0px 0px 0px #00000000);\n"
"    } 45% {\n"
"        opacity: 100%;\n"
"        width: 27vw;\n"
"        margin: 1vw;\n"
"        filter: drop-shadow(0px 0px 5px #ff0057);\n"
"    } 50% {\n"
"        opacity: 90%;\n"
"        width: 26vw;\n"
"        margin: 1.5vw;\n"
"        filter: drop-shadow(0px 0px 0px #00000000);\n"
"    } 55% {\n"
"        opacity: 100%;\n"
"        width: 27vw;\n"
"        margin: 1vw;\n"
"        filter: drop-shadow(0px 0px 5px #ff0057);\n"
"    } to {\n"
"        opacity: 90%;\n"
"        width: 25vw;\n"
"        margin: 2vw;\n"
"        filter: drop-shadow(0px 0px 0px #00000000);\n"
"    }\n"
"}\n"
"\n"
"button {\n"
"    margin: 5%;\n"
"}\n"
"\n"
"";

#endif