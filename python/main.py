import time
from itoc import *
from cv import *

def decide():
    left = recognizer('left.jpg')
    center = recognizer('center.jpg')
    right = recognizer('right.jpg')
    print left,center,right
    if (center):
        return 99
    elif (left and (not right) and (not center)):
        return 108
    elif (right and (not left) and (not center)):
        return 114
    else:
        return 103

while(1):
    print 'test'
    time.sleep(1)
   
    capture_image()
    print 'cap'
    upload()
    print 'up'
    com = decide()
    print com
    send_command(com)
    
