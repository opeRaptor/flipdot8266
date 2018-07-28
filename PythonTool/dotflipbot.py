import telegram
from telegram.ext import Updater, CommandHandler,MessageHandler,Filters
import numpy
import requests
from PIL import Image, ImageDraw, ImageFont
import logging


logging.basicConfig(format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',level=logging.INFO)


def file_handler(bot, update):
    file = bot.getFile(update.message.document.file_id)
    print ("file_id: " + str(update.message.document.file_id))
    file.download('image.png')

    image = Image.open('image.png', 'r')
    width, height = image.size
    pixel_values = list(image.getdata(3))
    pixel_values = numpy.array(pixel_values)
    pixel_values[pixel_values < 100] = 0
    pixel_values[pixel_values >= 100] = 1
    print (pixel_values)
    pixel_values = numpy.array(pixel_values).reshape((height, width))
    print (pixel_values)

    x = 0
    url = "http://10.0.0.91/path?="
    for x in range(width):
        out = 0
        y = height-1
        while y >= 0 :
            bit = pixel_values[y,x]
            out = (out << 1) | bit
            y -= 1
        #for bit in pixel_values[:,x]:
        #    out = (out << 1) | bit
        print(out)
        url = url + str(out)
        url = url +"&="

    url = url [:-2]
    r = requests.get(url)

updater = Updater(token='693891744:AAGKF1XG3kjfDzlYB20IAiNw0XdoTSIngpw')
dispatcher = updater.dispatcher
dispatcher.add_handler(MessageHandler(Filters.document, file_handler))

updater.start_polling()
updater.idle()