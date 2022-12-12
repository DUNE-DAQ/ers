/*
 *  RFilterStream.cxx
 *  ers
 *
 *  Created by Andrea Negri on 13.11.06 (from FilterStream.cxx)
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <ers/StreamFactory.hpp>
#include <ers/internal/RFilterStream.hpp>
#include <ers/internal/Util.hpp>

ERS_REGISTER_OUTPUT_STREAM(ers::RFilterStream, "rfilter", format)

namespace {
const char NOT = '!';
const char* const SEPARATORS = ",";
} // anonymous namespace

/** Constructor
 * \param chained the chained stream, which will be filtered. Only messages, which pass the filter
 * will go to the chained stream
 * on the stack and owned by the current object, i.e it will be deleted upon destruction
 * \param format describes filter expression.
 */
ers::RFilterStream::RFilterStream(const std::string& format)
{
  std::vector<std::string> tokens;
  ers::tokenize(format, SEPARATORS, tokens);
  for (size_t i = 0; i < tokens.size(); i++) {
    if (!tokens[i].empty() && tokens[i][0] == NOT)
      m_regExclude.push_back(boost::regex(tokens[i].substr(1)));
    else
      m_regInclude.push_back(boost::regex(tokens[i]));
  }
}

/** Filtering method
 * This method checks if an Issue is to be accepted or not.
 * \param issue_ptr the issue to check
 * \return \c true if the Issue passes filtering, \c false otherwise.
 */
bool
ers::RFilterStream::is_accepted(const ers::Issue& issue)
{
  // Get the function name that will be the input of the pattern search
  std::string s = issue.context().function_name();
  // Remove the function arguments, which could lead to fake matches
  s = s.substr(0, s.find_first_of('('));

  const std::vector<std::string>& qualifiers = issue.qualifiers();
  std::vector<boost::regex>::const_iterator it;

  // Check excluded matches
  for (it = m_regExclude.begin(); it != m_regExclude.end(); ++it) {
    // 1. Check against function name
    if (boost::regex_search(s, *it))
      return false;

    // 2. Check against qualifiers
    std::vector<std::string>::const_iterator ko;
    for (ko = qualifiers.begin(); ko < qualifiers.end(); ko++)
      if (boost::regex_search(*ko, *it))
        return false;
  }

  // Check included matches (function name, then qualifiers)
  for (it = m_regInclude.begin(); it != m_regInclude.end(); ++it) {
    // 1. Check against function name
    if (boost::regex_search(s, *it))
      return true;

    // 2. Check against qualifiers
    std::vector<std::string>::const_iterator ok;
    for (ok = qualifiers.begin(); ok < qualifiers.end(); ok++)
      if (boost::regex_search(*ok, *it))
        return true;
  }

  return false; // m_regInclude.empty();
}

/** Write method
 * basically calls \c is_accept to check if the issue is accepted.
 * If this is the case, the \c write method on the chained stream is called with
 * \c issue.
 * \param issue issue to be sent.
 */
void
ers::RFilterStream::write(const ers::Issue& issue)
{
  if (is_accepted(issue)) {
    chained().write(issue);
  }
} // send
