import sys
import ers
import logging
"""
#print("\nList of everything bound in test binding\n")

#print(dir(ers))

#print('')

name = sys.argv[1]

lc = ers.LocalContext(name, name,1,name,False)

ai = ers.AnyIssue(name, lc, name)

#nm = ai.get_class_name()

#print(nm)



ers.error(ai)

"""


class PermissionDenied( ers.PyIssue ):
    def __init__( self, cause = None ):
        ers.PyIssue.__init__( self, 'Permission denied', {}, cause )

class CantOpenFile( ers.PyIssue ):
    def __init__( self, fname, err, cause = None ):
        ers.PyIssue.__init__( self, 'Can not open "%s" file, error = %d' % ( fname, err ),
	    { 'filename' : fname, 'error' : err },
	    cause )

class Test(object):
    def method(self):
        raise PermissionDenied()

def test_function( arg1, arg2 ):
    format = 'This is a %s message'
    ers.pydebug(ers.message(format % 'debug'), 1)
    ers.pylog(ers.message(format % 'log'))
    ers.pyinfo(ers.message(format % 'info'))
    ers.pywarning(ers.message(format % 'warning'))
    ers.pyerror(ers.message(format % 'error'))
    ers.pyfatal(ers.message(format % 'fatal'))    
    ers.pylog(ers.message("-----------TESTING LOG TYPES DONE------------")) 
    try:
        t = Test()
        t.method()
    except PermissionDenied as e:
        raise CantOpenFile( "test.py", 13, e )

import time

if __name__ == "__main__":

    try:
    	test_function( "test", "ers" )
    except ers.PyIssue as e:
        ers.pywarning(e.ai)
        ers.pyinfo(e.ai)
        ers.pyerror(e.ai)
        print(vars(e))
