#include <boost/algorithm/string.hpp>
#include <boost/python.hpp>
#include <boost/python/exception_translator.hpp>

#include <ers/ers.h>
#include <ers/AnyIssue.h>
#include <ers/RemoteContext.h>

namespace
{
    PyObject * base_ex_type;
    
    PyObject * 
    to_python( PyObject *& custom_ex_type, ers::Issue const & ex )
    {
        static const std::string cpp_separator( "::" );
	static const std::string py_separator( "." );
        
        std::string str = ex.get_class_name();
        boost::replace_all( str, cpp_separator, py_separator );
        
	custom_ex_type = PyErr_NewException( (char*)str.c_str(), base_ex_type, 0 );

	PyObject * args = PyTuple_New( 3 );
	PyTuple_SetItem( args, 0, PyString_FromString(ex.what()) );
	PyTuple_SetItem( args, 1, PyDict_New() );
    	if ( !ex.cause() )
        {
	    PyTuple_SetItem( args, 2, Py_None );
        }
	else
	{
	    PyObject * t = 0;
            PyTuple_SetItem( args, 2, to_python( t, *ex.cause() ) );
        }

        PyObject * e = PyObject_CallObject( custom_ex_type, args );
        PyObject_SetAttrString( e, "severity", PyInt_FromLong((ers::severity)ex.severity()) );
        
        const std::vector<std::string> & q = ex.qualifiers();
	PyObject * qualifiers = PyList_New( q.size() );
	for ( size_t i = 0; i < q.size(); ++i )
	{
            PyList_SetItem( qualifiers, i, PyString_FromString( q[i].c_str() ) );
	}
        PyObject_SetAttrString( e, "qualifiers", qualifiers );
        
        const ers::string_map & p = ex.parameters();
	PyObject * parameters = PyObject_GetAttrString( e, "__dict__" );
	for ( ers::string_map::const_iterator it = p.begin(); it != p.end(); ++it )
        {
            PyDict_SetItem( 	parameters, 
            			PyString_FromString( it -> first.c_str() ), 
                                PyString_FromString( it -> second.c_str() ) );
        }
        
        PyObject * c = PyObject_GetAttrString( e, "_Issue__context" );
        PyObject_SetAttrString( c, "package_name", PyString_FromString(ex.context().package_name()) );
        PyObject_SetAttrString( c, "file_name", PyString_FromString(ex.context().file_name()) );
        PyObject_SetAttrString( c, "function_name", PyString_FromString(ex.context().function_name()) );
        PyObject_SetAttrString( c, "line_number", PyInt_FromLong(ex.context().line_number()) );
        
        return e;
    }
    
    void 
    translate_exception( ers::Issue const & ex )
    {
        PyObject * t = 0;
        PyObject * e = to_python( t, ex );
        PyErr_SetObject( t, e );
    }

    void 
    init( PyObject * o )
    {
	base_ex_type = o;
    }
    
    ers::Issue * 
    issue( PyObject * o )
    {
        std::string id	= PyString_AsString( 
        			PyObject_GetAttrString( 
                                	PyObject_Type( o ), "__name__" ) );
	
	std::string msg = PyString_AsString( PyObject_GetAttrString( o, "message" ) );
        
        PyObject * c = PyObject_GetAttrString( o, "context" );
	ers::RemoteContext context(
		    PyString_AsString( PyObject_GetAttrString( c, "package_name" ) ),
		    PyString_AsString( PyObject_GetAttrString( c, "file_name" ) ),
		    PyInt_AsLong( PyObject_GetAttrString( c, "line_number" ) ),
		    PyString_AsString( PyObject_GetAttrString( c, "function_name" ) ),
		    ers::RemoteProcessContext(
			    PyString_AsString( PyObject_GetAttrString( c, "host_name" ) ),
			    PyInt_AsLong( PyObject_GetAttrString( c, "process_id" ) ),
			    PyInt_AsLong( PyObject_GetAttrString( c, "thread_id" ) ),
			    PyString_AsString( PyObject_GetAttrString( c, "cwd" ) ),
			    PyInt_AsLong( PyObject_GetAttrString( c, "user_id" ) ),
			    PyString_AsString( PyObject_GetAttrString( c, "user_name" ) ),
                            PyString_AsString( PyObject_GetAttrString( c, "application_name" ) ) ) );
                
        PyObject * cause = PyObject_GetAttrString( o, "cause" );
        if ( cause != Py_None )
        {
	    std::vector<std::string> qualifiers;
	    std::map<std::string, std::string> parameters;
	    return new ers::AnyIssue( id, ers::Error, context, boost::posix_time::ptime(),
					msg, qualifiers, parameters, issue( cause ) );
        }
	
        return new ers::AnyIssue( id, context, msg );
    }
    
    void
    debug( int level, PyObject * o )
    {
    	std::auto_ptr<ers::Issue> a( issue( o ) );
        ers::debug( *a, level );
    }
    
    void
    log( PyObject * o )
    {
    	std::auto_ptr<ers::Issue> a( issue( o ) );
    	ers::log( *a );
    }
    
    void
    info( PyObject * o )
    {
    	std::auto_ptr<ers::Issue> a( issue( o ) );
    	ers::info( *a );
    }
    
    void
    warning( PyObject * o )
    {
    	std::auto_ptr<ers::Issue> a( issue( o ) );
    	ers::warning( *a );
    }
    
    void
    error( PyObject * o )
    {
    	std::auto_ptr<ers::Issue> a( issue( o ) );
    	ers::error( *a );
    }
    
    void
    fatal( PyObject * o )
    {
    	std::auto_ptr<ers::Issue> a( issue( o ) );
    	ers::fatal( *a );
    }

    BOOST_PYTHON_MODULE(liberspy)
    {
	boost::python::register_exception_translator<ers::Issue>(&translate_exception);
	boost::python::def("init", &init);
	boost::python::def("debug", &debug);
	boost::python::def("log", &log);
	boost::python::def("info", &info);
	boost::python::def("warning", &warning);
	boost::python::def("error", &error);
	boost::python::def("fatal", &fatal);
    }
}
