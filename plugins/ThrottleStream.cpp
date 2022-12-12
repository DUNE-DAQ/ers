/*
 *  ThrottleStream.cxx
 *  ers
 *
 *  Created by Gordon Crone on 02.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */
#include <boost/lexical_cast.hpp>

#include <ers/StreamFactory.hpp>
#include <ers/internal/FilterStream.hpp>
#include <ers/internal/Util.hpp>

#include <ers/internal/ThrottleStream.hpp>

ERS_REGISTER_OUTPUT_STREAM(ers::ThrottleStream, "throttle", format)

ers::ThrottleStream::IssueRecord::IssueRecord()
{
  reset();
}

void
ers::ThrottleStream::IssueRecord::reset()
{
  m_lastOccurance = 0;
  m_lastReport = 0;
  m_initialCounter = 0;
  m_threshold = 10;
  m_suppressedCounter = 0;
}

void
ers::ThrottleStream::reportSuppression(IssueRecord& record, const ers::Issue& issue)
{
  std::ostringstream msgStream;
  msgStream << " -- " << record.m_suppressedCounter << " similar messages suppressed, last occurrence was at "
            << record.m_lastOccuranceFormatted;

  ers::Issue* suppressedNotice = issue.clone();
  suppressedNotice->wrap_message("", msgStream.str());

  suppressedNotice->set_severity(issue.severity());
  chained().write(*suppressedNotice);
  delete suppressedNotice;

  record.m_lastReport = issue.time_t();
  record.m_suppressedCounter = 0;
}

void
ers::ThrottleStream::throttle(IssueRecord& rec, const ers::Issue& issue)
{
  std::time_t issueTime = issue.time_t();
  bool reported = false;
  if (issueTime - rec.m_lastOccurance > m_timeLimit) {
    if (rec.m_suppressedCounter > 0) {
      reportSuppression(rec, issue);
      reported = true;
    }
    rec.reset();
  }

  if (rec.m_initialCounter < m_initialThreshold) {
    rec.m_initialCounter++;
    rec.m_lastReport = issueTime;
    if (!reported) {
      chained().write(issue);
    }
  } else if (rec.m_suppressedCounter >= rec.m_threshold) {
    rec.m_threshold = rec.m_threshold * 10;
    reportSuppression(rec, issue);
  } else if (issueTime - rec.m_lastReport > m_timeLimit) {
    reportSuppression(rec, issue);
  } else {
    rec.m_suppressedCounter++;
  }

  rec.m_lastOccurance = issueTime;
  rec.m_lastOccuranceFormatted = issue.time<std::chrono::microseconds>();
}

ers::ThrottleStream::ThrottleStream(const std::string& criteria)
{
  m_initialThreshold = 30;
  m_timeLimit = 30;

  std::vector<std::string> params;
  ers::tokenize(criteria, ",", params);

  if (params.size() > 0) {
    std::istringstream in(params[0]);
    in >> m_initialThreshold;
  }

  if (params.size() > 1) {
    std::istringstream in(params[1]);
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
ers::ThrottleStream::write(const ers::Issue& issue)
{
  const ers::Context& context = issue.context();
  std::string issueId = context.file_name() + boost::lexical_cast<std::string>(context.line_number());

  std::scoped_lock ml(m_mutex);
  throttle(m_issueMap[issueId], issue);
}
