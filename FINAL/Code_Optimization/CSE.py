'''
CODE LOGIC:
O(n^2) code
We loop between lines in quads and check if same line exists with different results
----------->Would work better with constant folded and propagated quads.
----------->Break the inner loop as soon as one of the args in the inner quad line is 
            the result of another line in quads(Import and Print are exempted).
----------->If the found line has same operator and arguments as outer loop quad and the result
            is a temporary, delete the line and replace all occurences of that temporary with
            the result of the quad on which the search is being done.
'''
import re
fptr=open("non_optimized/show.tsv","r")
all_quads=fptr.readlines()[1:]
fptr.close()
all_quads = [(x[:-1].split("\t"))[1:] for x in all_quads]
todel=set()
print(len(all_quads))
for i in range(len(all_quads)-1):
    op,arg1,arg2,res=all_quads[i]
    if op=="PRINT" or op=="GOTO" or op=="LABEL":
        continue
    results=set()
    for j in range(i+1,len(all_quads)):
        opj,arg1j,arg2j,resj=all_quads[j]
        #if arg1 or arg2 is reassigned values and opj cant be Import or print because their result is -
        if (resj==arg1 or resj==arg2) and (opj!="IMPORT" and opj!="PRINT"):
            break
        #to remove a line : only temporaries need to be removed
        if op==opj and arg1==arg1j and arg2==arg2j and (re.search(r"^t[1-9]",resj)):
            results.add(resj)
            todel.add(j)
        if arg1j in results:
            all_quads[j][1]=res
        if arg2j in results:
            all_quads[j][2]=res
    #print(i,results)
all_quads=[all_quads[i] for i in (set(range(len(all_quads)))-todel)]
all_quads=["\t".join(i)+"\n" for i in all_quads]
all_quads=[str(i+1)+"\t"+all_quads[i] for i in range(len(all_quads))]
print(len(all_quads))
all_quads="".join(all_quads)
f=open("optimized/showCSE.tsv","w")
f.write("#\top\tA1\tA2\tRes\n")
f.write(all_quads)
f.close()

    

