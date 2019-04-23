#include <chrono>

#include <boost/noncopyable.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/python.hpp>
#include <boost/python/exception_translator.hpp>

#include <mutex>
#include <unordered_map>

#include <ers/ers.h>
#include <ers/AnyIssue.h>
#include <ers/RemoteContext.h>

namespace
{
    struct Py_RH : boost::noncopyable {
	Py_RH(PyObject * object) : m_object(object)
	{ ; }

	~Py_RH()
	{ Py_DECREF(m_object); }

	operator PyObject *() const { return m_object; }

    private:
	PyObject * m_object;
    };


    PyObject * base_ex_type;
    
    PyObject * 
    to_python( PyObject *& custom_ex_type, ers::Issue const & ex )
    {
        static const std::string cpp_separator( "::" );
	static const std::string py_separator( "." );
	static std::unordered_map<std::string, PyObject *> ex_types;
	static std::mutex types_mutex;
        
        std::string str = ex.get_class_name();
        boost::replace_all( str, cpp_separator, py_separator );
        
        {
            std::unique_lock<std::mutex> lock(types_mutex);
            std::unordered_map<std::string, PyObject *>::iterator it = ex_types.find(str);
	    if (it == ex_types.end()) {
		custom_ex_type = PyErr_NewException( (char*)str.c_str(), base_ex_type, 0 );
		ex_types[str] = custom_ex_type;
	    } else {
		custom_ex_type = it->second;
	    }
        }

	Py_RH args(PyTuple_New( 3 ));
	PyTuple_SetItem( args, 0, PyUnicode_FromString(ex.what()) );
	PyTuple_SetItem( args, 1, PyDict_New() );
    	if ( !ex.cause() )
        {
    	    Py_INCREF( Py_None );
    	    PyTuple_SetItem( args, 2, Py_None );
        }
	else
	{
	    PyObject * t = 0;
            PyTuple_SetItem( args, 2, to_python( t, *ex.cause() ) );
        }

    	PyObject * e = PyObject_CallObject( custom_ex_type, args );

        PyObject_SetAttrString( e, "severity", PyLong_FromLong((ers::severity)ex.severity()) );
        
        const std::vector<std::string> & q = ex.qualifiers();
        Py_RH qualifiers(PyList_New(q.size()));
	for ( size_t i = 0; i < q.size(); ++i )
	{
            PyList_SetItem( qualifiers, i, PyUnicode_FromString( q[i].c_str() ) );
	}
        PyObject_SetAttrString( e, "qualifiers", qualifiers );
        
        const ers::string_map & p = ex.parameters();
        Py_RH parameters(PyObject_GetAttrString( e, "__dict__" ));
	for ( ers::string_map::const_iterator it = p.begin(); it != p.end(); ++it )
        {
            PyDict_SetItem( 	parameters, 
            			PyUnicode_FromString( it -> first.c_str() ), 
                                PyUnicode_FromString( it -> second.c_str() ) );
        }

	Py_RH c(PyObject_GetAttrString( e, "_Issue__context" ));
        PyObject_SetAttrString( c, "package_name", PyUnicode_FromString(ex.context().package_name()) );
        PyObject_SetAttrString( c, "file_name", PyUnicode_FromString(ex.context().file_name()) );
        PyObject_SetAttrString( c, "function_name", PyUnicode_FromString(ex.context().function_name()) );
        PyObject_SetAttrString( c, "line_number", PyLong_FromLong(ex.context().line_number()) );
        
        return e;
    }
    
    void 
    translate_exception( ers::Issue const & ex )
    {
        PyObject * t = 0;
        PyObject * e = to_python( t, ex );
        PyErr_SetObject( t, e );
        Py_DECREF( e );
    }

    void 
    init( PyObject * o )
    {
	base_ex_type = o;
    }
    
