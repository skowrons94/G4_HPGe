import os
import sys
import numpy as np
import subprocess

file = sys.argv[1]

x, y, z, d = 0, 0, 0, 7
iters = 2

for i in range( iters ):

    # Wait for 2 seconds
    subprocess.run(["sleep", "2"])

    # Sample y from a uniform distribution between -3 and 3
    x = np.random.uniform(-3, 3)
    # Sample z from a uniform distribution between -3 and 3
    y = np.random.uniform(-3, 3)

    with open(file, 'r') as f:
        lines = f.readlines()
        for i, line in enumerate(lines):
            if "xxx" in line:
                lines[i] = lines[i].replace("xxx", str(x))
            if "yyy" in line:
                lines[i] = lines[i].replace("yyy", str(y))
            if "zzz" in line:
                lines[i] = lines[i].replace("zzz", str(z))
            if "ddd" in line:
                lines[i] = lines[i].replace("ddd", str(d))
    
    with open("mac/run.mac", 'w') as f:
        f.writelines(lines)

    # Get source name from file
    source = file.split("/")[-1].split(".")[0]

    subprocess.run(["./G4_HPGe", "mac/run.mac"])
    subprocess.run(["mv", "sim.root", "{}_{}_{}.root".format(source, x, y)])