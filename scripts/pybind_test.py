import sys
import ers

print("\nList of everything bound in test binding\n")

print(dir(ers))

print('')

name = sys.argv[1]

lc = ers.LocalContext(name, name,1,name,False)

ai = ers.AnyIssue(name, lc, name)

nm = ai.get_class_name()

print(nm)

ers.info(ai)
