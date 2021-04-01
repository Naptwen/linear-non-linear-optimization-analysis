import sysconfig
import requests

def src_URL(name):
    print('Obtaning src from ' + name)
    url = requests.get(name)
    print('python loading url finish')
    return url.text
