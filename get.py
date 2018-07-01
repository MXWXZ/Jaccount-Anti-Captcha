import cv2
from PIL import Image,ImageEnhance
import pytesseract
import urllib.request
import os

if not os.path.isdir('src'):
    os.mkdir('src')

cv2.namedWindow("img", cv2.WINDOW_KEEPRATIO)
for i in range(1000):
    file=urllib.request.urlopen('https://jaccount.sjtu.edu.cn/jaccount/captcha')
    data=file.read()
    fhandle=open("src/captcha.jpg","wb")
    fhandle.write(data)
    fhandle.close()

    im = Image.open('src/captcha.jpg')
    im=im.convert('L')
    im=ImageEnhance.Contrast(im)
    im=im.enhance(3)
    img2 = Image.new('RGB', (150, 60), (255, 255, 255))
    img2.paste(im.copy(), (25, 10))
    code=pytesseract.image_to_string(img2)

    if code.isalpha():
        img=cv2.imread('src/captcha.jpg')
        img=cv2.resize(img,(200,100))
        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(img, code, (0, 25), font, 1, (0, 0, 0), 2)
        cv2.imshow("img",img)
        if(cv2.waitKey(0)==32):
            os.rename('src/captcha.jpg','src/'+code+'.jpg')
        