    ers::Issue * 
    issue( PyObject * o )
    {
        if (!o)
            return 0;

	std::string id	= PyBytes_AsString(
	    Py_RH( PyObject_GetAttrString( PyObject_Type( o ), "__name__" ) ) );
	
	std::string msg = PyBytes_AsString( Py_RH( PyObject_GetAttrString( o, "message" ) ) );
        
	Py_RH c(PyObject_GetAttrString( o, "context" ));
	ers::RemoteContext context(
		    PyBytes_AsString( Py_RH( PyObject_GetAttrString( c, "package_name" ) ) ),
		    PyBytes_AsString( Py_RH( PyObject_GetAttrString( c, "file_name" ) ) ),
		    PyLong_AsLong( Py_RH( PyObject_GetAttrString( c, "line_number" ) ) ),
		    PyBytes_AsString( Py_RH( PyObject_GetAttrString( c, "function_name" ) ) ),
		    ers::RemoteProcessContext(
			    PyBytes_AsString( Py_RH( PyObject_GetAttrString( c, "host_name" ) ) ),
			    PyLong_AsLong( Py_RH( PyObject_GetAttrString( c, "process_id" ) ) ),
			    PyLong_AsLong( Py_RH( PyObject_GetAttrString( c, "thread_id" ) ) ),
			    PyBytes_AsString( Py_RH( PyObject_GetAttrString( c, "cwd" ) ) ),
			    PyLong_AsLong( Py_RH( PyObject_GetAttrString( c, "user_id" ) ) ),
			    PyBytes_AsString( Py_RH( PyObject_GetAttrString( c, "user_name" ) ) ),
                            PyBytes_AsString( Py_RH( PyObject_GetAttrString( c, "application_name" ) ) ) ) );

        std::vector<std::string> qualifiers;
        Py_RH q(PyObject_GetAttrString( o, "qualifiers" ));
        int size = PyList_Size( q );
        for (int i = 0; i < size; ++i ) {
            qualifiers.push_back(PyBytes_AsString(PyList_GetItem(q, i)));
        }

	std::map<std::string, std::string> parameters;
	Py_RH items(PyDict_Items(Py_RH(PyObject_GetAttrString(o, "parameters"))));
        size = PyList_Size( items );
        for (int i = 0; i < size; ++i ) {
            PyObject * item = PyList_GetItem(items, i);
            parameters.insert( std::make_pair(
        	PyBytes_AsString(PyTuple_GetItem(item, 0)),
        	PyBytes_AsString(PyTuple_GetItem(item, 1))
        	));
        }

	double t = PyFloat_AsDouble( Py_RH( PyObject_GetAttrString(o, "time")));
	std::chrono::system_clock::time_point
	    time(std::chrono::nanoseconds(static_cast<int64_t>(t*1000000000.)));

	Py_RH cause(PyObject_GetAttrString(o, "cause"));

	return new ers::AnyIssue(id, ers::Error, context, time, msg,
				    qualifiers, parameters,
				    cause != Py_None ? issue( cause ) : 0 );
    }
    
    void
    debug( int level, PyObject * o )
    {
    	std::unique_ptr<ers::Issue> a( issue( o ) );
        ers::debug( *a, level );
    }
    
    void
    log( PyObject * o )
    {
    	std::unique_ptr<ers::Issue> a( issue( o ) );
    	ers::log( *a );
    }
    
    void
    info( PyObject * o )
    {
    	std::unique_ptr<ers::Issue> a( issue( o ) );
    	ers::info( *a );
    }
    
    void
    warning( PyObject * o )
    {
    	std::unique_ptr<ers::Issue> a( issue( o ) );
    	ers::warning( *a );
    }
    
    void
    error( PyObject * o )
    {
    	std::unique_ptr<ers::Issue> a( issue( o ) );
    	ers::error( *a );
    }
    
    void
    fatal( PyObject * o )
    {
    	std::unique_ptr<ers::Issue> a( issue( o ) );
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
