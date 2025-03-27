#!/usr/bin/env python3

import os
import urllib.parse
import sys

try:
	query = os.environ.get('_QUERY', '')
	params = urllib.parse.parse_qs(query)
	name = params.get('name', ['guest'])[0]
	with open('tools/website/cgi/cgi.html', 'r') as file:
		content = file.read()

	custom_text = f"<div><h2 class=\"text-4xl font-bold mb-10\">Hello, {name}!</h2></div>"
	content = content.replace('<!--PLACEHOLDER-->', custom_text)

	print("Content-Type: text/html")
	print()
	print(content)
except Exception as e:
	print(f"{e}", file=sys.stdout)
	exit(1)
