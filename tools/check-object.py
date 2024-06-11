import argparse
import requests
import csv
import curses
import os
import subprocess
import sys
from curses import wrapper
from io import StringIO

# SMO Decomp Function Map: https://docs.google.com/spreadsheets/d/198vrkkDqktrRDLInSAkK2HsG5hy1Fl8cmCNRMND3nCY/edit?gid=4087080#gid=4087080
SHEET_ID = '198vrkkDqktrRDLInSAkK2HsG5hy1Fl8cmCNRMND3nCY'
SHEET_GID = 4087080

TSV_PATH = 'data/odyssey_mappings.tsv'
CSV_PATH = 'data/odyssey_functions.csv'

def download_sheets_data(sheet_id, sheet_gid):
    url = f"https://docs.google.com/spreadsheets/d/{sheet_id}/export?format=tsv&gid={sheet_gid}"
    print(f"Downloading function map from {url}")
    response = requests.get(url)
    if response.status_code == 200:
        return response.text
    else:
        raise Exception(f"Failed to download Google Sheet: {response.status_code}")

def preprocess_tsv(spreadsheet_functions_path, odyssey_functions_path):
    odyssey_data = {}
    with open(odyssey_functions_path, 'r') as odyssey_file:
        odyssey_csv = csv.DictReader(odyssey_file)
        for row in odyssey_csv:
            address = int(row['Address'], 16)
            quality = row['Quality']
            size = int(row['Size'], 16)
            mangled_name = row['Name']
            odyssey_data[address] = (address, quality, size, mangled_name)
    
    processed_data = {}
    old_folder = None
    old_object = None
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
            
            unorganized = False
            if row['Not in Obj'] == "TRUE":
                unorganized = True
                old_folder = current_folder
                old_object = current_object
                current_folder = "Unorganized"
                current_object = "Unorganized"
            
            odyssey_address, odyssey_quality, odyssey_size, odyssey_mangled_name = odyssey_data[address]
            
            if current_folder and current_object:
                if current_folder not in processed_data:
                    processed_data[current_folder] = {}
                if current_object not in processed_data[current_folder]:
                    processed_data[current_folder][current_object] = []
                
                processed_data[current_folder][current_object].append((function_name, address, odyssey_size, odyssey_quality))
            
            if unorganized:
                current_folder = old_folder
                current_object = old_object
    
    return processed_data

def find_mangled_name_in_odyssey_functions(start_address):
    with open(CSV_PATH, 'r', encoding='utf-8') as file:
        reader = csv.DictReader(file)
        for row in reader:
            if int(row['Address'], 16) == start_address:
                return row['Name']
    return None

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

        color = curses.COLOR_WHITE
        if quality == 'O':
            color = curses.COLOR_GREEN
        elif quality == 'U':
            color = curses.COLOR_RED
        elif quality == 'M':
            color = curses.COLOR_YELLOW
        elif quality == 'm':
            color = curses.COLOR_YELLOW

        formatted_string = f"{quality} {function}\n"
        buffer.append((formatted_string, color))

    return buffer, completed_functions, total_functions, completed_size_bytes, object_size_bytes

def run_tools_check(functions, current_row):
    if current_row < len(functions):
        start_address = functions[current_row][1]
        command = f"tools/check {find_mangled_name_in_odyssey_functions(start_address)}"
        try:
            subprocess.run(command, shell=True, check=True, cwd=os.getcwd())
        except subprocess.CalledProcessError as e:
            print(f"\nError running tools/check: {e}")
    else:
        print("Invalid row index")

def parse_arguments():
    parser = argparse.ArgumentParser(description='Fetch functions from a Google Sheets document')
    parser.add_argument('folder_object', nargs='?', default='', type=str, help='Folder/Object to search for')
    parser.add_argument('-r', '--refresh', action='store_true', help='Redownload the TSV file')
    parser.add_argument('-f', '--folder_view', action='store_true', help='View folders instead of functions')
    return parser.parse_args()

def setup_curses():
    curses.start_color()
    curses.use_default_colors()
    curses.init_pair(curses.COLOR_RED, curses.COLOR_RED, curses.COLOR_BLACK)
    curses.init_pair(curses.COLOR_GREEN, curses.COLOR_GREEN, curses.COLOR_BLACK)
    curses.init_pair(curses.COLOR_YELLOW, curses.COLOR_YELLOW, curses.COLOR_BLACK)
    curses.init_pair(curses.COLOR_WHITE, curses.COLOR_WHITE, curses.COLOR_BLACK)

def load_tsv_data(args):
    if args.refresh or not os.path.exists(TSV_PATH):
        tsv_data = download_sheets_data(SHEET_ID, SHEET_GID)
        with open(TSV_PATH, 'w', encoding='utf-8') as file:
            file.write(tsv_data)
    else:
        if os.path.exists(TSV_PATH):
            with open(TSV_PATH, 'r', encoding='utf-8') as file:
                tsv_data = file.read()
        else:
            raise Exception(f"Cannot read {TSV_PATH}")
    return tsv_data

