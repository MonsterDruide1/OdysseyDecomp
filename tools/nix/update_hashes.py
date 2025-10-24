#! /usr/bin/env python3
import json
import os
import subprocess

tools = None
toolsPath = f"{os.path.dirname(os.path.realpath(__file__))}/tools.json"

with open(toolsPath) as file:
    tools = json.load(file)
    print(tools["artifacts"])

    for system in tools["artifacts"]:
        cmd = f"nix store prefetch-file --json --unpack {tools['cacheUrlPrefix']}{tools['version']}/{tools['artifacts'][system]['filename']}"
        process = subprocess.Popen(
            cmd,
            shell=True,
            stdout=subprocess.PIPE)
        process.wait()
        data, err = process.communicate()

        if process.returncode == 0:
            output = json.loads(data.decode('utf-8'))
            print(f"got {output['hash']} for {output['storePath']}")
            tools["artifacts"][system]['hash'] = output["hash"]
        else:
            print(f"artifact {artifact.filename} failed to download!")
            print("stdout")
            print(data)
            print("stderr")
            print(err)
            exit(1)

with open(toolsPath, "w") as file:
    json.dump(tools, file, indent = 4)
