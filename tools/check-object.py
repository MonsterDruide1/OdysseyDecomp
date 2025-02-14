#!/usr/bin/env python3

import argparse
import os
from pathlib import Path
import urllib.request
import urllib.parse
import csv
import curses
import subprocess
from curses import wrapper
from io import StringIO
import re

# SMO Decomp Function Map: https://docs.google.com/spreadsheets/d/198vrkkDqktrRDLInSAkK2HsG5hy1Fl8cmCNRMND3nCY/edit?gid=4087080#gid=4087080
SHEET_ID = '198vrkkDqktrRDLInSAkK2HsG5hy1Fl8cmCNRMND3nCY'
SHEET_GID = 4087080

TSV_PATH = 'data/odyssey_mappings.tsv'
CSV_PATH = 'data/odyssey_functions.csv'

args = None

# Process spreadsheet

def download_sheets_data():
    url = f"https://docs.google.com/spreadsheets/d/{SHEET_ID}/export?format=tsv&gid={SHEET_GID}"
    print(f"Downloading function map from {url}")
    urllib.request.urlretrieve(url, TSV_PATH)

def read_odyssey_data(odyssey_functions_path):
    odyssey_data = {}
    with open(odyssey_functions_path, 'r') as odyssey_file:
        odyssey_csv = csv.DictReader(odyssey_file)
        for row in odyssey_csv:
            address = int(row['Address'], 16)
            quality = row['Quality']
            size = int(row['Size'], 16)
            mangled_name = row['Name']
            odyssey_data[address] = (address, quality, size, mangled_name)
    return odyssey_data

def extract_class_name(function_name):
    first_paren_index = function_name.find('(')
    truncated_name = function_name[:first_paren_index] if first_paren_index != -1 else function_name
    split_by_colons = truncated_name.split('::')
    
    if len(split_by_colons) > 2:
        return '::'.join(split_by_colons[:2]) + '::'
    elif len(split_by_colons) > 1:
        return '::'.join(split_by_colons[:1]) + '::'
    else:
        return ''

def find_mangled_name_in_odyssey_functions(start_address):
    with open(CSV_PATH, 'r', encoding='utf-8') as file:
        reader = csv.DictReader(file)
        for row in reader:
            if int(row['Address'], 16) == start_address:
                return row['Name']
    return None

def preprocess_tsv(spreadsheet_functions_path, odyssey_functions_path):
    odyssey_data = read_odyssey_data(odyssey_functions_path)
    
    processed_data = {}
    unorganized_data = []
    current_folder = None
    current_object = None

    with open(spreadsheet_functions_path, 'r') as spreadsheet_file:
        tsv = csv.DictReader(spreadsheet_file, delimiter='\t')
        for row in tsv:
            folder = row['Folder']
            obj = row['Object']
            address = int(row['Start Address'], 16)
            
            if folder:
                current_folder = folder
                current_object = None
            if obj:
                current_object = obj

            function_name = row['Demangled Name']
            if function_name == "None":
                function_name = row['Mangled Name']

            odyssey_address, odyssey_quality, odyssey_size, _ = odyssey_data[address]

            if row['Not in Obj'] == "TRUE":
                unorganized_data.append((function_name, address, odyssey_size, odyssey_quality))
            else:
                if current_folder not in processed_data:
                    processed_data[current_folder] = {}
                if current_object not in processed_data[current_folder]:
                    processed_data[current_folder][current_object] = []

                processed_data[current_folder][current_object].append((function_name, address, odyssey_size, odyssey_quality))
    
    return processed_data, unorganized_data

# Processed data buffer

def create_buffer(functions):
    buffer = []
    completed_functions = 0
    total_functions = len(functions)

    completed_size_bytes = 0
    object_size_bytes = 0

    for (function, address, size, quality) in functions:

        if quality == 'O':
            completed_functions += 1
            completed_size_bytes += size

        object_size_bytes += size

        color = 4  # White
        if quality == 'O':
            color = 2  # Green
        elif quality == 'U':
            color = 1  # Red
        elif quality in ['M', 'm']:
            color = 3  # Yellow

        formatted_string = f"{quality} {function}\n"
        buffer.append((formatted_string, color))

    return buffer, completed_functions, total_functions, completed_size_bytes, object_size_bytes

