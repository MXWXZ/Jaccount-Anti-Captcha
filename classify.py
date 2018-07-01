import os
from fnmatch import fnmatch
from PIL import Image,ImageEnhance

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

filedir = 'src'
f = open("data.txt", "w")
for file in os.listdir(filedir):
    if fnmatch(file, '*.jpg'):
        filename = file
        img=OpenImg(filedir+'/'+filename)
        ret=Split(img)
        cnt=0
        for i in ret:
            data=filename[cnt]
            for x in range(i.height):
               for y in range(i.width):
                   data+=str(i.getpixel((y, x)))
            f.write(data+'\n')
            cnt+=1
f.close()