import subprocess
import sys
import requests

r1 = requests.get(sys.argv[1])
if(r1.status_code == 200):
	with open('image.png', 'wb') as handler:
		handler.write(r1.content)

subprocess.call(['faster'])