def create_folder_buffer(folders, folder):
    if folder not in folders:
        raise Exception(f"Folder {folder} not found.")

    buffer = []
    if not folder in folders:
        objects = {}
    else:
        objects = folders[folder]

    total_size = 0
    total_completed_size = 0
    total_functions = 0
    total_completed_functions = 0

    for obj in objects:
        completed_size = 0
        completed_functions = 0
        object_functions = 0
        for function_name, address, size, quality in folders[folder][obj]:
            if quality == 'O':
                completed_size += size
                total_completed_size += size
                completed_functions += 1
                total_completed_functions += 1
            total_size += size
            total_functions += 1
            object_functions += 1
        if object_functions == 0:
            progress_percentage = 0
        else:
            progress_percentage = completed_functions / object_functions * 100
        progress_string = f"{completed_functions}/{object_functions}".ljust(7)
        percentage_string = f"{progress_percentage:.3f}%".rjust(9)

        if progress_percentage == 100:
            color = 2  # Green
        elif progress_percentage > 0:
            color = 3  # Yellow
        else:
            color = 1  # Red

        buffer.append((f"{progress_string} | {percentage_string} | {folder}/{obj.ljust(30)}", color))
    return buffer, total_completed_size, total_size, total_completed_functions, total_functions

def create_mismatch_buffer(folders):
    buffer = []

    longest_object_length = max(len(obj) for objects in folders.values() for obj in objects.keys())

    for folder, objects in folders.items():
        for obj, functions in objects.items():
            for function_name, start_address, size, quality in functions:
                if quality in ['M', 'm']:
                    object_name = f'{folder}/{obj}'

                    if quality == 'm':
                        color = 2  # Green
                    else:
                        color = 3  # Yellow

                    buffer.append((
                        f"{object_name.ljust(longest_object_length)} | {quality} {hex(start_address).ljust(8)} {function_name.ljust(30)}",
                        color
                    ))
    return buffer

def scroll_down(current_row, first_visible_row, max_display_rows, buffer):
    if current_row < len(buffer) - 1:
        current_row += 1
        if current_row >= first_visible_row + max_display_rows:
            first_visible_row += 1
    return current_row, first_visible_row

def scroll_up(current_row, first_visible_row):
    if current_row > 0:
        current_row -= 1
        if current_row < first_visible_row:
            first_visible_row -= 1
    return current_row, first_visible_row

def search_items(buffer, search_string):
    filtered_buffer = []
    for item in buffer:
        if search_string.lower() in item[0].lower():
            filtered_buffer.append(item)
    return filtered_buffer

def handle_key_press(key, current_row, first_visible_row, max_display_rows, buffer, search_string):
    if key == curses.KEY_DOWN:
        current_row, first_visible_row = scroll_down(current_row, first_visible_row, max_display_rows, buffer)
    elif key == curses.KEY_UP:
        current_row, first_visible_row = scroll_up(current_row, first_visible_row)
    elif key == curses.KEY_BACKSPACE:
        search_string = search_string[:-1]
    elif key >= 32 and key <= 126 and not key == ord("/"):
        search_string += chr(key)

    filtered_buffer = search_items(buffer, search_string)
    current_row = min(current_row, len(filtered_buffer) - 1)
    first_visible_row = max(0, min(first_visible_row, len(filtered_buffer) - max_display_rows))

    return current_row, first_visible_row, filtered_buffer, search_string

def set_cursor_position(window, max_display_rows, search_string):
    if search_string:
        window.move(max_display_rows + 1, len(search_string))
        curses.curs_set(2)
    else:
        curses.curs_set(0)

def display_items(window, max_display_rows, first_visible_row, filtered_buffer, current_row):
    for i in range(first_visible_row, min(first_visible_row + max_display_rows, len(filtered_buffer))):
        string, color = filtered_buffer[i]
        if i == current_row:
            window.addstr(i - first_visible_row, 0, string, curses.color_pair(color) | curses.A_REVERSE)
        else:
            window.addstr(i - first_visible_row, 0, string, curses.color_pair(color))

# Main functionality

def run_tools_check(name):
    command = f"tools/check {name}"
    subprocess.run(command, shell=True, check=False, cwd=os.getcwd())

def format_function_name(name):
    formatted_name = re.sub(r"([:\w]+) const([\*&])", r"const \1\2", name);
    return formatted_name

def create_header_file(folder, object_name, functions):
    if folder.startswith('Library') or folder.startswith('Project'):
        path = os.path.join('lib/al', folder, f'{object_name}.h')
    else:
        path = os.path.join('src', folder, f'{object_name}.h')

    directory = os.path.dirname(path)
    if not os.path.exists(directory):
        os.makedirs(directory)

    if not os.path.exists(path):
        with open(path, 'w') as header_file:
            header_file.write("#pragma once\n\n")

            header_file.write("/*\n")
            for function_name, _, _, _ in functions:
                formatted_name = format_function_name(function_name)
                header_file.write("{}\n".format(formatted_name))
            header_file.write("*/\n")
        print(f"Created {os.path.abspath(path)}")
    else:
        print("Header file already exists!")

