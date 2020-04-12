/*
 *  ThrottleStream.h
 *  ers
 *
 *  Created by Gordon Crone on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#ifndef ERS_THROTTLE_STREAM_H
#define ERS_THROTTLE_STREAM_H

#include <map>
#include <mutex>

#include <ers/OutputStream.h>

namespace ers
{
   class ThrottleStream : public ers::OutputStream
   {
     public:
      ThrottleStream( const std::string & criteria );
      
      void write( const ers::Issue & issue );
      
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
      std::mutex m_mutex;

      void throttle(IssueRecord& record, const ers::Issue& issue);
      void reportSuppression(IssueRecord& record, const ers::Issue& issue);
   };
}

#endif
