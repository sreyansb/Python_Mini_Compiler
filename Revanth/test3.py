#this is a comment
import hello
x=3
y=2
lis = [10,20,30,"hello1"]#another comment
for i in range(10,20):
	print(i)
	for j in range(10,20):
		print(j)
		print("for")
		while False:
			print("oop")
	print("end")
'''
multiline comment
'''
while(x<5):
	print(x)
	while True:
		print("true")
	for n in range(3,10):
		print(n)
		for d in range(3,10):
			print(d)
else:
	print("in else")


while True:
	print("True")
	while False:
		print("False")
		for l in range(3,10):
			print(l)

for c in lis:
	print(c)
	
def foo():
	print("hello world")
	return False
