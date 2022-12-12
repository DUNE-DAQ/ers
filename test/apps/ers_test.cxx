/*
 *  test.cxx
 *  Test
 *
 *  Created by Matthias Wiesmann on 24.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <ers/OutputStream.hpp>
#include <ers/SampleIssues.hpp>
#include <ers/StreamManager.hpp>

#include <boost/lexical_cast.hpp>
#include <csignal>
#include <ers/ers.hpp>
#include <stdexcept>
#include <thread>

ERS_DECLARE_ISSUE(ers, ExceptionTest, "Exception test", ERS_EMPTY)
struct Test
{
  void pass(int step)
  {
    ERS_RANGE_CHECK(0, step, 8);

    switch (step) {
      case 1: {
        ers::PermissionDenied issue(ERS_HERE, "foo2", 0x777);
        throw issue;
      }
      case 2: {
        ers::FileDoesNotExist issue(ERS_HERE, "foo3");
        throw issue;
      }
      case 3: {
        ers::FileDoesNotExist issue(ERS_HERE, "foo3");
        issue.add_qualifier("ers_test");
        throw issue;
      }
      case 4: {
        throw ers::CantOpenFile(ERS_HERE, "foo3");
      }
      case 5: {
        throw std::runtime_error("std::out_of_range error");
      }
      case 6: {
        ERS_ASSERT_MSG(step <= 6, "ERS_ASSERT_MSG is broken");
      } break;
      case 7: {
        ERS_ASSERT_MSG(step > 6, "ERS_ASSERT_MSG is broken");
      } break;
      default: {
        ers::info(ers::ExceptionTest(ERS_HERE));
        struct UnhandledException
        {};
        throw UnhandledException();
      }
    }
  }
};

void
test_function(int /* index */)
{
  usleep(10000);
  ers::error(ers::FileDoesNotExist(ERS_HERE, "error file"));
  usleep(10000);
  ers::fatal(ers::FileDoesNotExist(ERS_HERE, "fatal file"));
  usleep(10000);
  ers::warning(ers::FileDoesNotExist(ERS_HERE, "warning file"));
}

struct IssueCatcher
{
  void handler(const ers::Issue& issue)
  {
    std::cout << "IssueCatcher has been called for the following issue:" << std::endl;
    ers::error(issue);
  }
};

void
test_local_catcher()
{
  IssueCatcher catcher;
  std::unique_ptr<ers::IssueCatcherHandler> handler;
  try {
    handler.reset(ers::set_issue_catcher(std::bind(&IssueCatcher::handler, &catcher, std::placeholders::_1)));
  } catch (ers::IssueCatcherAlreadySet& ex) {
    ers::fatal(ex);
    return;
  }

  ers::CantOpenFile issue(ERS_HERE, "TEST");
  ers::warning(issue);
  ers::error(issue);
  ers::fatal(issue);
}

int
main(int ac, char** av)
{
  test_function(0);
  test_function(0);

  test_local_catcher();

  IssueCatcher catcher;
  std::unique_ptr<ers::IssueCatcherHandler> handler;
  try {
    handler.reset(ers::set_issue_catcher(std::bind(&IssueCatcher::handler, &catcher, std::placeholders::_1)));
  } catch (ers::IssueCatcherAlreadySet& ex) {
    ers::fatal(ex);
    return 1;
  }

  test_local_catcher();

  std::thread thr1(std::bind(test_function, 1));
  std::thread thr2(std::bind(test_function, 2));
  std::thread thr3(std::bind(test_function, 3));
  std::thread thr4(std::bind(test_function, 4));
  thr1.join();
  thr2.join();
  thr3.join();
  thr4.join();

  test_function(0);
  test_function(0);

  int steps = ac > 1 ? boost::lexical_cast<int>(av[1]) : 9;
  Test test;
  for (int step = 1; step < steps; ++step) {
    try {
      test.pass(step);
      usleep(100000);
    } catch (ers::PermissionDenied& ex) {
      ers::CantOpenFile issue(ERS_HERE, ex.get_file_name(), ex);
      issue.add_qualifier("q1");
      ers::warning(issue);
    } catch (ers::FileDoesNotExist& ex) {
      ers::CantOpenFile issue(ERS_HERE, ex.get_file_name(), ex);
      issue.add_qualifier("q2");
      ers::warning(issue);
    } catch (ers::Issue& ex) {
      ers::error(ex);
    } catch (std::exception& ex) {
      ers::CantOpenFile issue(ERS_HERE, "unknown", ex);
      issue.add_qualifier("q3");
      ers::warning(issue);
      handler.reset();
    }
  }
  return 0;
}
