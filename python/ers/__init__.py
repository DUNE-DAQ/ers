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
        self.qualifiers = [ self.__context.package_name ]
        self.__parameters = dict([ (str(k), str(v)) for (k,v) in list(kwargs.items()) ])
        self.__dict__.update( kwargs )
        self.lc = ers.LocalContext(self.__context.application_name, self.__context.file_name, self.__context.line_number, self.__context.function_name, False)
        self.ai = ers.AnyIssue( message, self.lc, message )        
        
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
        
