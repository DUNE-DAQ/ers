/*
 *  StandardErrStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <ers/ExampleIssues.h>
#include <ers/internal/StandardStream.h>
#include <boost/thread/mutex.hpp>

namespace
{
    struct NonLockableDevice
    {
	NonLockableDevice( std::ostream & out )
          : out_( out )
	{ ; }
        
        std::ostream & stream() const
        { return out_; }
        
        const NonLockableDevice & device()
        { return *this; }
      
      private:
        NonLockableDevice & operator=( const NonLockableDevice & );
        
      	std::ostream &	out_;
    };
    
    struct ObjectLock
    {
      protected:
	boost::mutex & mutex()
        {
            return mutex_;
        }
        
      private:
        boost::mutex	mutex_;
    };
    
    template <int LockDiscriminator>
    struct ClassLock
    {
      protected:
	boost::mutex & mutex()
        {
            static boost::mutex	mutex_;
            return mutex_;
        }
    };
    
    template <class ScopedLock=ObjectLock>
    struct LockableDevice : public ScopedLock,
    			    public NonLockableDevice
    {
	using ScopedLock::mutex;
        
        struct Lock : public boost::mutex::scoped_lock,
		      public NonLockableDevice
	{            
            Lock( std::ostream & out, boost::mutex & mutex )
	      : boost::mutex::scoped_lock( mutex ),
		NonLockableDevice( out )
	    { ; }

	    // this constructor is not implemented because it is must never be called !!!
	    // nevertheless removing this declaration causes compilation error
	    Lock( const Lock & );
            
	  private:
	    Lock & operator=( const Lock & );
	};
        
	LockableDevice( std::ostream & out )
	  : NonLockableDevice( out )
	{ ; }
        
        Lock device()
        {
	    return Lock( stream(), mutex() );
        }
    };
        
    
    template <class Device>
    struct OutputDevice : public Device
    {
	OutputDevice() 
	  : Device( std::cout )
	{ ; }
    };
    
    template <class Device>
    struct ErrorDevice : public Device
    {
	ErrorDevice()
	  : Device( std::cerr )
	{ ; }
    };
    
    template <class Device>
    struct FileDevice : public Device
    {
	FileDevice( const std::string & file_name )
          : Device( out_ ),
            out_( file_name.c_str() )
	{
            if ( !out_ )
            {
            	throw ers::FileDoesNotExist( ERS_HERE, file_name.c_str() );
            }
        }
                
      private:
      	std::ofstream out_;
    };
}

ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<FileDevice<NonLockableDevice> >, "file", file_name )
ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<OutputDevice<NonLockableDevice> >, "stdout", )
ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<ErrorDevice<NonLockableDevice> >, "stderr", )

ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<FileDevice<LockableDevice<> > >, "lfile", file_name )
ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<OutputDevice<LockableDevice<ClassLock<1> > > >, "lstdout", )
ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<ErrorDevice<LockableDevice<ClassLock<2> > > >, "lstderr", )
