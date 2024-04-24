# ##############################################################################
#
#              """          file_to_char.py
#       -\-    _|__
#        |\___/  . \        Created on 11 Apr. 2024 at 16:54
#        \     /(((/        by hmelica
#         \___/)))/         hmelica@student.42.fr
#
# ##############################################################################

import os, sys
from css_html_js_minify import process_single_html_file, process_single_js_file, process_single_css_file, html_minify, js_minify, css_minify

def minify(file, text):
    if (file[-5:] == ".html"):
        return html_minify(text)
    # elif (file[-3:] == ".js"):
    #     return js_minify(text)
    elif (file[-4:] == ".css"):
        return css_minify(text)
    return text

def file_to_char(file):
    with open(file, 'r') as f:
        text = f.read()
    text = minify(file, text)
    text = text.replace('\\', '\\\\')
    text = text.replace('"', '\\"')
    # text = text.replace('`', '\\`')
    text = text.replace('\n', '\\n"\n"')
    text = text.replace('%', '%%')
    return (text)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python3 file_to_char.py <file1> <file2> ...")
    for file in sys.argv[1:]:
        try:
            new_file = file + ".h"
            var_name = new_file.replace("/", "_")
            var_name = var_name.replace(".", "_")
            with open(new_file, "w") as f:
                f.write("// converted from " + file + " using file_to_char.py by " +
                        "etyloppihacilem.\n" +
                        "// https://github.com/etyloppihacilem/file_to_char\n\n" +
                        "#pragma once\n#ifndef " + var_name.upper() + "\n#define " + var_name.upper() + "\n\n"
                        "static const char " + var_name + "[] = \"")
                f.write(file_to_char(file));
                f.write("\";\n\n#endif")
        except FileNotFoundError:
            print(file, "not found.")
