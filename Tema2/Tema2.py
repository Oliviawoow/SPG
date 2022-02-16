#Copyright Oprea Olivia Maria-Magdalena 341C5 2022

from ast import Index
import numpy as np
from PIL import Image
from IPython.display import display
import string

def rgb_to_hsv(r, g, b):
    # https://www.rapidtables.com/convert/color/rgb-to-hsv.html
    r /= 255.0
    g /= 255.0
    b /= 255.0
    maxc = max(r, g, b)
    minc = min(r, g, b)
    v = maxc
    if minc == maxc:
        return 0.0, 0.0, v
    s = (maxc - minc) / maxc
    rc = (maxc - r) / (maxc - minc)
    gc = (maxc - g) / (maxc - minc)
    bc = (maxc - b) / (maxc - minc)
    if r == maxc:
        h = 0.0 + bc - gc
    elif g == maxc:
        h = 2.0 + rc - bc
    else:
        h = 4.0 + gc - rc
    h = (h / 6.0) % 1.0
    return h * 360, s * 100, v * 100


if __name__ == "__main__":
    image = np.asarray(Image.open('input.png'))
    sablon = np.asarray(Image.open('sablon.png'))
    width = 1920
    height = 1080
    green = 120
    white = (255, 255, 255)
    black = (0, 0, 0)
    red = (255, 0, 0)

    # aplicarea unui filtru median
    temp = [(0, 0)] * 9
    image2 = np.array(Image.new("RGB", (width, height), "white"))
    for i in range(1, height - 1):
        for j in range(1, width - 1):
            temp[0] = tuple(image[i - 1][j - 1])
            temp[1] = tuple(image[i - 1][j])
            temp[2] = tuple(image[i - 1][j + 1])
            temp[3] = tuple(image[i][j - 1])
            temp[4] = tuple(image[i][j])
            temp[5] = tuple(image[i][j + 1])
            temp[6] = tuple(image[i + 1][j - 1])
            temp[7] = tuple(image[i + 1][j])
            temp[8] = tuple(image[i + 1][j + 1])
            temp.sort()

            shape = (temp[4][0], temp[4][1], temp[4][2])
            image2[i][j] = shape

    Image.fromarray(image2).save('image2.png')
    Image.fromarray(image2).show()

    # conversie din spatiul rgb in spatiul hsv
    image3 = np.array(Image.new("RGB", (width, height), "white"))
    for i in range(height):
        for j in range(width):
            image3[i][j] = rgb_to_hsv(image2[i][j][0], image2[i][j][1], image2[i][j][2])

    # segmentarea imaginii
    visited = np.zeros((height, width))
    q = []
    id = 1

    for i in range(0, height, 2):
        for j in range(width):
            if visited[i][j] == 0 and image3[i][j][0] == green:
                q.append((i, j))
                visited[i][j] = id
                image3[i][j] = white

                while len(q) > 0:
                    center = q.pop(0)
                    upleft = (center[0] - 1, center[1] - 1)
                    up = (center[0] - 1, center[1])
                    upright = (center[0] - 1, center[1] + 1)
                    left = (center[0], center[1] - 1)
                    right = (center[0], center[1] + 1)
                    downleft = (center[0] + 1, center[1] - 1)
                    down = (center[0] + 1, center[1])
                    downright = (center[0] + 1, center[1] + 1)

                    if (image3[upleft[0]][upleft[1]][0] == green and visited[upleft[0]][upleft[1]] == 0):
                        q.append(upleft)
                        visited[upleft[0]][upleft[1]] = id
                        image3[upleft[0]][upleft[1]] = white
                    if (image3[up[0]][up[1]][0] == green and visited[up[0]][up[1]] == 0):
                        q.append(up)
                        visited[up[0]][up[1]] = id
                        image3[up[0]][up[1]] = white
                    if (image3[upright[0]][upright[1]][0] == green and visited[upright[0]][upright[1]] == 0):
                        q.append(upright)
                        visited[upright[0]][upright[1]] = id
                        image3[upright[0]][upright[1]] = white
                    if (image3[left[0]][left[1]][0] == green and visited[left[0]][left[1]] == 0):
                        q.append(left)
                        visited[left[0]][left[1]] = id
                        image3[left[0]][left[1]] = white
                    if (image3[right[0]][right[1]][0] == green and visited[right[0]][right[1]] == 0):
                        q.append(right)
                        visited[right[0]][right[1]] = id
                        image3[right[0]][right[1]] = white
                    if (image3[downleft[0]][downleft[1]][0] == green and visited[downleft[0]][downleft[1]] == 0):
                        q.append(downleft)
                        visited[downleft[0]][downleft[1]] = id
                        image3[downleft[0]][downleft[1]] = white
                    if (image3[down[0]][down[1]][0] == green and visited[down[0]][down[1]] == 0):
                        q.append(down)
                        visited[down[0]][down[1]] = id
                        image3[down[0]][down[1]] = white
                    if (image3[downright[0]][downright[1]][0] == green and visited[downright[0]][downright[1]] == 0):
                        q.append(downright)
                        visited[downright[0]][downright[1]] = id
                        image3[downright[0]][downright[1]] = white
                id += 1
            elif visited[i][j] == 0 and image3[i][j][0] != green:
                image3[i][j] = black
                visited[i][j] = -1

    # Identificarea dreptunghiurilor încadratoare
    # minim I, minim J, maxim I, maxim J
    minmax = [[width + 1, width + 1, -width, -width] for _ in range(id)]

    for i in range(height):
        for j in range(width):
            if visited[i][j] > 0:
                index = int(visited[i][j])
                if minmax[index][0] > i:
                    minmax[index][0] = i
                if minmax[index][1] > j:
                    minmax[index][1] = j
                if minmax[index][2] < i:
                    minmax[index][2] = i
                if minmax[index][3] < j:
                    minmax[index][3] = j
            else:
                image3[i][j] = black

    Image.fromarray(image3).save('image3.png')
    Image.fromarray(image3).show()
    
    for x in range(len(minmax)):
        for j in range(minmax[x][1], minmax[x][3] + 1):
            image3[minmax[x][0]][j] = red
        for i in range(minmax[x][0], minmax[x][2] + 1):
            image3[i][minmax[x][1]] = red
        for j in range(minmax[x][1], minmax[x][3] + 1):
            image3[minmax[x][2]][j] = red
        for i in range(minmax[x][0], minmax[x][2] + 1):
            image3[i][minmax[x][3]] = red

    Image.fromarray(image3).save('image4.png')
    Image.fromarray(image3).show()

    sablonTxt = open('sablon.txt', 'r')
    lines = sablonTxt.readlines()
    result = ""

    for r in range(1, len(minmax) - 1):
        D = (minmax[r][2] - minmax[r][0] + 20, minmax[r][3] - minmax[r][1] + 20)
        if D[0] * D[1] < 700:
            continue

        maxCountPixels = 0
        bestMatchingLetter = ""

        for line in lines:
            line = line.strip().split(" ")
            if len(line) < 2:
                continue

            leftUpD = (minmax[r][0], minmax[r][1])
            leftUpF = (int(line[2]), int(line[1]))

            countPixels = 0

            for i in range(D[0]):
                for j in range(D[1]):
                    if (image3[leftUpD[0] + i][leftUpD[1] + j] == sablon[leftUpF[0] + i][leftUpF[1] + j]).all():
                        countPixels = countPixels + 1

            if countPixels > maxCountPixels:
                maxCountPixels = countPixels
                bestMatchingLetter = line[0]
        if bestMatchingLetter == '!':
            atTheEnd = bestMatchingLetter
            continue
        if bestMatchingLetter != "":
            if abs(minmax[r][1] - minmax[r - 1][3]) >= 46:
                result = result + " " + bestMatchingLetter
            else:
                result = result + bestMatchingLetter

    print((result + atTheEnd)[1:])
