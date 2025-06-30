import os
import sys
from pathlib import Path

workdir = Path(sys.argv[1])

for root, _, files in os.walk(workdir):
    for file in files:
        file_path = os.path.join(root, file)
        # Only process .h and .hpp files
        if not file.endswith(".h") and not file.endswith(".hpp"):
            continue
        with open(file_path, "r+") as file:
            lines = list(file)
            i = 0
            while i < len(lines):
                if "private:" in lines[i]:
                     lines[i] = lines[i].replace("private:", "public:")
                elif "protected:" in lines[i]:
                    lines[i] = lines[i].replace("protected:", "public:")
                if "al/" not in file_path and "game/" not in file_path:
                    i += 1
                    continue
                if "inline " in lines[i] and "(" in lines[i] and not "= default" in lines[i]:
                    for j, line_inner in enumerate(lines[i:]):
                        line_inner_no_newline = line_inner.strip("\n")
                        if line_inner_no_newline.endswith("{") or line_inner_no_newline.endswith("}"):
                            break
                        if line_inner_no_newline.endswith(";"):
                            del lines[i:i+j+1]
                            break
                i += 1
            file.seek(0)
            file.writelines(lines)
            file.truncate()
