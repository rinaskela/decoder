with open('demofile2.txt') as f:
    lines = f.readlines()
f.close()

file = open("demofile6.txt", "a")
for word in range(0,len(lines)):
    print(lines[word])
    linestart=lines[word][:9]
    linedel=lines[word][9:]
    n=linedel.find(' ')
    print(linestart + linedel[:n+1] + '19' + linedel[n+3:])
    file.write(linestart + linedel[:n+1] + '19' + linedel[n+3:])

file.close()
# f = open("demofile5.txt", "a")
#
#
# loops = 0
# while loops < 3:
#     det = input("Enter detector number: ")
#     lr = input("Enter l/r or space for skip: ")
#     if lr == "r":
#         i = 0
#         for channel in arrR:
#             f.write(str(sntdc) + " " + str(i) + " 14 " + str(det) + " " + str(arrL[channel]) + " R\n") #module number!!!
#             i += 1
#         loops += 1
#     elif lr == "l":
#         i = 0
#         for channel in arrL:
#             f.write(str(sntdc) + " " + str(i) + " 14 " + str(det) + " " + str(arrL[channel]) + " L\n") #module number!!!
#             i += 1
#         loops += 1
#     elif lr == " ": loops += 1
#     else: print("1 or 0 needed")
#     print(str(loops) + ' of 3\n')
#

f.close()
