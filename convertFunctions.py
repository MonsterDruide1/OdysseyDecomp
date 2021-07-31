import csv

prefixes = ("sub_")
good_prefixes = ("eco::", "ui::")

last_addr = 0
with open("data/ukf.csv", "r") as csvfile, open("data/uking_functions.csv", "w") as f:
    w = csv.writer(f)
    w.writerow(["Address", "Quality", "Size", "Name"])
    for row in csv.reader(csvfile):
        addr = row[0].strip()
        ida_name = row[1].strip()
        if not ida_name.startswith(good_prefixes) and "sub" in ida_name.lower() and "nullsub" not in ida_name.lower() and ida_name.lower().endswith(addr[-10:].lower()):
            if not ida_name.lower().startswith("sub"):
                print(ida_name)
                assert False
            ida_name = ""
        name = ida_name
        size = "{:06d}".format(int(row[2].strip(), 0))
        assert len(size) == 6
        code_name = row[3].strip()
        quality = "U"
        #if "nullsub" in name:
        #    print(name, size)

        addr_val = int(row[0], 16)
        assert addr_val > last_addr
        last_addr = addr_val
        addr = "0x{0:016x}".format(addr_val)
        assert len(addr) == 18

        assert not ida_name or (ida_name[-1] != "?" and ida_name[-1] != "!")

        assert int(size) > 1

        if code_name and code_name != "l":
            assert len(code_name) > 1
            if code_name[-1] == "?":
                quality = "m"
                name = code_name[:-1]
            elif code_name[-1] == "!":
                quality = "M"
                name = code_name[:-1]
            else:
                quality = "O"
                name = code_name
        elif code_name == "l":
            quality = "L"

        if set(name) == {"x"}:
            print(row)
            name = ""

        #assert name
        w.writerow([addr, quality, size, name])