def main(stdscr):
    args = parse_arguments()
    stdscr.erase()
    stdscr.keypad(True)
    setup_curses()

    window = curses.newwin(curses.LINES-1, curses.COLS-1, 0, 0)
    window.keypad(True)
    window.timeout(100)

    tsv_data = load_tsv_data(args)
    folders, unorganized_functions = preprocess_tsv(TSV_PATH, CSV_PATH)

    if args.mismatch_view:
        mismatch_view(stdscr, window, folders)
    elif args.folder_view:
        if args.folder_object:
            folder_object = folder_view(stdscr, window, folders, [args.folder_object])
        else:
            all_folders = [folder for folder in folders]
            folder_object = folder_view(stdscr, window, folders, all_folders)
        if folder_object is not None:
            function_view(stdscr, window, folders, folder_object, unorganized_functions)
    else:
        function_view(stdscr, window, folders, args.folder_object, unorganized_functions)

def function_view(stdscr, window, folders, folder_object, unorganized_functions):
    if not folder_object.endswith('.o'):
        folder_object += '.o'

    folder_object_parts = folder_object.split('/')
    if len(folder_object_parts) > 1:
        folder = '/'.join(folder_object_parts[:-1])
        object_name = folder_object_parts[-1]
    else:
        folder = ''
        object_name = folder_object_parts[0]

    if not folder:
        folder = next((f for f, objs in folders.items() if object_name in objs), '')

    if not folder or object_name not in folders[folder]:
        raise Exception(f"Object {object_name} not found in any folder.")

    functions = folders[folder][object_name]
    if not functions:
        raise Exception(f"No functions found for {object_name} in folder {folder}.")

    #raise Exception(f"{unorganized_functions}")
    for function_name, address, size, quality in unorganized_functions:
        if f"{object_name[:-2]}::" in function_name:
            functions.append((function_name, address, size, quality))

    buffer, completed_functions, total_functions, completed_size_bytes, object_size_bytes = create_buffer(functions)
    current_row = 0
    first_visible_row = 0
    search_string = ""
    filtered_buffer = buffer[:]  # Initially, display all functions

    is_slash_held = False

    while True:
        window.erase()

        max_display_rows, _ = window.getmaxyx()
        max_display_rows -= 1

        if search_string:
            max_display_rows -= 1
            window.addstr(max_display_rows + 1, 0, search_string)

        display_items(window, max_display_rows, first_visible_row, filtered_buffer, current_row)

        if not is_slash_held:
            window.addstr(max_display_rows, 0, f"{object_name} | Matched {completed_functions}/{total_functions} ({round(completed_functions / total_functions * 100, 3)}%) | {round(completed_size_bytes / 1024, 3)} KB / {round(object_size_bytes / 1024, 3)} KB")
        else:
            window.addstr(max_display_rows, 0, "Press / + Enter to create header file...")
        
        key = window.getch()

        if is_slash_held and key == ord('\n'):
            curses.endwin()
            create_header_file(folder, object_name[:-2], functions)
            return

        if key == ord("/"):
            is_slash_held = True
            window.nodelay(True)
        else:
            is_slash_held = False
            if key == ord('\n') or key == curses.KEY_RIGHT:
                curses.endwin()
                run_tools_check(find_mangled_name_in_odyssey_functions(functions[current_row][1]))
                return
        
        window.nodelay(False)
        
        window.refresh()
        set_cursor_position(window, max_display_rows, search_string)

        current_row, first_visible_row, filtered_buffer, search_string = handle_key_press(key, current_row, first_visible_row, max_display_rows, buffer, search_string)

