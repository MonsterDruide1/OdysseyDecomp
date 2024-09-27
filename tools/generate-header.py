#!/usr/bin/env python3

import os
import re

CURRENT_FOLDER = os.path.dirname(os.path.abspath(__file__))

with open(os.path.join(CURRENT_FOLDER, 'functions.txt')) as f:
    file = f.read()

functions_block = file.split("#VTABLE")[0]
vtable_block = file.split("#VTABLE")[1]

functions = functions_block.split('\n')
functions = [x.split("\t")[0] for x in functions if x]

functions_demangled = []
for f in functions:
    demangled = os.popen(f"c++filt {f}").read().strip()
    demangled = re.sub(r"(^|([\(,]))\s*([^\s\(,]+) const([&*])", r"\g<1>const \g<3>\g<4>", demangled)
    demangled = demangled.replace("<float>", "f")
    demangled = re.sub(r"([^\w])float", r"\g<1>f32", demangled)
    demangled = re.sub(r"([^\w])unsigned int", r"\g<1>u32", demangled)
    demangled = re.sub(r"([^\w])int", r"\g<1>s32", demangled)
    demangled = re.sub(r"([^\w])unsigned short", r"\g<1>u16", demangled)
    demangled = re.sub(r"([^\w])short", r"\g<1>s16", demangled)
    demangled = re.sub(r"([^\w])unsigned char", r"\g<1>u8", demangled)
    demangled = re.sub(r"([^\w])unsigned long", r"\g<1>u64", demangled)
    demangled = re.sub(r"([^\w])long", r"\g<1>s64", demangled)
    demangled = demangled[demangled.find("::")+2:]

    functions_demangled.append(demangled)

vtable_functions = [x.split(" ")[-1] for x in vtable_block.split('\n') if x]
for f in vtable_functions:
    if f in functions:
        index = functions.index(f)
        functions_demangled[index] = functions_demangled[index] + " override"

functions_demangled = ["UNK_RET "+f+";" for f in functions_demangled]

print("\n".join(functions_demangled))
