
file = open('file_list_raw.csv', 'r')
lines = file.readlines()[1:]
file.close()

current_file = ""
current_folder = ""

mangled_symbols = []
addresses = []

ignored_libs = ["nn", "sead", "agl", "eui"]

o = open('file_list.csv', 'w')
for line in lines:
    [folder, file, demangled, mangled, address] = line.split(';')
    should_skip = False
    for lib in ignored_libs:
        if demangled.startswith(lib+"::") or mangled.startswith("_ZNSt"):
            should_skip = True
            break

    if should_skip: continue
    if folder != "" and folder != current_folder:
        current_folder = folder
    if file != "" and file != current_file:
        current_file = file
    
    if mangled.startswith("sub_") or mangled.startswith("nullsub_"):
        mangled = ""
        demangled = ""
    
    line = address.strip() + "," + current_folder + "," + current_file + "," + demangled + "\n"
    o.write(line)

    mangled_symbols += [mangled]
    addresses += [int(address[2:], 16)]

o.close()

file = open("odyssey_functions.csv", "r")
lines = file.readlines()[1:]
file.close()

functions_mangled_symbols = []
functions_addresses = []

for line in lines:
    parts = line.split(',')
    mangled = parts[3].strip()
    address = int(parts[0].strip()[2:], 16)

    should_skip = False
    if (not mangled.startswith("_Z") and (not mangled=="" and not mangled.startswith("nullsub"))) or mangled.startswith("_ZNSt"):
        should_skip = True
    for lib in ignored_libs:
        if mangled.startswith("_ZN"+str(len(lib))+lib) or mangled.startswith("_ZNK"+str(len(lib))+lib):
            should_skip = True
            break
    if should_skip: continue

    functions_mangled_symbols += [mangled]
    functions_addresses += [address]

file_off = 0

i = 0
j = 0

while i < len(mangled_symbols) and j < len(functions_mangled_symbols):
    if addresses[i] == functions_addresses[j]:
        if mangled_symbols[i] != functions_mangled_symbols[j]:
            print(f"Different symbol      ({hex(addresses[i])}): FILE={mangled_symbols[i]} vs. CSV={functions_mangled_symbols[j]}")
        i += 1
        j += 1
    else:
        if addresses[i] < functions_addresses[j]:
            print(f"Missing symbol in CSV ({hex(addresses[i])}): FILE={mangled_symbols[i]}")
            i += 1
        else:
            print(f"Extra symbol in CSV   ({hex(functions_addresses[j])}): CSV={functions_mangled_symbols[j]}")
            j += 1


file = open('file_list_raw.csv', 'r')
lines = file.readlines()[1:]
o = open('file_list.yaml', 'w')
for line in lines:
    [folder, file, demangled, mangled, address] = line.split(';')
    should_skip = False
    for lib in ignored_libs:
        if demangled.startswith(lib+"::") or mangled.startswith("_ZNSt"):
            should_skip = True
            break

    if should_skip: continue
    if folder != "" and folder != current_folder:
        current_folder = folder
    if file != "" and file != current_file:
        current_file = file
        o.write(f"{current_folder}/{current_file}:\n")
    
    if mangled.startswith("sub_") or mangled.startswith("nullsub_"):
        mangled = ""
        demangled = ""
    
    line = f"  - {address.strip()}: {demangled}\n"
    o.write(line)

    mangled_symbols += [mangled]
    addresses += [int(address[2:], 16)]

o.close()
