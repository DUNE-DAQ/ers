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
#include <mutex>

#include <ers/SampleIssues.h>

#include <ers/internal/StandardStream.h>
#include <ers/internal/FormattedStandardStream.h>

namespace
{
    struct OutDevice
    {
	explicit OutDevice( std::ostream & out )
          : out_( out )
	{ ; }
        
        std::ostream & stream() const
        { return out_; }
        
        const OutDevice & device()
        { return *this; }
      
      private:
        OutDevice(const OutDevice &) = delete;
        OutDevice & operator=(const OutDevice &) = delete;

      private:
      	std::ostream &	out_;
    };
    
    struct ObjectLock
    {
      protected:
        ObjectLock() = default;

	std::mutex & mutex()
        {
            return mutex_;
        }
        
      private:
	ObjectLock(const ObjectLock &) = delete;
	ObjectLock & operator=(const ObjectLock &) = delete;

      private:
        std::mutex	mutex_;
    };
    
    template <int LockDiscriminator>
    struct ClassLock
    {
      protected:
        ClassLock() = default;

	std::mutex & mutex()
        {
            static std::mutex * m = new std::mutex;
            return *m;
        }

      private:
	ClassLock(const ClassLock &) = delete;
	ClassLock & operator=(const ClassLock &) = delete;
    };
    
    template <class L=ObjectLock>
    struct LockableDevice : public L,
    			    public OutDevice
    {
	using L::mutex;

        struct LockedDevice : public OutDevice
	{            
	    LockedDevice( std::ostream & out, std::mutex & mutex )
	      : OutDevice( out ),
	        m_lock(mutex)
	    { ; }

        private:
	    LockedDevice( const LockedDevice & ) = delete;
	    LockedDevice & operator=( const LockedDevice & ) = delete;

        private:
	    std::unique_lock<std::mutex> m_lock;
	};
        
	LockableDevice( std::ostream & out )
	  : OutDevice( out )
	{ ; }
        
	LockedDevice device()
        {
	    return LockedDevice( stream(), mutex() );
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
ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<OutputDevice<OutDevice> >, "stdout", ERS_EMPTY)
ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<ErrorDevice<OutDevice> >, "stderr", ERS_EMPTY)

ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<FileDevice<LockableDevice<> > >, "lfile", file_name )
ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<OutputDevice<LockableDevice<ClassLock<1> > > >, "lstdout", ERS_EMPTY)
ERS_REGISTER_OUTPUT_STREAM( ers::StandardStream<ErrorDevice<LockableDevice<ClassLock<2> > > >, "lstderr", ERS_EMPTY)

ERS_REGISTER_OUTPUT_STREAM( ers::FormattedStandardStream<FileDevice<OutDevice> >, "ffile", format )
ERS_REGISTER_OUTPUT_STREAM( ers::FormattedStandardStream<OutputDevice<OutDevice> >, "fstdout", format )
ERS_REGISTER_OUTPUT_STREAM( ers::FormattedStandardStream<ErrorDevice<OutDevice> >, "fstderr", format )

ERS_REGISTER_OUTPUT_STREAM( ers::FormattedStandardStream<FileDevice<LockableDevice<> > >, "lffile", format )
ERS_REGISTER_OUTPUT_STREAM( ers::FormattedStandardStream<OutputDevice<LockableDevice<ClassLock<1> > > >, "lfstdout", format )
ERS_REGISTER_OUTPUT_STREAM( ers::FormattedStandardStream<ErrorDevice<LockableDevice<ClassLock<2> > > >, "lfstderr", format )
