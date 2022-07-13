from ._daq_ers_py import *

import sys
import ers
import inspect
import platform
import os
import sys
import re
import time
import getpass
import _thread
import logging

class PyContext( object ):
    "extracts context for an ERS issue"    
    __file = re.sub( r'\.pyc$', '.py', __file__ )

    def __init__( self, issue ):
        self.stack = [f[0] for f in inspect.stack() \
                        if  f[1] != self.__file \
                        and (      'self' not in f[0].f_locals\
                                or not isinstance(f[0].f_locals['self'], ers.PyIssue))]
        self.package_name = issue.__class__.__module__                                
        class_name    = lambda : 'self' in self.stack[0].f_locals \
                                 and self.stack[0].f_locals['self'].__class__.__name__ + '.'\
                                 or ''

        self.function_name = class_name() + self.stack[0].f_code.co_name;
        try:
            self.function_name += inspect.formatargvalues( *inspect.getargvalues(self.stack[0]) )
        except Exception as exx:
            self.function_name += '(...)'
                
        self.file_name = self.stack[0].f_code.co_filename
        self.line_number = self.stack[0].f_lineno
        self.host_name = platform.node()
        self.cwd = os.getcwd()
        self.process_id = os.getpid()
        # Need to truncate this to fit 32 bit signed int
        self.thread_id = int(_thread.get_ident()%2**31)
        self.user_id = os.getuid()
        self.user_name = getpass.getuser()
        self.application_name = os.getenv( "DUNEDAQ_APPLICATION_NAME", "Undefined" )


class PyIssue( Exception ):
    "base class for Python  ERS exceptions"    
    __verbosity = int( os.getenv( "DUNEDAQ_ERS_VERBOSITY_LEVEL", "0" ) )
    
    def __init__( self, message, kwargs, cause ):
        Exception.__init__( self, message, kwargs, cause )
        self.time = time.time()
#        self.severity = ers.severity.Error
        self.message = message
        self.cause = cause
        self.__context = PyContext(self)
        self.context_parameters = vars(self.__context)
        self.qualifiers = [ self.__context.package_name ]
        self.__parameters = dict([ (str(k), str(v)) for (k,v) in list(kwargs.items()) ])
        self.__dict__.update( kwargs )
        self.lc = ers.LocalContext(self.__context.application_name, self.__context.file_name, self.__context.line_number, self.__context.function_name, False)
        self.ai = ers.AnyIssue( self.__context.function_name, self.lc, message )        
        
    @property
    def context( self ):
        return self.__context
            
    @property
    def parameters( self ):
        return self.__parameters
                    
    def isInstanceOf( self, cname ):
            return self.__class__.__name__ == cname and True or False
        
    def __str__( self ):
        return self.message

'''Wrap a message string as an ERS issue'''
def message(message):
    # This class collects information needed for message context
    ct = PyContext(message)
    # Still need to do something about these
    package_name = "erspy"
    type="Warning"
    # Create Context using the ers-python bindings
    rpc = ers.RemoteProcessContext(ct.host_name,ct.process_id,ct.thread_id,ct.cwd,ct.user_id,ct.user_name,ct.application_name)
    rc = ers.RemoteContext(package_name,ct.file_name,ct.line_number,ct.function_name,rpc)
    # Create and return an ers issue
    return ers.AnyIssue(type, rc, message)


'''Bunch of functions to wrap a text string and inject it as an ers message of different types'''
def pydebug( msg, lvl ):
    "sends msg to the debug stream"
    ers.debug( isinstance( msg, ers.AnyIssue ) and msg or message( msg ), lvl )

def pylog( msg ):
    "sends msg to the log stream"
    ers.log( isinstance( msg, ers.AnyIssue ) and msg or message( msg ) )

def pyinfo( msg ):
    "sends msg to the information stream"
    ers.info( isinstance( msg, ers.AnyIssue ) and msg or message( msg ) )

'''These just inject an issue into the more serious streams.
   The only thing these add over just calling the bound ers function directly is that they
   warn you if you use the wrong type of argument.
   Mainly here because the old ers.py had them, and they provide a consistency of interface
   rather than forcing the user to use these calls for some things and direct ers calls for others'''
def pywarning( issue ):
    "sends issue to the warning stream"
    assert isinstance(issue, ers.AnyIssue), \
            'Only an instance of ers.Issue sub-class can be sent to the ers.warning stream'
    ers.warning( issue )

def pyerror( issue ):
    "sends issue to the error stream"
    assert isinstance(issue, ers.AnyIssue), \
            'Only an instance of ers.Issue sub-class can be sent to the ers.error stream'
    ers.error( issue )

def pyfatal( issue ):
    "sends issue to the fatal stream"
    assert isinstance(issue, ers.AnyIssue), \
            'Only an instance of ers.Issue sub-class can be sent to the ers.fatal stream'
    ers.fatal( issue )
