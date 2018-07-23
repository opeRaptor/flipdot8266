import numpy
import requests
from PIL import Image, ImageDraw
from datetime import datetime

#url = "http://10.0.0.91/path?=0&=0&=0&=0&=48&=56&=440&=492&=6140&=8184&=7936&=768&=768&=768&=768&=256&=0&=0&=0&=0"
now = datetime.now().strftime('%M')
while True:
    
    while  datetime.now().strftime('%M') != now:
        url = "http://10.0.0.91/path?="


        channels =0
        x=0
        id=0

        img = Image.new('RGBA', (20, 14))
        #img = Image.new('RGBA', (20, 14), color = (0, 0, 0))
        d = ImageDraw.Draw(img)
        d.text((-1,-1), datetime.now().strftime('%H'), fill=(0,0,0))
        d.text((9,5), datetime.now().strftime('%M'), fill=(0,0,0))

        #d.text((-1,-1), "19~", fill=(0,0,0))
        #d.text((9,5), "00", fill=(0,0,0))
        img.save('image.png')




        image = Image.open('image.png', 'r')
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
            y = height
            while y > 0:
                bit = pixel_values[y-1,x]
                out = (out << 1) | bit
                y -= 1
            #for bit in pixel_values[:,x]:
            #    out = (out << 1) | bit
            print(out)
            url = url + str(out)
            url = url +"&="

        url = url [:-2]
        r = requests.get(url)
        print (url)
        now = datetime.now().strftime('%M')
    