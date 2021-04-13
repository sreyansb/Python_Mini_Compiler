fptr=open("outputs/evalCSE.tsv","r")
all_quads=fptr.readlines()[1:]
fptr.close()
all_quads = [(x[:-1].split("\t"))[1:] for x in all_quads]
