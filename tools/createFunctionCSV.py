import idaapi
import idautils

with open("/tmp/funclist.csv", "w") as f:
    for ea in idautils.Functions():
        size = idaapi.get_func(ea).size()
        name = idaapi.get_func_name(ea)
        f.write(",".join(("0x%016lx" % ea, name, str(size), "")) + "\n")
