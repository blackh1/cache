with open(r"ma.out", 'r') as src:
    dst=open(r"MA2.out", 'w')
    while 1:
        line = src.readline()
        if not line:
            break
        thisline = line.strip().split(" ")
        # if thisline[0]!="0:":
        if thisline[1]=="global:":
            if thisline[2]=="Write":
                dst.write("w"+' '+thisline[-1]+'\n')
            elif thisline[2]=="Read":
                dst.write("r"+' '+thisline[-1]+'\n')
            else:
                pass
dst.close()

