import sysconfig
import requests


front = '<div class="YMlKec">'
rear = "</div>"

def hello(name):
    print('python loading url')
    print("https://www.google.com/finance/quote/AAPL:NASDAQ")
    url = requests.get("https://www.google.com/finance/quote/AAPL:NASDAQ")
    print('python loading url finish')
    return url

if __name__ == "__main__":
    hello()