import time
from picamera import PiCamera
from io import BytesIO
import picamera.array
from PIL import Image
from PIL import ImageFilter
from PIL import ImageEnhance
import numpy as np
import boto3

global left, right, center


rek = boto3.client('rekognition')
s3 = boto3.client('s3')
stream = BytesIO()
left_box = (0,0, 864, 1944)
center_box = (864, 0, 1728, 1944)
right_box = (1728, 0, 2592, 1944)

global camera
camera = PiCamera()
camera.resolution = (2592, 1944)
camera.framerate= 30

def capture_image():
    camera.capture(stream, format = 'jpeg')
    stream.seek(0)
    image = Image.open(stream)

     
    enhancer = ImageEnhance.Brightness(image)
    imf = enhancer.enhance(3)
    #imf = imf.filter(ImageFilter.FIND_EDGES)

    iml = imf.crop(left_box)
    imc = imf.crop(center_box) 
    imr = imf.crop(right_box)

    iml.save('left.jpg')
    imc.save('cent.jpg')
    imr.save('rt.jpg')

    

def upload():
    s3.upload_file('left.jpg','hackbeans','left.jpg')
    s3.upload_file('cent.jpg','hackbeans','center.jpg')
    s3.upload_file('rt.jpg','hackbeans','right.jpg')

def recognizer(image_filename):
    response = rek.detect_labels(
        Image={
            'S3Object':{
                'Bucket':'hackbeans',
                'Name':image_filename
            }
            }
    )

    #print response
    for i in response['Labels']:
        print i['Name']
        if i['Name'] == 'Human':
            return True
    print '%n'

##capture_image()
##upload()
##left = recognizer('left.jpg')
##right = recognizer('center.jpg')
##center = recognizer('right.jpg')
