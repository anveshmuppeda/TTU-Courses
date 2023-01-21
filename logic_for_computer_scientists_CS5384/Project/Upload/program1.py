file1=open("D:\TTU SEM 1\Logic of Computer Scientists\Project\Python programs\CNF_file.cnf","w")
n=int(input())

#To calculate number of clauses
r_diag=(n-1)*n/2
t=n-2
while t>0:
    r_diag=r_diag+2*(t*(t+1)/2)
    t=t-1
n_clauses=n+(2*(n*(n*n-n)/2))+(2*r_diag)
file1.write("p cnf "+str(n*n)+" "+str(int(n_clauses))+"\n")

#To check if there is a queen in every row
for i in range(1,n*n+1):
    file1.write(str(i)+" ")
    if i%n == 0:
        file1.write("0\n")
    
file1.write("\n\n")

#To check if every row has only one queen
for i in range(1,n*n+1):
    for j in range(i,((int(i/n)+1)*n)+1):
        if i != j:
            if i%n !=0:
                file1.write(str(i*-1)+" "+str(j*-1)+" 0\n")

file1.write("\n\n")

#To check if every column has only one queen
for i in range(1,(n*n)+1):
    j=i
    while j in range(i,n*n+1):
        if i != j:
            file1.write(str(i*-1)+" "+str(j*-1)+" 0\n")
        j=j+n
        #print(i)

file1.write("\n\n")

#To check if every right diagonal has only one queen
for i in range(1,n*n-n):
    j=i
    while j in range(i,n*n+1):
        if i != j:
            if i%n != 0:
                file1.write(str(i*-1)+" "+str(j*-1)+" 0\n")
        if j%n == 0:
            break
        j=j+n+1
        

file1.write("\n\n")

#To check every left diagonal has only one queen
for i in range(1,(n*n-n)+1):
    j=i
    while j in range(i,n*n+1):
        if i != j:
            if i%n != 1:
                file1.write(str(i*-1)+" "+str(j*-1)+" 0\n")
        if j%n == 1:
            break
        j=j+n-1