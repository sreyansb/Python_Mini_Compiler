fptr=open("quads.tsv","r")
all_quads=fptr.readlines()[1:]
fptr.close()
all_quads = [(x[:-1].split("\t"))[1:] for x in all_quads]
print(all_quads)
leng=len(all_quads)
for i in range(leng):
    
    for j in range(i+1,leng):
        pass
