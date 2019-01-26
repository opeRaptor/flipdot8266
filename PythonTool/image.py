import numpy
from PIL import Image
import requests

#url = "http://10.0.0.91/path?=0&=0&=0&=0&=48&=56&=440&=492&=6140&=8184&=7936&=768&=768&=768&=768&=256&=0&=0&=0&=0"
url = "http://10.0.0.91/path?="


channels =0
x=0
id=0


image = Image.open('frog.png', 'r')
width, height = image.size
pixel_values = list(image.getdata(3))
pixel_values = numpy.array(pixel_values)
pixel_values[pixel_values == 255] = 1
print (pixel_values)
pixel_values = numpy.array(pixel_values).reshape((height, width))
print (pixel_values)

x = 0
for x in range(width):
    out = 0
    y = height-1
    
    while y >= 0:
        bit = pixel_values[y,x]
        out = (out << 1) | bit
        y -= 1
    #for bit in pixel_values[:,x]:
    #    out = (out << 1) | bit
    out = out << 1
    print(out)
    url = url + str(out)
    url = url +"&="

url = url [:-2]
print (url)
r = requests.get(url)