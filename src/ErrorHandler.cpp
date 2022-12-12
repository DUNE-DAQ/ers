/*
 *  ErrorHandler.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 02.08.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */
#include <csignal>
#include <iomanip>
#include <iostream>
#include <map>

#ifdef ERS_NO_DEBUG
#undef ERS_NO_DEBUG
#endif

#include <ers/Issue.hpp>
#include <ers/StandardStreamOutput.hpp>
#include <ers/ers.hpp>

ERS_DECLARE_ISSUE(ers, UnhandledException, "Unhandled '" << name << "' exception has been thrown", ((const char*)name))

ERS_DECLARE_ISSUE(ers, SignalCatched, "Got signal " << signum << " " << name, ((int)signum)((const char*)name))

namespace ers {
class ErrorHandler
{
  class SignalHandler
  {
    int signal_;
    std::string name_;
    struct sigaction old_action_;

    static void action(int, siginfo_t*, void*);

  public:
    SignalHandler(int signal, const char* sig_name)
      : signal_(signal)
      , name_(sig_name)
    {
      struct sigaction sa;

      sa.sa_sigaction = action;
      sigemptyset(&sa.sa_mask);
      sa.sa_flags = SA_SIGINFO;

      ::sigaction(signal_, &sa, &old_action_);
    }

    ~SignalHandler() { ::sigaction(signal_, &old_action_, 0); }
  };

public:
  friend class SignalHandler;

  ErrorHandler();
  ~ErrorHandler();

private:
  static void abort(const ers::Issue& issue);
  static void terminate_handler();

  static std::map<int, SignalHandler*> handlers;
};

void
ErrorHandler::SignalHandler::action(int signal, siginfo_t*, void* ucontext)
{
  static bool recursive_invocation = false;
  if (recursive_invocation) {
    std::cerr << "Got signal " << signal << " " << handlers[signal]->name_ << ", aborting the program ..." << std::endl;
    ::abort();
  }
  recursive_invocation = true;

  ers::SignalCatched ex(ERS_HERE_DEBUG, signal, handlers[signal]->name_.c_str());
#ifdef __x86_64__
  if (ex.context().stack_size() > 1) {
    /* overwrite sigaction with caller's address */
    ucontext_t* uc = (ucontext_t*)ucontext;
    const_cast<void**>(ex.context().stack_symbols())[1] = (void*)uc->uc_mcontext.gregs[REG_RIP];
  }
#endif
  ErrorHandler::abort(ex);
}

ErrorHandler::ErrorHandler()
{
  if (!::getenv("DUNEDAQ_ERS_NO_SIGNAL_HANDLERS")) {
    handlers[SIGSEGV] = new SignalHandler(SIGSEGV, "Segmentation fault (invalid memory reference)");
    handlers[SIGBUS] = new SignalHandler(SIGBUS, "Bus error (bad memory access)");
    handlers[SIGILL] = new SignalHandler(SIGILL, "Illegal Instruction");
    handlers[SIGFPE] = new SignalHandler(SIGFPE, "Floating point exception");
    std::set_terminate(terminate_handler);
    std::set_unexpected(terminate_handler);
  }
}

ErrorHandler::~ErrorHandler()
{
  std::map<int, SignalHandler*>::iterator it;
  for (it = handlers.begin(); it != handlers.end(); ++it) {
    delete it->second;
  }
}

void
ErrorHandler::terminate_handler()
{
  static bool recursive_invocation = false;
  if (recursive_invocation) {
    std::cerr << "Unhandled exception has been thrown, aborting the program ..." << std::endl;
    ::abort();
  }
  recursive_invocation = true;

  try {
    throw;
  } catch (ers::Issue& ex) {
    ErrorHandler::abort(UnhandledException(ERS_HERE_DEBUG, ex.get_class_name(), ex));
  } catch (std::exception& ex) {
    ErrorHandler::abort(UnhandledException(ERS_HERE_DEBUG, "std::exception", ex));
  } catch (...) {
    ErrorHandler::abort(UnhandledException(ERS_HERE_DEBUG, "unknown"));
  }
}

void
ErrorHandler::abort(const ers::Issue& issue)
{
  StandardStreamOutput::println(std::cerr, issue, 13);
  ::abort();
}
}

std::map<int, ers::ErrorHandler::SignalHandler*> ers::ErrorHandler::handlers;

namespace {
ers::ErrorHandler __handler__;
}
