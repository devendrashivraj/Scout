import time
from itoc import *
from cv import *

def decide():
    left = recognizer('left.jpg')
    right = recognizer('center.jpg')
    center = recognizer('right.jpg')

    if (left and (not right) and (not center)):
        return 108
    elif (right and (not left) and (not center)):
        return 114
    elif (center):
        return 99
    else:
        return 108

while(1):
    print 'test'
    capture_image()
    print 'cap'
    upload()
    print 'up'
    com = decide()
    print com
    send_command(com)
    
