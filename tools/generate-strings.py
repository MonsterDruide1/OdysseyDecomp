#!/ usr / bin / env python3

import os
import re

from common import setup_common as setup

MIN_TEXT_SIZE = 0x2
MAX_TEXT_SIZE = 0x100
CHUNK_SIZE = 0x200 # Read file chunk size

NSO_OFFSET = 0x7100000000
START_OFFSET = 0x710181c3d8 # String table start main 1.0.0
END_OFFSET = 0x710187fb74 # String table end

def parse_string(buffer, offset, csv_file):
    if len (buffer) < MIN_TEXT_SIZE:
        return
    if len(buffer) > MAX_TEXT_SIZE:
        return

    nso_addr = NSO_OFFSET + offset - 0x100

    try :
        text = buffer.decode('utf-8')

        # Make escape sequences visible
        text = text.replace("\n", "\\n")
        text = text.replace("\r", "\\r")
        text = text.replace("\t", "\\t")

        # Write to csv file
        csv_file.write(hex(nso_addr))
        csv_file.write(",\"")
        csv_file.write(text)
        csv_file.write("\"\n")
    except:
        #Decode to utf - 8 can fail on non - string data
        print("Unable to parse string")
        print(buffer)

def parse_nso(csv_file, nso_file):
    offset = START_OFFSET - NSO_OFFSET + 0x100
    end = END_OFFSET - NSO_OFFSET + 0x100
    nso_file.seek(offset);
    buffer = bytes()
    
    while nso_file.tell() < end:
        chunk = nso_file.read(CHUNK_SIZE)
        if not chunk:
            return
        buffer += chunk 
        previous_zero = 0
        last_zero = -1
        
        for i, b in enumerate(buffer):
            if b != 0:
                continue
            if offset > end:
                return
            previous_zero = last_zero
            last_zero = i
            text = buffer[previous_zero + 1 : last_zero]
            parse_string(text, offset, csv_file)
            offset += last_zero - previous_zero
            
        buffer = buffer[last_zero + 1 : len(buffer)]

def create_string_table(string_path, nso_path):
    csv_file = open(string_path, "w")
    nso_file = open(nso_path, "rb")
    
    parse_nso(csv_file, nso_file)
    
    nso_file.close()
    csv_file.close()

project_root = setup.ROOT

def main():
    if not os.path.isfile(project_root / 'data' / "main.nso"):
        print("main.nso not found")
        return
    create_string_table(project_root / 'data'/ "data_strings.csv", project_root / 'data'/ "main.nso");

if __name__ == "__main__" :
    main()