def main(stdscr):
    args = parse_arguments()
    stdscr.clear()
    setup_curses()

    window = curses.newwin(curses.LINES-1, curses.COLS-1, 0, 0)
    window.keypad(True)
    window.timeout(100)

    tsv_data = load_tsv_data(args)
    folders = preprocess_tsv(TSV_PATH, CSV_PATH)

    if args.folder_view:
        if args.folder_object:
            selected_folder, selected_object = folder_view(stdscr, window, folders, [args.folder_object])
        else:
            all_folders = [folder for folder in folders]
            selected_folder, selected_object = folder_view(stdscr, window, folders, all_folders)
        if selected_folder is not None and selected_object is not None:
            function_view(stdscr, window, folders, f"{selected_folder[0]}/{selected_object}")
    else:
        function_view(stdscr, window, folders, args.folder_object)



def function_view(stdscr, window, folders, folder_object):
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
    
    unorganized_functions = folders["Unorganized"]["Unorganized"]
    for function_name, address, size, quality in unorganized_functions:
        if f"{object_name[:-2]}::" in function_name:
            functions.append((function_name, address, size, quality))

    buffer, completed_functions, total_functions, completed_size_bytes, object_size_bytes = create_buffer(functions)
    current_row = 0

    while True:
        window.clear()

        for i in range(current_row, len(buffer)):
            string, color = buffer[i]
            max_display_rows, max_display_cols = window.getmaxyx()
            max_display_rows -= 1
            max_display_cols -= 1

            if i - current_row >= max_display_rows:
                break
            
            if i - current_row == stdscr.getyx()[0]:
                window.addstr(i - current_row, 0, string, curses.color_pair(color) | curses.A_REVERSE)
            else:
                window.addstr(i - current_row, 0, string, curses.color_pair(color))
        
        max_display_rows, _ = window.getmaxyx()
        window.addstr(max_display_rows - 1, 0, f"{object_name} | Matched {completed_functions}/{total_functions} ({round(completed_functions / total_functions * 100, 3)}%) | {round(completed_size_bytes / 1024, 3)} KB / {round(object_size_bytes / 1024, 3)} KB")
        
        window.refresh()
        window.move(0, 0)

        key = window.getch()
        if key == curses.KEY_DOWN:
            current_row = min(current_row + 1, len(functions) - 1)
        elif key == curses.KEY_UP:
            current_row = max(current_row - 1, 0)
        elif key == ord('\n'):
            curses.endwin()
            run_tools_check(functions, current_row)
            return folder, object_name
        elif key == ord('q'):
            return None, None

    curses.endwin()

def create_folder_buffer(folders, folder):
    if folder not in folders:
        raise Exception(f"Folder {folder} not found.")

    buffer = []
    if not folder in folders:
        objects = {}
    else:
        objects = folders[folder]

    for subfolder in folders:
        if subfolder.startswith(f"{folder}/"):
            subfolder_name = subfolder[len(folder) + 1:]
            for obj in folders[subfolder]:
                prefixed_obj = f"{subfolder_name}/{obj}"
                objects[prefixed_obj] = folders[subfolder][obj]

    if objects:
        first_key = next(iter(objects))
        objects[first_key] = objects.pop(first_key)

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
        progress_percentage = completed_functions / object_functions * 100
        progress_string = f"{completed_functions}/{object_functions}".ljust(7)
        percentage_string = f"{progress_percentage:.3f}%".rjust(9)

        if progress_percentage == 100:
            color = curses.COLOR_GREEN
        elif progress_percentage > 0:
            color = curses.COLOR_YELLOW
        else:
            color = curses.COLOR_RED

        buffer.append((f"{progress_string} | {percentage_string} | {obj.ljust(30)}", color))
    return buffer, total_completed_size, total_size




def folder_view(stdscr, window, folders, folder_list):
    all_buffers = []
    total_completed_size = 0
    total_size = 0
    
    for folder in folder_list:
        buffer, folder_completed_size, folder_total_size = create_folder_buffer(folders, folder)
        all_buffers.extend(buffer)
        total_completed_size += folder_completed_size
        total_size += folder_total_size

    current_row = 0

    while True:
        window.clear()

        for i in range(current_row, len(all_buffers)):
            string, color = all_buffers[i]
            max_display_rows, max_display_cols = window.getmaxyx()
            max_display_rows -= 1
            max_display_cols -= 1

            if i - current_row >= max_display_rows:
                break
            
            if i - current_row == stdscr.getyx()[0]:
                window.addstr(i - current_row, 0, string, curses.color_pair(color) | curses.A_REVERSE)
            else:
                window.addstr(i - current_row, 0, string, curses.color_pair(color))
        
        max_display_rows, _ = window.getmaxyx()
        window.addstr(max_display_rows - 1, 0, f"Selected Folders | {len(all_buffers)} items | {total_completed_size / 1024:.3f} KB / {total_size / 1024:.3f} KB")
        
        window.refresh()
        window.move(0, 0)

        key = window.getch()
        if key == curses.KEY_DOWN:
            current_row = min(current_row + 1, len(all_buffers) - 1)
        elif key == curses.KEY_UP:
            current_row = max(current_row - 1, 0)
        elif key == ord('\n'):  # Enter key pressed
            selected_item = all_buffers[current_row]
            selected_object = selected_item[0].split('|')[2].strip()
            curses.endwin()
            return folder_list, selected_object
        elif key == ord('q'):
            curses.endwin()
            return None, None  # Return None for both folder and object if user quits

    curses.endwin()


if __name__ == "__main__":
    curses.wrapper(main)

print("\n")
