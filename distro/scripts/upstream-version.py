#!/usr/bin/env python3

import json
import urllib.request

with urllib.request.urlopen('https://gitlab.nic.cz/api/v4/projects/820/releases?order_by=released_at&sort=desc') as response:
    data = json.loads(response.read())

print(data[0]['tag_name'])
