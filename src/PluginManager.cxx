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
    const char * const DefaultLibraryName = "ErsBaseStreams";
    const char * const MRSStreamLibraryName = "mtsStreams";
    const char * const EnvironmentName = "TDAQ_ERS_STREAM_LIBS";
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
        
	try
	{
	    libraries_[MRSStreamLibraryName] = new SharedLibrary( MRSStreamLibraryName );
	}
	catch( PluginException & ex )
	{
	    ERS_INTERNAL_DEBUG( 1, "Library " << MRSStreamLibraryName << " can not be loaded because " << ex.reason() )
	}
        
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
