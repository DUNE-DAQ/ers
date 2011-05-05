"""Error reporting service

This module is part of the Error Reporting Service (ERS) 
package of the ATLAS TDAQ system.
"""

__author__ = "Serguei Kolos (Serguei.Kolos@cern.ch)"
__version__ = "$Revision: 1.0 $"
__date__ = "$Date: 2010/06/05 21:57:20 $"

import getpass
import inspect
import logging
import os
import platform
import re
import sys
import thread
import time

class Severity( object ):
    "defines supported severity values"
    values = ( DEBUG , LOG, INFO, WARNING, ERROR, FATAL ) = tuple( range( 6 ) )

SeverityNames = tuple ( [ [k for k, v in Severity.__dict__.iteritems() if v == s][0] \
					for s in Severity.values ] )

class Context( object ):
    "stores context of an ERS issue"    
    __file = re.sub( r'\.pyc$', '.py', __file__ )

    def __init__( self, issue ):
	self.stack = [f[0] for f in inspect.stack() if f[3] <> '__init__' and f[1] <> self.__file ]
        self.package_name = issue.__class__.__module__
        class_name    = lambda : self.stack[0].f_locals.has_key( 'self' ) \
        			 and self.stack[0].f_locals['self'].__class__.__name__ + '.'\
                                 or ''

        self.function_name =	class_name() \
        			+ self.stack[0].f_code.co_name \
                                + inspect.formatargvalues( *inspect.getargvalues(self.stack[0]) );
	self.file_name = self.stack[0].f_code.co_filename
        self.line_number = self.stack[0].f_lineno
        self.host_name = platform.node()
        self.cwd = os.getcwd()
        self.process_id = os.getpid()
        self.thread_id = thread.get_ident()
        self.user_id = os.getuid()
        self.user_name = getpass.getuser()
                
class Issue( Exception ):
    "base class for ERS exceptions"    
    __verbosity = int( os.getenv( "TDAQ_ERS_VERBOSITY_LEVEL", "0" ) )
    
    def __init__( self, message, kwargs, cause ):
    	Exception.__init__( self )
        self.time = time.time()
        self.severity = Severity.ERROR
        self.message = message
        self.cause = cause
        self.__context = Context( self )
        self.qualifiers = [ self.__context.package_name ]
	self.__dict__.update( kwargs )
    
    @property
    def context( self ):
	return self.__context
            
    def isInstanceOf( self, cname ):
    	return self.__class__.__name__ == cname and True or False
        
    def __repr__( self ):
        pretty_function = lambda f,v: v > 0 \
        			 and f \
				 or  re.sub( '\(.*\)', '(...)', f )
        
        s = '%s %s [%s at %s:%d] %s' % ( 
                        SeverityNames[self.severity],
        		time.strftime( '%Y-%b-%d %H:%M:%S', time.localtime( self.time ) ),
			pretty_function( self.__context.function_name, self.__verbosity ),
			self.__context.file_name,
			self.__context.line_number,
			self.message )
	if self.cause <> None:
            s += '\n\twas caused by: %s' % repr(self.cause)
        return s

    def __str__( self ):
        return self.message

class Message( Issue ):
    "this is wrapper for string mesages to be sent to debug, log and info streams"
    def __init__( self, msg ):
    	Issue.__init__( self, msg, {}, None )

import liberspy
liberspy.init( Issue )        
              
def debug( lvl, msg ):
    "sends msg to the debug stream"
    liberspy.debug( lvl, isinstance( msg, Issue ) and msg or Message( msg ) )

def log( msg ):
    "sends msg to the log stream"
    liberspy.log( isinstance( msg, Issue ) and msg or Message( msg ) )

def info( msg ):
    "sends msg to the information stream"
    liberspy.info( isinstance( msg, Issue ) and msg or Message( msg ) )

def warning( issue ):
    "sends issue to the warning stream"
    assert isinstance(issue,Issue), 'Only an instance of ers.Issue sub-class can be sent to the ers.warning stream'
    liberspy.warning( issue )

def error( issue ):
    "sends issue to the error stream"
    assert isinstance(issue,Issue), 'Only an instance of ers.Issue sub-class can be sent to the ers.error stream'
    liberspy.error( issue )

def fatal( issue ):
    "sends issue to the fatal stream"
    assert isinstance(issue,Issue), 'Only an instance of ers.Issue sub-class can be sent to the ers.fatal stream'
    liberspy.fatal( issue )

class LoggingHandler( logging.Handler ) :
    severity_mapper = { logging.getLevelName( logging.DEBUG )	: lambda m: debug( 0, m ),
			logging.getLevelName( logging.INFO )	: info,
			logging.getLevelName( logging.WARNING )	: warning,
			logging.getLevelName( logging.ERROR )	: error,
			logging.getLevelName( logging.CRITICAL ): fatal }
    def __init__(self):
	logging.Handler.__init__(self)
        
    def emit(self, record):
        if isinstance( record.msg, Issue ):
            self.severity_mapper[record.levelname]( record.msg )
        else:
            msg = Message( record.msg )
	    msg.context.function_name = record.funcName;
	    msg.context.file_name = record.filename
	    msg.context.line_number = record.lineno
            msg.context.package_name = record.module
            self.severity_mapper[record.levelname]( msg )

def addLoggingHandler( loggername = '' ):
    "adds ERS logging handler to the given logger"
    logging.getLogger( loggername ).addHandler( LoggingHandler() )

def replaceAllLoggingHandlers( loggername = '' ):
    """removes all logging handlers from the given logger 
    and adds ERS logging handler to it"""
    l = logging.getLogger( loggername )
    while len( l.handlers ) <> 0:
	l.removeHandler(l.handlers[0])
    addLoggingHandler( loggername )
