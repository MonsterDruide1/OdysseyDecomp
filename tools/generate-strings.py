#!/usr/bin/env python3

import os
import re

from common import setup_common as setup

MIN_TEXT_SIZE = 0x2
MAX_TEXT_SIZE = 0x600
CHUNK_SIZE = 0x200 # Read file chunk size

NSO_OFFSET = 0x70FFFFFF00
U8_START_OFFSET = 0x710181c3d8 # UTF8 String table start main 1.0.0
U8_END_OFFSET = 0x710187fb74 # UTF8 String table end
U16_START_OFFSET = 0x71018acfbc # UTF16 string table start main 1.0.0
U16_END_OFFSET = 0x71018ad1b3 # UTF16 string table end

def parse_string(buffer, offset, csv_file, encoding):
    nso_addr = NSO_OFFSET + offset
    
    if len (buffer) < MIN_TEXT_SIZE:
        return
    if len(buffer) > MAX_TEXT_SIZE:
        print(hex(nso_addr), "Warning: String is bigger than buffer size")
        return

    try :
        text = buffer.decode(encoding)

        # Make escape sequences visible
        text = text.replace("\\", "\\\\")
        text = text.replace("\n", "\\n")
        text = text.replace("\r", "\\r")
        text = text.replace("\t", "\\t")
        text = text.replace("\"", "\\\"")

        # Write to csv file
        csv_file.write(hex(nso_addr))
        if encoding == 'utf-16':
            csv_file.write(",u\"")
        else:
            csv_file.write(",\"")
        csv_file.write(text)
        csv_file.write("\"\n")
    except:
        # Decode can fail on non-string data
        print(hex(nso_addr), "Unable to parse string")

def parse_utf8(csv_file, nso_file):
    offset = U8_START_OFFSET - NSO_OFFSET
    end = U8_END_OFFSET - NSO_OFFSET
    nso_file.seek(offset)
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
            parse_string(text, offset, csv_file, 'utf-8')
            offset += last_zero - previous_zero

        buffer = buffer[last_zero + 1 : len(buffer)]

def parse_utf16(csv_file, nso_file):
    offset = U16_START_OFFSET - NSO_OFFSET
    end = U16_END_OFFSET - NSO_OFFSET
    nso_file.seek(offset)
    buffer = bytes()

    while nso_file.tell() < end:
        chunk = nso_file.read(CHUNK_SIZE)
        if not chunk:
            return
        buffer += chunk
        previous_zero = 0
        last_zero = -1
        prev = 0

        for i, b in enumerate(buffer):
            if i % 2 == 0:
                prev = b
                continue
            if prev != 0 or b != 0:
                continue
            if offset > end:
                return
            previous_zero = last_zero
            last_zero = i
            text = buffer[previous_zero + 1 : last_zero - 1]
            parse_string(text, offset, csv_file, 'utf-16')
            offset += last_zero - previous_zero

        buffer = buffer[last_zero + 1 : len(buffer)]

def create_string_table(string_path, nso_path):
    with open(string_path, "w") as csv_file, open(nso_path, "rb") as nso_file:
        parse_utf8(csv_file, nso_file)
        parse_utf16(csv_file, nso_file)

project_root = setup.ROOT

def main():
    if not os.path.isfile(project_root / 'data' / "main.nso"):
        print("main.nso not found!")
        return
    create_string_table(project_root / 'data'/ "data_strings.csv", project_root / 'data'/ "main.nso")

if __name__ == "__main__" :
    main()
