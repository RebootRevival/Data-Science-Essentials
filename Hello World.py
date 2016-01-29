b=""
for i in range(0,36):
   if (i+1)%3 is 0:
     a=map(str,"072101108108111032119111114108100033")[i-2:i+1]
     b+=chr(int(''.join(a)))
print b
