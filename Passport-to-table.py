sntdc = input("Did u change mod no?\n Enter SnTDC: ")


arrL = [6, 9, 0, 3, 7, 10, 1, 4, 8, 11, 2, 5, 15, 12, 21, 18, 16, 13, 22, 19, 17, 14, 23, 20]
arrR = [17, 14, 23, 20, 16, 13, 22, 19, 15, 12, 21, 18, 8, 11, 2, 5, 7, 10, 1, 4, 6, 9, 0, 3]

f = open("demofile5.txt", "a")


loops = 0
while loops < 3:
    det = input("Enter detector number: ")
    lr = input("Enter l/r or space for skip: ")
    if lr == "r":
        i = 0
        for channel in arrR:
            f.write(str(sntdc) + " " + str(i) + " 14 " + str(det) + " " + str(arrL[channel]) + " R\n") #module number!!!
            i += 1
        loops += 1
    elif lr == "l":
        i = 0
        for channel in arrL:
            f.write(str(sntdc) + " " + str(i) + " 14 " + str(det) + " " + str(arrL[channel]) + " L\n") #module number!!!
            i += 1
        loops += 1
    elif lr == " ": loops += 1
    else: print("1 or 0 needed")
    print(str(loops) + ' of 3\n')


f.close()
