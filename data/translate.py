print("Reading input files...")

# offset: (quality, size, name)
function_csv = {}

with open('odyssey_functions.csv', 'r') as file:
    lines = file.readlines()[1:]
    for line in lines:
        parts = line.split(',')
        # Address,Quality,Size,Name
        function_csv[int(parts[0].strip(), 16)] = (parts[1].strip(), int(parts[2].strip()), parts[3].strip())

# offset: (file, name)
drive_list = {}

with open('file_list_raw.csv', 'r') as file:
    lines = file.readlines()[1:]
    last_folder = ""
    last_file = ""
    for line in lines:
        parts = line.split(';')
        # Folder;Object;Demangled Name;Mangled Name;Start Address
        if parts[0].strip() != "":
            last_folder = parts[0].strip()
        if parts[1].strip() != "":
            last_file = parts[1].strip()
        drive_list[int(parts[4].strip(), 16)] = (last_folder+"/"+last_file, parts[3].strip())

# verify that no function listed in the file list is missing from the function list
for offset in drive_list:
    if offset not in function_csv:
        print("Function missing from function list: %s" % hex(offset))
    elif function_csv[offset][2] != drive_list[offset][1]:
        forbidden_start = ["sub_", "nullsub_", "_ZNK12_GLOBAL__N_", "_ZNK2al12_GLOBAL__N_"]
        if (not any(drive_list[offset][1].startswith(x) for x in forbidden_start)) and ("InformationWindowKoopaCapRequesterNrv" not in drive_list[offset][1]):
            print("Function name mismatch: %s (%s vs %s)" % (hex(offset), function_csv[offset][2], drive_list[offset][1]))

print("Collecting symbols...")

class Symbol:
    def __init__(self, name, start, end):
        self.name = name
        self.start = start
        self.end = end
        self.occurrences = []
    
    def add_occurrence(self, file):
        self.occurrences.append(file)

def read_global_symbols(file):
    symbols = {}
    with open(file, 'r') as file:
        for line in file.readlines():
            line = line.strip()
            if line.startswith(".global"):
                name = line.split(" ")[1]
                symbols[name] = Symbol(name, 0, 0)  # TODO fill in proper offsets
    return symbols

class Segment:
    def __init__(self, name):
        self.name = name
        self.file = "split/%s.s" % name
        self.symbols = read_global_symbols(self.file)

class Segments:
    def __init__(self, segments):
        self.segments = [Segment(segment) for segment in segments]
        self.text = Segment("text")
    
    def add_reference(self, name, reference):
        for segment in self.segments:
            if name in segment.symbols:
                segment.symbols[name].add_occurrence(reference)
                return
        if not name in self.text.symbols:
            print("Symbol %s not found in any segment" % name)

segments = Segments(["data", "rodata", "got"])

print("Resolving references to symbols...")

with open('split/text.s', 'r') as file:
    current_function = "NO FUNCTION"
    for line in file.readlines():
        line = line.strip()
        if ".global" in line:
            current_function = line.split(" ")[1]
        if ":lo12:" in line:
            parts = line.split(":lo12:")
            if len(parts) != 2:
                print(":lo12: does appear more than once in line: %s" % line)
            # off_7101E4EAF0
            off_name = parts[1][0:14]
            if not off_name.startswith("off_"):
                print("Invalid offset name: %s" % off_name)

            segments.add_reference(off_name, current_function)

def process_self_references(file):
    global segments

    with open(file, 'r') as file:
        current_symbol = "NO SYMBOL"
        for line in file.readlines():
            line = line.strip()
            if ".global" in line:
                current_symbol = line.split(" ")[1]
            if ".quad" in line:
                parts = line.split(" ")
                if len(parts) != 2:
                    print(".quad does appear more than once in line: %s" % line)
                off_name = parts[1]
                segments.add_reference(off_name, current_symbol)

for segment in segments.segments:
    process_self_references(segment.file)

for segment in segments.segments:
    unref_symbols = 0
    for symbol in segment.symbols:
        if len(segment.symbols[symbol].occurrences) == 0:
            print("No references for symbol: %s" % symbol)
            unref_symbols += 1
    print("Unreferenced symbols in %s: %d" % (segment.name, unref_symbols))

print("Generating .text list...")

# name: {.text: [offset: name], .data: [start, end], ...}
file_list = {}

unknown_file_counter = 0
last_file = "UNKNOWN"
for function in function_csv:
    if function not in drive_list:
        file = last_file
        unknown_file_counter += 1
    else:
        file = drive_list[function][0]
        last_file = file
    
    if not file in file_list:
        file_list[file] = {".text": []}
    
    file_list[file][".text"].append({function: function_csv[function][2]})

def generate_section(section, symbols):
    global file_list
    # contains file names in order of appearance
    files = list(file_list)
    # maps from file name to symbols that belong to this TU
    index_list = {file: [] for file in files}

    file_index = 0
    unreferenced = 0
    for symbol in symbols:
        occurrences = symbols[symbol]
        if len(occurrences) == 0:
            unreferenced += 1
            index_list[files[file_index]].append(symbol)
            continue
        while len([x for x in occurrences if x in file_list[files[file_index][".text"]]]) == 0:
            file_index += 1
            if file_index == len(files):
                print("Symbol %s not referenced in any file" % symbol)
                break
    
    for f in file_list:
        symbols_file = index_list[f]
        start = min([symbols[symbol].start for symbol in symbols_file])
        end = max([symbols[symbol].end for symbol in symbols_file])
        file_list[f][section] = [start, end]

print("Unknown files: %d" % unknown_file_counter)

print("Writing output files...")

import yaml

def hexint_presenter(dumper, data):
    return dumper.represent_int(hex(data))
yaml.add_representer(int, hexint_presenter)

with open('file_list_new.yaml', 'w') as file:
    yaml.dump(file_list, file)

with open('test_data.txt', 'w') as file:
    yaml.dump(segments, file)
