'''Lots of imports. May not need them all, prune list later'''
import sys
import ers
import inspect
import platform
import os
import sys
import time
import getpass
import _thread

from ers import AnyIssue

'''Wrap a message string as an ERS issue'''
def Message(message):
	# Stuff needed for the Context
    host_name = platform.node()
    cwd = os.getcwd()
    user_id = os.getuid()
    user_name = getpass.getuser()
    process_id = os.getpid() 
    thread_id = int(_thread.get_ident())
    thread_id = int(thread_id%2**31)
    app_name = os.getenv( "DUNEDAQ_APPLICATION_NAME", "Undefined" )
    # This hard-coded stuff should be filled with the correct information, as in the old python Context class
    package_name = "pytest2"
    filename = "pytest2.py"
    line_number = 42
    function_name = "test"
    type="Warning"
    # Create Context using the ers-python bindings
    rpc = ers.RemoteProcessContext(host_name,process_id,thread_id,cwd,user_id,user_name,app_name)
    rc = ers.RemoteContext(package_name,filename,line_number,function_name,rpc)
    # Creat and return an ers issue
    return ers.AnyIssue(type, rc, message)


'''Bunch of functions to wrap a text string and inject it as an ers message of different types'''
def debug( msg, lvl ):
    "sends msg to the debug stream"
    ers.debug( isinstance( msg, ers.AnyIssue ) and msg or Message( msg ), lvl )

def log( msg ):
    "sends msg to the log stream"
    ers.log( isinstance( msg, ers.AnyIssue ) and msg or Message( msg ) )

def info( msg ):
    "sends msg to the information stream"
    ers.info( isinstance( msg, ers.AnyIssue ) and msg or Message( msg ) )

'''These just inject an issue into the more serious streams.
   The only thing these add over just calling the bound ers function directly is that they
   warn you if you use the wrong type of argument.
   Mainly here because the old ers.py had them, and they provide a consistency of interface
   rather than forcing the user to use these calls for some things and direct ers calls for others'''
def warning( issue ):
    "sends issue to the warning stream"
    assert isinstance(issue,ers.AnyIssue), \
            'Only an instance of ers.Issue sub-class can be sent to the ers.warning stream'
    ers.warning( issue )

def error( issue ):
    "sends issue to the error stream"
    assert isinstance(issue,ers.AnyIssue), \
            'Only an instance of ers.Issue sub-class can be sent to the ers.error stream'
    ers.error( issue )

def fatal( issue ):
    "sends issue to the fatal stream"
    assert isinstance(issue,ers.AnyIssue), \
            'Only an instance of ers.Issue sub-class can be sent to the ers.fatal stream'
    ers.fatal( issue )
