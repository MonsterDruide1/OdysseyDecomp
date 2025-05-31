import os
import sys
from pathlib import Path

workdir = Path(sys.argv[1])

for root, _, files in os.walk(workdir):
    for file in files:
        file_path = os.path.join(root, file)
        # Only process .h and .hpp files
        if ".h" not in str(file):
            continue
        with open(file_path, "r+") as file:
            lines = list(file)
            for i, line in enumerate(lines):
                if "private:" in line:
                     lines[i] = line.replace("private:", "public:")
                elif "protected:" in line:
                    lines[i] = line.replace("protected:", "public:")
                if "al/" not in file_path and "game/" not in file_path:
                    continue
                if "inline " in line and "(" in line and not "= default" in line:
                    for j, line_inner in enumerate(lines[i:]):
                        line_inner_no_newline = line_inner.strip("\n")
                        if line_inner_no_newline.endswith("{") or line_inner_no_newline.endswith("}"):
                            break
                        if line_inner_no_newline.endswith(";"):
                            del lines[i:i+j+1]
                            break;
            file.seek(0)
            file.truncate(0)
            file.writelines(lines)
