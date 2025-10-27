#! /usr/bin/env python3
import json
import os
import subprocess

artifacts = None
cache = None
cachePath = f"{os.path.dirname(os.path.realpath(__file__))}/../cache-version.json"
artifactsPath = f"{os.path.dirname(os.path.realpath(__file__))}/artifacts.json"

with open(cachePath) as file:
    cache = json.load(file)

with open(artifactsPath) as file:
    artifacts = json.load(file)
    print(artifacts)

    for system in artifacts:
        cmd = f"nix store prefetch-file --json --unpack {cache['urlPrefix']}/{cache['version']}/{artifacts[system]['filename']}"
        process = subprocess.Popen(
            cmd,
            shell=True,
            stdout=subprocess.PIPE)
        process.wait()
        data, err = process.communicate()

        if process.returncode == 0:
            output = json.loads(data.decode('utf-8'))
            print(f"got {output['hash']} for {output['storePath']}")
            artifacts[system]['hash'] = output["hash"]
        else:
            print(f"artifact {artifact.filename} failed to download!")
            print("stdout")
            print(data)
            print("stderr")
            print(err)
            exit(1)

with open(artifactsPath, "w") as file:
    json.dump(artifacts, file, indent = 4)
