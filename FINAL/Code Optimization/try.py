#we assume we get code on which constant propagation and folding is already done
fptr=open("optimized/EVALEDCSEtest3.tsv","r")
all_quads=fptr.readlines()[1:]
fptr.close()
all_quads = [(x[:-1].split("\t"))[1:] for x in all_quads]
for i in all_quads:
	if i[1].startswith('"'):
		print(i[1])
