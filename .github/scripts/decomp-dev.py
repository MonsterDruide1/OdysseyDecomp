#!/usr/bin/env python3
import argparse
import yaml
from functools import reduce
import cxxfilt

import sys, os
sys.path.append(os.path.join(os.path.dirname(__file__), "..", "..", "tools"))
sys.path.append(os.path.join(os.path.dirname(__file__), "..", "files", "decomp-dev"))

from report_pb2 import Report, Measures, ReportUnit, ReportUnitMetadata, ReportItem, ReportItemMetadata

data = yaml.load(open("data/file_list.yml"), Loader=yaml.CLoader)

def get_measures(unit_name=None):
    funcs = data[unit_name][".text"] if unit_name else reduce(lambda a,b: a+b, [unit[".text"] for unit in data.values()])

    total_units = len(data)
    total_code = sum([fun["size"] for fun in funcs])
    total_functions = len(funcs)

    matched_code = sum([fun["size"] for fun in funcs if fun["status"] == "Matching"])
    matched_functions = len([fun for fun in funcs if fun["status"] == "Matching"])
    matched_code_percent = matched_code / total_code * 100 if total_code > 0 else 0.0
    matched_functions_percent = matched_functions / total_functions * 100 if total_functions > 0 else 0.0

    fuzzy_code = sum([fun["size"] for fun in funcs if fun["status"] != "NotDecompiled"])
    fuzzy_match_percent = fuzzy_code / total_code * 100 if total_code > 0 else 0.0

    # TODO: we don't handle data or linking yet, so 0 for all of those
    total_data = 0
    matched_data = 0
    matched_data_percent = 0.0
    complete_code = 0
    complete_code_percent = 0.0
    complete_data = 0
    complete_data_percent = 0.0
    complete_units = 0
    
    measures = Measures()
    measures.fuzzy_match_percent = fuzzy_match_percent
    measures.total_code = total_code
    measures.matched_code = matched_code
    measures.matched_code_percent = matched_code_percent
    measures.total_data = total_data
    measures.matched_data = matched_data
    measures.matched_data_percent = matched_data_percent
    measures.total_functions = total_functions
    measures.matched_functions = matched_functions
    measures.matched_functions_percent = matched_functions_percent
    measures.complete_code = complete_code
    measures.complete_code_percent = complete_code_percent
    measures.complete_data = complete_data
    measures.complete_data_percent = complete_data_percent
    measures.total_units = total_units
    measures.complete_units = complete_units
    return measures

def get_functions(unit_name):
    funcs = data[unit_name][".text"]
    functions = []

    # TODO: determine actual percentage based on tools/check?
    def get_function_fuzzy_match_percent(fun):
        if fun["status"] == "Matching":
            return 100.0
        elif fun["status"] == "NonMatchingMinor":
            return 75.0
        elif fun["status"] == "NonMatchingMajor":
            return 25.0
        else:
            return 0.0
        
    def get_function_demangled_name(name):
        try:
            return cxxfilt.demangle(name)
        except cxxfilt.InvalidName:
            return None
    
    for fun in funcs:
        mangled_name = fun["label"] if isinstance(fun["label"], str) else fun["label"][0]
        demangled_name = get_function_demangled_name(mangled_name)

        function = ReportItem()
        function.name = mangled_name
        function.size = fun["size"]
        function.fuzzy_match_percent = get_function_fuzzy_match_percent(fun)

        metadata = ReportItemMetadata()
        if demangled_name is not None:
            metadata.demangled_name = demangled_name
        metadata.virtual_address = fun["offset"]

        function.metadata.CopyFrom(metadata)
        function.address = fun["offset"] - funcs[0]["offset"]
        functions.append(function)
    
    return functions

def get_units():
    units = []
    for unit_name, unit_data in data.items():
        unit = ReportUnit()
        unit.name = unit_name.removesuffix(".o")
        unit.measures.CopyFrom(get_measures(unit_name))
        unit.sections.extend([])  # TODO: no splitting by sections yet
        unit.functions.extend(get_functions(unit_name))
        
        metadata = ReportUnitMetadata()
        # metadata.complete = False  # TODO: no linking yet
        metadata.module_name = "/".join(unit_name.split("/")[:-1])
        metadata.module_id = hash(metadata.module_name) & 0xffffffff
        metadata.source_path = unit.name + ".cpp"
        metadata.progress_categories.extend([])  # TODO: no progress categories yet
        metadata.auto_generated = False

        unit.metadata.CopyFrom(metadata)
        units.append(unit)

    return units

def build_report():
    report = Report()
    report.measures.CopyFrom(get_measures())
    report.units.extend(get_units())
    report.version = 1
    report.categories.extend([])
    return report

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--version", help="Specify which game version to generate the report for")
    parser.add_argument("--output", "-o", help="Output JSON file")
    args = parser.parse_args()

    if args.version != "1.0":
        print(f"Unsupported version: {args.version}. Only '1.0' is supported.")
        sys.exit(1)

    report = build_report()

    from google.protobuf.json_format import MessageToJson
    json_str = MessageToJson(report, indent=2)

    if args.output:
        with open(args.output, "w") as f:
            f.write(json_str)
    else:
        print(json_str)

if __name__ == "__main__":
    main()
