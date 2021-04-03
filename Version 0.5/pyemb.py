import sysconfig
import requests

def src_URL(name):
    url = requests.get(name)
    return url.text
