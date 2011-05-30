// $Id$
// //////////////////////////////////////////////////////////////////////
//  Simple filter to throttle messages sent to ERS
//
//  Author:  G.J.Crone
//
//  $Log$
//  Revision 1.4  2008/05/16 13:30:13  kolos
//  Fix memory leak in the IssueCatcher processing. Fix the bug of using uninitialized variable in the ThrottleStream.
//
//  Revision 1.3  2008/03/26 17:13:23  kolos
//  Remove debug output from the constructor of the Throttle stream.
//
//  Revision 1.2  2008/03/26 08:37:17  kolos
//  Change the way in which parameters are passed to the throttle stream.
//
//  Revision 1.1  2008/01/25 17:32:14  kolos
//  Add possibility to configure ERS streams dynamically, add Throttle and FormattedOutput streams.
//
//  Revision 1.6  2007/11/08 08:43:36  gcrone
//  Comment out debug cout
//
//  Revision 1.5  2007/09/28 13:00:54  gcrone
//  Wrap existing Issue with suppresion text instead of creating a ThrottleIssue
//
//  Revision 1.4  2007/03/13 15:10:09  gcrone
//  Issue summary message with Context of latest occurance rather than the
//  Context of ThrottleStream.
//
//  Revision 1.3  2007/03/09 16:09:12  gcrone
//  Remove ROSException specific code and only output the summary Issue when suppressing, not the original
//
//  Revision 1.2  2007/03/06 10:47:31  gcrone
//  Handle Issues which aren't ROSExceptions by using the file name and
//  line number retrieved from the ers::Context as the key.
//
//  Revision 1.1  2007/03/01 18:03:25  gcrone
//  Added simple ERS rate throttling library
//
//
// //////////////////////////////////////////////////////////////////////
#include <boost/lexical_cast.hpp>

#include <ers/internal/FilterStream.h>
#include <ers/internal/Util.h>
#include <ers/StreamFactory.h>

#include <ers/internal/ThrottleStream.h>

ERS_REGISTER_OUTPUT_STREAM( ers::ThrottleStream, "throttle", format )

ers::ThrottleStream::IssueRecord::IssueRecord()
{
    reset();
}

void 
ers::ThrottleStream::IssueRecord::reset()
{
    m_lastOccurance=0;
    m_initialCounter=0;
    m_threshold=10;
    m_suppressedCounter=0;
}

void 
ers::ThrottleStream::reportSuppression(IssueRecord& record, const ers::Issue& issue)
{
    std::ostringstream msgStream;
    msgStream << " -- " << record.m_suppressedCounter << " similar messages suppressed, last occurrence was at "
		<< record.m_lastOccuranceFormatted;
    
    ers::Issue* suppressedNotice = issue.clone();
    suppressedNotice->wrap_message( "",  msgStream.str());

    suppressedNotice->set_severity(issue.severity());
    chained().write(*suppressedNotice);
    delete suppressedNotice;

    record.m_lastReport = issue.time_t();
    record.m_suppressedCounter = 0;
}

void 
ers::ThrottleStream::throttle(IssueRecord& rec, const ers::Issue& issue)
{
    std::time_t issueTime=issue.time_t();
    bool reported=false;
    if (issueTime - rec.m_lastOccurance > m_timeLimit) {
	if (rec.m_suppressedCounter>0) {
	   reportSuppression(rec, issue);
	   reported=true;
	}
	rec.reset();
    }

    if (rec.m_initialCounter<m_initialThreshold) {
	rec.m_initialCounter++;
	rec.m_lastReport=issueTime;
	if (!reported) {
	    chained().write(issue);
	}
    }
    else if (rec.m_suppressedCounter>=rec.m_threshold) {
	rec.m_threshold=rec.m_threshold*10;
	reportSuppression(rec, issue);
    }
    else if (issueTime - rec.m_lastReport > m_timeLimit) {
	reportSuppression(rec, issue);
    }
    else {
	rec.m_suppressedCounter++;
    }

    rec.m_lastOccurance=issueTime;
    rec.m_lastOccuranceFormatted=issue.time();
}

ers::ThrottleStream::ThrottleStream( const std::string & criteria )
{
    m_initialThreshold = 30;
    m_timeLimit = 30;
    
    std::vector<std::string> params;
    ers::tokenize( criteria, ",", params );
    
    if ( params.size() > 0 )
    {
	std::istringstream in( params[0] );
        in >> m_initialThreshold;
    }
    
    if ( params.size() > 1 )
    {
	std::istringstream in( params[1] );
        in >> m_timeLimit;
    }
}

/** Write method 
  * basically calls \c throttle to check if the issue is accepted. 
  * If this is the case, the \c write method on the chained stream is called with 
  * \c issue. 
  * \param issue issue to be sent.
  */
void 
ers::ThrottleStream::write( const ers::Issue & issue )
{
    const ers::Context& context = issue.context();
    std::string issueId = context.file_name() + boost::lexical_cast<std::string>(context.line_number());

    boost::mutex::scoped_lock ml(m_mutex);
    throttle( m_issueMap[issueId], issue );
}
