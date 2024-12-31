from advanced_calculator import AdvancedCalculator
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
filename = os.path.join(script_dir, "validTestCases.txt")

file1=open(filename,'r')
adv=AdvancedCalculator()
totalcount=0
passcount=0
by0testcount=0
by0testpasscount=0
print("This test involves only valid cases,\nie those cases whose value can be found without any errors.")
print("Started please w8")
for line in file1:
    totalcount+=1
    exp=""
    for i in line:
        if i=='{':
            exp+='('
        elif i=='}':
            exp+=')'
        else:
            exp+=i
    result=adv.evaluate_expression(line[:-1])
    if(type(result)==str):
        try:
            eval(exp)
        except Exception as e:
            by0testcount+=1
            print(totalcount)
            if(result=="Error"):
                passcount+=1
                by0testpasscount+=1
                continue
        print()
        print("Failed for ")
        print(line[:-1])
        print("expected",eval(exp))
        print("found",result)
        print()
        continue
    if(abs(eval(exp)-result)/eval(exp)<1e-3):
        passcount+=1
    else:
        print()
        print("Failed for ")
        print(line[:-1])
        print("expected",eval(exp))
        print("found",result)
        print()

print("passes",passcount,"out of",totalcount)