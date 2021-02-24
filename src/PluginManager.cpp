#ifndef __rtems__
#include <dlfcn.h>
#endif

#include <vector>

#include <ers/internal/Util.h>
#include <ers/internal/PluginManager.h>
#include <ers/internal/macro.h>

namespace
{
    const char * const SEPARATOR = ":";
    const char * const DefaultLibraryName = "ers_AbortStream_ersStream:ers_ExitStream_ersStream:ers_FilterStream_ersStream:ers_GlobalLockStream_ersStream:ers_LockStream_ersStream:ers_NullStream_ersStream:ers_RFilterStream_ersStream:ers_StandardStream_ersStream:ers_ThrottleStream_ersStream:ers_ThrowStream_ersStream";
    const char * const EnvironmentName = "DUNEDAQ_ERS_STREAM_LIBS";
}

namespace ers
{

    PluginManager::SharedLibrary::SharedLibrary( const std::string & name )
    {
#ifndef __rtems__
	std::string library = "lib" + name + ".so" ;

	handle_ = dlopen( library.c_str(), RTLD_LAZY|RTLD_GLOBAL );
	char * error = dlerror();

	if ( !handle_ )
	{
	    throw PluginException( error );
	}
#endif
    }

    PluginManager::SharedLibrary::~SharedLibrary( )
    {
	////////////////////////////////////////////////////////////////////
	// Library should be unloaded, but with some compilers (e.g gcc323)
	// this results in crash of program, because it is not recognized
	// that the library is still in use
	////////////////////////////////////////////////////////////////////

	//dlclose( handle_ );
    }

    PluginManager::~PluginManager()
    {
	LibMap::iterator it = libraries_.begin();
	for ( ; it != libraries_.end(); )
	{
	    delete it->second;
	    libraries_.erase( it++ );
	}
    }

    PluginManager::PluginManager( )
    {
	const char * env = ::getenv( EnvironmentName );
        std::string config( env ? std::string( env ) + SEPARATOR + DefaultLibraryName : DefaultLibraryName );
        
        std::vector<std::string> libs;
    	ers::tokenize( config, SEPARATOR, libs );
        
        
	for ( size_t i = 0; i < libs.size(); i++ )
	{
	    LibMap::iterator it = libraries_.find( libs[i] );
	    if ( it == libraries_.end() )
	    {
		try
                {
                    libraries_[libs[i]] = new SharedLibrary( libs[i] );
                }
                catch( PluginException & ex )
                {
		    ERS_INTERNAL_ERROR( "Library " << libs[i] << " can not be loaded because " << ex.reason() )
                }
	    }
	}
    }
}
