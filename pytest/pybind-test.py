''' Script to test python bindings of ers methods
    Usage: python pybind-test.py <message>'''
import sys
import inspect
import platform
import os
import sys
import time
import getpass
import _thread

# This is the pybind11 binding 
import ers
# This is a python script with some interface classes and functions
import erspy

from ers import AnyIssue

print("\nList of everything bound in test binding\n")

print(dir(ers))

print('')

message = sys.argv[1]

host_name = platform.node()
cwd = os.getcwd()
user_id = os.getuid()
user_name = getpass.getuser()
process_id = os.getpid() 
thread_id = int(_thread.get_ident())
thread_id = int(thread_id%2**31)
app_name = os.getenv( "DUNEDAQ_APPLICATION_NAME", "Undefined" )

package_name = "pytest2"
filename = "pytest2.py"
line_number = 42
function_name = "test"

type="Python test"



lc = ers.LocalContext(package_name,filename,line_number,message,False)
rpc = ers.RemoteProcessContext(host_name,process_id,thread_id,cwd,user_id,user_name,app_name)
rc = ers.RemoteContext(package_name,filename,line_number,function_name,rpc)

ai1 = ers.AnyIssue(type, lc, message)
ai2 = ers.AnyIssue(type, rc, message)


nm = ai2.get_class_name()
what = ai2.what()
cause = ai2.cause()
severity = ai2.severity()
qualifiers = ai2.qualifiers()
parameters = ai2.parameters()

print("Class name (remote context) = ",ai2.get_class_name())
print("severity (remote context) = ",ai2.severity().type,", ",ai2.severity().rank)

print("debug_level = ",ers.debug_level())


# Directly use bound ers library calls to log issues created above
ers.info(ai1)
ers.log(ai2)
ers.warning(ai2)
ers.error(ai2)
ers.fatal(ai2)
ers.debug(ai2,5)

# Use python interfaces to log string as issue
erspy.info(message)
erspy.log(message)
# Use python def to wrap string as an ers issue
ai3 = erspy.Message(message)
# These python methods only accept issues as input
erspy.warning(ai3)
erspy.error(ai3)
erspy.fatal(ai3)
erspy.debug(ai2, 5)

