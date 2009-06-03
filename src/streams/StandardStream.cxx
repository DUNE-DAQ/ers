/*
 *  StandardStream.cxx
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
#include <ers/internal/FormattedStandardStream.h>
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>

namespace
{
    struct OutDevice : boost::noncopyable
    {
	OutDevice( std::ostream & out )
          : out_( out )
	{ ; }
        
        std::ostream & stream() const
        { return out_; }
        
        const OutDevice & device()
        { return *this; }
      
      private:        
      	std::ostream &	out_;
    };
    
    struct ObjectLock : boost::noncopyable
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
    struct ClassLock : boost::noncopyable
    {
      protected:
	boost::mutex & mutex()
        {
            static boost::mutex * mutex_ = new boost::mutex;
            return *mutex_;
        }
    };
    
    template <class L=ObjectLock>
    struct LockableDevice : public L,
    			    public OutDevice
    {
	using L::mutex;
        
        struct Lock : public boost::mutex::scoped_lock,
		      public OutDevice
	{            
            Lock( std::ostream & out, boost::mutex & mutex )
	      : boost::mutex::scoped_lock( mutex ),
		OutDevice( out )
	    { ; }

	    // this constructor is not implemented because it must never be called !!!
	    // nevertheless removing this declaration causes compilation error
	    Lock( const Lock & );
            
          private:
	    Lock & operator=( const Lock & );
	};
        
	LockableDevice( std::ostream & out )
	  : OutDevice( out )
	{ ; }
        
        Lock device()
        {
	    return Lock( stream(), mutex() );
        }
    };
        
    
    template <class D>
    struct OutputDevice : public D
    {
	OutputDevice( const std::string & = "" ) 
	  : D( std::cout )
	{ ; }
    };
    
    template <class D>
    struct ErrorDevice : public D
    {
	ErrorDevice( const std::string & = "" )
	  : D( std::cerr )
	{ ; }
    };
    
    template <class D>
    struct FileDevice : public D
    {
	FileDevice( const std::string & file_name )
          : D( out_ ),
            out_( file_name.c_str() )
	{
            if ( !out_ )
            {
            	throw ers::CantOpenFile( ERS_HERE, file_name.c_str() );
            }
        }
                
      private:
      	std::ofstream out_;
    };
}

ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<FileDevice<OutDevice> >, "file", file_name )
ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<OutputDevice<OutDevice> >, "stdout", )
ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<ErrorDevice<OutDevice> >, "stderr", )

ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<FileDevice<LockableDevice<> > >, "lfile", file_name )
ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<OutputDevice<LockableDevice<ClassLock<1> > > >, "lstdout", )
ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<ErrorDevice<LockableDevice<ClassLock<2> > > >, "lstderr", )

ERS_REGISTER_OUTPUT_STREAM( ers::FormattedStandardStream<FileDevice<OutDevice> >, "ffile", format )
ERS_REGISTER_OUTPUT_STREAM( ers::FormattedStandardStream<OutputDevice<OutDevice> >, "fstdout", format )
ERS_REGISTER_OUTPUT_STREAM( ers::FormattedStandardStream<ErrorDevice<OutDevice> >, "fstderr", format )

ERS_REGISTER_OUTPUT_STREAM( ers::FormattedStandardStream<FileDevice<LockableDevice<> > >, "lffile", format )
ERS_REGISTER_OUTPUT_STREAM( ers::FormattedStandardStream<OutputDevice<LockableDevice<ClassLock<1> > > >, "lfstdout", format )
ERS_REGISTER_OUTPUT_STREAM( ers::FormattedStandardStream<ErrorDevice<LockableDevice<ClassLock<2> > > >, "lfstderr", format )
