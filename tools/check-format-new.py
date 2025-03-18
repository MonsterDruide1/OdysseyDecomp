import clang.cindex as libclang
from pathlib import Path
from functools import cache

ROOT = Path(__file__).parent.parent

@cache
def get_common_compiler_args():
    cd = libclang.CompilationDatabase.fromDirectory(ROOT/'build')
    arguments = {x.filename: list(x.arguments) for x in cd.getAllCompileCommands() if str(ROOT/'src') in x.filename or str(ROOT/'lib/al') in x.filename}

    for (file,args) in arguments.items():
        # remove "-o" and one following argument from all entries
        for i in range(len(args)):
            if args[i] == '-o':
                args.pop(i)
                args.pop(i)
                break
        # remove last argument = filename
        args.pop()


    common_args = next(iter(arguments.values()))

    for (file,args) in arguments.items():
        common_args = [arg for arg in common_args if arg in args]

    for (file,args) in arguments.items():
        args = [arg for arg in args if arg not in common_args]
        if len(args) > 0:
            print("ERROR: Found additional arguments for", file, ", arguments:", args)
    
    return common_args

index = libclang.Index.create()
tu = index.parse(str(ROOT/'src'/'Util'/'ObjUtil.h'), ["--driver-mode=g++"])#, get_common_compiler_args())

cursor = tu.cursor
def debug_node(node):
    print(node.kind, node.spelling, node.location)
    for child in node.get_children():
        debug_node(child)

debug_node(cursor)
print(get_common_compiler_args())
