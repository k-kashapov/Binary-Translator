prerender = open ("POW.prerender")

for line in prerender:
    for byte in range (len (line[0:-1:2])):
        print ("0x" + line[byte * 2] + line[byte * 2 + 1] + ", ", end = "")
