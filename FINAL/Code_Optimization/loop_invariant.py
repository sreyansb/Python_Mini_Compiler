#we assume we get quads on which constant propagation and folding is already done
#we only bring those records out which are assignment and have assignment to constants
fptr=open("optimized/foprInvariance.tsv","r")
all_quads=fptr.readlines()[1:]
fptr.close()
all_quads = [(x[:-1].split("\t"))[1:] for x in all_quads]
i=0
n=len(all_quads)

def recurse(i,n=n):#i is the index of the record
	global all_quads
	op,arg1,arg2,res=all_quads[i]
	if op=="IF" and arg1!="0" and arg1!="FALSE":#indicates loop in our code atleast
		label_index=-1
		for j in range(i-1,-1,-1):
			if all_quads[j][0].upper()=="LABEL":
				label_index=j
				break
		final_label=all_quads[i+1][-1]
		j=i+2
		invariants=[]
		while(j<n):
			opj,arg1j,arg2j,resj=all_quads[j]
			flag=j+1
			if opj.upper()=="LABEL" and resj==final_label:
				break
			try:
				if opj=="=" and (arg1j.startswith('"') or int(arg1j)):
					invariants.append(all_quads[j])
				if opj.upper()=="IF":
					flag=recurse(j)
			except:
				pass
			j=flag
		
		after=[all_quads[k] for k in range(label_index,j) if all_quads[k] not in invariants]
		all_quads=all_quads[:label_index]+invariants+after+all_quads[j:n]
		return j+1
	return i+1

while i<n:
	i=recurse(i)

all_quads=["\t".join(i)+"\n" for i in all_quads]
all_quads=[str(i+1)+"\t"+all_quads[i] for i in range(len(all_quads))]
print(len(all_quads))
all_quads="".join(all_quads)
f=open("optimized/invarianted.tsv","w")
f.write("#\top\tA1\tA2\tRes\n")
f.write(all_quads)
f.close()	
				


