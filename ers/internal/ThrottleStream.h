// -*- c++ -*-
// $Id$ 
//
// $Log$
// Revision 1.1  2008/01/25 17:32:14  kolos
// Add possibility to configure ERS streams dynamically, add Throttle and FormattedOutput streams.
//
// Revision 1.3  2007/09/28 13:00:54  gcrone
// Wrap existing Issue with suppresion text instead of creating a ThrottleIssue
//
// Revision 1.2  2007/03/09 16:09:12  gcrone
// Remove ROSException specific code and only output the summary Issue when suppressing, not the original
//
// Revision 1.1  2007/03/01 18:03:24  gcrone
// Added simple ERS rate throttling library
//
//
#ifndef ERS_THROTTLE_STREAM_H
#define ERS_THROTTLE_STREAM_H

#include <map>
#include <boost/thread/mutex.hpp>

#include <ers/OutputStream.h>

namespace ers
{
   class ThrottleStream : public ers::OutputStream
   {
     public:
      ThrottleStream(const std::string& criteria);
      void write(const ers::Issue& issue);
      
     private:
      
      class IssueRecord
      {
       public:
         IssueRecord();
         void reset();

         std::time_t m_lastOccurance;
         std::time_t m_lastReport;
         std::string m_lastOccuranceFormatted;
         int m_initialCounter;
         int m_threshold;
         int m_suppressedCounter;
      };

      typedef std::map<std::string,IssueRecord> IssueMap;
      IssueMap m_issueMap;

      int m_initialThreshold;
      int m_timeLimit;
      boost::mutex m_mutex;

      void throttle(IssueRecord& record, const ers::Issue& issue);
      void reportSuppression(IssueRecord& record, const ers::Issue& issue);
   };
}

#endif
