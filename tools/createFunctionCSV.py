import idaapi
import idautils
import re

sub = re.compile("^sub_71[0-9A-F]{8}$")
nullsub = re.compile("^nullsub_\d+$")

# Address,Quality,Size,Name
# 0x0000007100000024,U,000316,_init
# ...

with open("/tmp/funclist.csv", "w") as f:
    f.write("Address,Quality,Size,Name\n")
    for ea in idautils.Functions():
        size = idaapi.get_func(ea).size()
        name = idaapi.get_func_name(ea)
        if sub.match(name) or nullsub.match(name):
            name = ""
        f.write(",".join(("0x%016lx" % ea, "U", "{:06d}".format(size), name)) + "\n")