def folder_view(stdscr, window, folders, folder_list):
    all_buffers = []
    total_completed_size = 0
    total_size = 0
    total_completed_functions = 0
    total_functions = 0

    for folder in folder_list:
        buffer, folder_completed_size, folder_total_size, folder_total_completed_functions, folder_total_functions = create_folder_buffer(folders, folder)
        all_buffers.extend(buffer)
        total_completed_size += folder_completed_size
        total_size += folder_total_size
        total_completed_functions += folder_total_completed_functions
        total_functions += folder_total_functions

    current_row = 0
    first_visible_row = 0
    search_string = ""
    filtered_buffers = all_buffers[:]  # Initially, display all buffers

    while True:
        window.erase()

        max_display_rows, _ = window.getmaxyx()
        max_display_rows -= 1

        if search_string:
            max_display_rows -= 1
            window.addstr(max_display_rows + 1, 0, search_string)

        for i in range(first_visible_row, min(first_visible_row + max_display_rows, len(filtered_buffers))):
            string, color = filtered_buffers[i]
            if i == current_row:
                window.addstr(i - first_visible_row, 0, string, curses.color_pair(color) | curses.A_REVERSE)
            else:
                window.addstr(i - first_visible_row, 0, string, curses.color_pair(color))
        
        folder_name = ""
        if len(folder_list) == 1:
            folder_name = f"{folder_list[0]} | "
        window.addstr(max_display_rows, 0, f"{folder_name}{len(filtered_buffers)} items | {total_completed_functions}/{total_functions} functions ({total_completed_functions / total_functions * 100:.3f}%) | {total_completed_size / 1024:.3f} KB / {total_size / 1024:.3f} KB")

        window.refresh()
        set_cursor_position(window, max_display_rows, search_string)

        key = window.getch()

        if key == ord('\n') or key == curses.KEY_RIGHT:
            path = filtered_buffers[current_row][0].split('|')[-1].strip()
            return path

        result = handle_key_press(key, current_row, first_visible_row, max_display_rows, all_buffers, search_string)
        if result is None:  # Exit condition
            curses.endwin()
            return
        else:
            current_row, first_visible_row, filtered_buffers, search_string = result

def mismatch_view(stdscr, window, folders):
    buffer = create_mismatch_buffer(folders)
    current_row = 0
    first_visible_row = 0
    search_string = ""
    filtered_buffer = buffer[:]  # Initially, display all items

    while True:
        window.erase()

        max_display_rows, _ = window.getmaxyx()
        max_display_rows -= 1

        if search_string:
            max_display_rows -= 1
            window.addstr(max_display_rows + 1, 0, search_string)

        for i in range(first_visible_row, min(first_visible_row + max_display_rows, len(filtered_buffer))):
            string, color = filtered_buffer[i]
            if i == current_row:
                window.addstr(i - first_visible_row, 0, string, curses.color_pair(color) | curses.A_REVERSE)
            else:
                window.addstr(i - first_visible_row, 0, string, curses.color_pair(color))

        window.addstr(max_display_rows, 0, f"Mismatched Functions | {len(filtered_buffer)} items")

        window.refresh()
        set_cursor_position(window, max_display_rows, search_string)

        key = window.getch()
        if key == ord('\n') or key == curses.KEY_RIGHT:
            function_address = int(filtered_buffer[current_row][0].split('|')[-1].split(' ')[2], 16)

            curses.endwin()
            run_tools_check(find_mangled_name_in_odyssey_functions(function_address))
            return
        else:
            result = handle_key_press(key, current_row, first_visible_row, max_display_rows, buffer, search_string)
            if result is not None:
                current_row, first_visible_row, filtered_buffer, search_string = result

# Setup

def parse_arguments():
    parser = argparse.ArgumentParser(description='Check individual object files in the decomp')
    parser.add_argument('folder_object', nargs='?', default=None, type=str, help='Folder/Object to search for')
    parser.add_argument('-r', '--refresh', action='store_true', help='Redownload the TSV file')
    parser.add_argument('-f', '--folder_view', action='store_true', help='View folders instead of functions')
    parser.add_argument('-m', '--mismatch_view', action='store_true', help='View mismatched functions')

    args = parser.parse_args()

    if args.folder_object is None and not args.folder_view and not args.mismatch_view:
        parser.error("The following arguments are required: folder_object")

    return args

def setup_curses():
    curses.start_color()
    curses.use_default_colors()
    curses.init_pair(1, curses.COLOR_RED, -1)    # -1 indicates the default background color
    curses.init_pair(2, curses.COLOR_GREEN, -1)
    curses.init_pair(3, curses.COLOR_YELLOW, -1)
    curses.init_pair(4, curses.COLOR_WHITE, -1)
    curses.curs_set(0)

def load_tsv_data(args):
    if args.refresh or not os.path.exists(TSV_PATH):
        download_sheets_data()

    if os.path.exists(TSV_PATH):
        with open(TSV_PATH, 'r', encoding='utf-8') as file:
            tsv_data = file.read()
    else:
        raise Exception(f"Cannot read {TSV_PATH}")
    return tsv_data

if __name__ == "__main__":
    args = parse_arguments()
    curses.wrapper(main)

print("\n")
