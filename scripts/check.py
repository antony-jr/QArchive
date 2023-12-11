#!/usr/bin/env python3
import sys

if len(sys.argv) < 2:
    print("Usage: check.py [GIT COMMIT MESSAGE] [OUTPUT OF git tags]")
    sys.exit(-1)

message = sys.argv[1];
tags = sys.argv[2];
if "Release" in message:
    parts = message.split('[Release ')
    if len(parts) != 2:
        print("false")
        sys.exit(0)

    parts2 = parts[1].split(']')
    if len(parts2) != 2:
        print("false")
        sys.exit(0)

    version = parts2[0]
    version = version.lower()

    # Check if there is a existing tag.
    with open(tags, 'r') as fp:
        for i in fp:
            s = i.strip()
            if version == s:
                print("false")
                sys.exit(0)
    print(version)
else:
    print("false")
    sys.exit(0)
sys.exit(0)
