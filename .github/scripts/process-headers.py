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
                        if line_inner_no_newline.endswith("\\"):  # part of a macro definition => cut out relevant parts of the line to decide whether to delete it
                            line_inner_no_newline = line_inner_no_newline[:-1].strip()
                        if line_inner_no_newline.strip() == "":
                            # should not happen if everything is correct, but there's still broken stuff for macro definitions
                            # in this case: https://github.com/MonsterDruide1/OdysseyDecomp/blob/master/lib/al/Library/HitSensor/SensorMsgSetupUtil.h#L87
                            # (ending a macro definition with a function definition with no body)
                            break
                        if line_inner_no_newline == "inline SensorMsg##Type(PARAM_LIST CtorParams);":
                            # despite all special case handlings above, this macro line is still special
                            # it is only declared in the class itself, so should according to all current rules be removed,
                            # but is also defined immediately afterwards in the same macro, which means that it is
                            # still inlined in all usages and should therefore be included in the headers repo
                            break
                        if line_inner_no_newline.endswith("{") or line_inner_no_newline.endswith("}"):
                            break
                        if line_inner_no_newline.endswith(";"):
                            del lines[i:i+j+1]
                            i -= 1
                            break
                i += 1
            file.seek(0)
            file.writelines(lines)
            file.truncate()
