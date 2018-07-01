from PIL import Image
import urllib.request
import os
from PIL import Image,ImageEnhance
import cv2

table = []
for i in range(256):
    if i < 64:
        table.append(0)
    else:
        table.append(1)

def OpenImg(path):
    img = Image.open(path)
    # 图像增强
    img=img.convert('L')
    img=ImageEnhance.Contrast(img)
    img=img.enhance(3)
    # 二值化
    img=img.point(table,'1')
    return img

# 垂直切割
def VSplit(img):
    x=0
    while x < img.height:
        flg=0
        for y in range(img.width):
            if img.getpixel((y, x)) == 0:
                flg=1
                break
        if flg:
            m=img.height-1
            while m > x:
                flg=1
                for n in range(img.width):
                    if img.getpixel((n, m)) == 0:
                        flg=0
                        break
                if flg:
                    croped = img.crop((0, x, 15, m))
                    newimg=Image.new('1', (15, 20), 1)
                    newimg.paste(croped,(0,0))
                    return newimg
                m-=1
            break
        x+=1

# 水平切割
def HSplit(img):
    ret=[]
    y=0
    while y < img.width:
        flg=0
        for x in range(img.height):
            if img.getpixel((y, x)) == 0:
                flg=1
                break
        if flg:
            n=y+1
            while n < img.width:
                flg=1
                for m in range(img.height):
                    if img.getpixel((n, m)) == 0:
                        flg=0
                        break
                if flg:
                    croped = img.crop((y, 0, n, 40))
                    newimg=Image.new('1', (15, 40), 1)
                    newimg.paste(croped,(0,0))
                    ret.append(newimg)
                    break
                n+=1
            y=n
        y+=1
    return ret

# 切割图像
def Split(img):
    ret=[]
    tmp=HSplit(img)
    for i in tmp:
        ret.append(VSplit(i))
    return ret

cv2.namedWindow("img", cv2.WINDOW_KEEPRATIO)
while(1):
    file=urllib.request.urlopen('https://jaccount.sjtu.edu.cn/jaccount/captcha')
    data=file.read()
    fhandle=open("captcha.jpg","wb")
    fhandle.write(data)
    fhandle.close()

    f = open("test.txt", "w")
    img=OpenImg('captcha.jpg')
    ret=Split(img)
    for i in ret:
        data=''
        for x in range(i.height):
            for y in range(i.width):
                data+=str(i.getpixel((y, x)))
        f.write(data+'\n')
    f.close()

    os.system('Network.exe')
    img=cv2.imread('captcha.jpg')
    cv2.imshow("img",img)
    cv2.waitKey(0)