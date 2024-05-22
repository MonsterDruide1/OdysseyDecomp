new_file = open('odyssey_functions_new.csv', 'r')
new_lines = new_file.readlines()[1:]
new_file.close()

old_file = open('odyssey_functions_old.csv', 'r')
old_lines = old_file.readlines()[1:]
old_file.close()

result = open('odyssey_functions.csv', 'w')

while len(old_lines) != 0 and len(new_lines) != 0:
    # Address,Quality,Size,Name
    [address, quality, size, name] = new_lines[0].split(',')
    [old_address, old_quality, old_size, old_name] = old_lines[0].split(',')

    if address == old_address and size == old_size:
        result.write(old_address + ',' + old_quality + ',' + old_size + ',' + name)
    else:
        result.write(address + ',' + quality + ',' + size + ',' + name)

    if size == old_size:
        new_lines.pop(0)
        old_lines.pop(0)
    elif size < old_size:
        new_lines.pop(0)
    else:
        old_lines.pop(0)
