#!/usr/bin/env python3

import os
import urllib.parse

print("Content-Type: text/html")
print()

query_string = os.environ.get('_QUERY_STRING', '')
params = urllib.parse.parse_qs(query_string)
name = params.get('name', ['Guest'])[0]

print(f"<html><body><h1>Hello, {name}!</h1><p>CGI Script Executed Successfully</p></body></html>")
