#include "ers/AnyIssue.hpp"
#include "ers/Issue.hpp"
#include "ers/IssueFactory.hpp"
#include "ers/Context.hpp"
#include "ers/LocalContext.hpp"
#include "ers/RemoteContext.hpp"
#include "ers/Severity.hpp"
#include "ers/ers.hpp"

#include <exception>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;


PYBIND11_MODULE(_daq_ers_py, liberspy) {

  
  // Class inheritence to be included
  // std::exception -> ers::Issue -> ers::AnyIssue
  // Abstract classes (like ers::Issue) cannot be called with
  // pybind11. Line 24 is only a decleration of ers::Issue.
  
  py::class_<std::exception>(liberspy, "STDException");
  
  py::class_<ers::Issue, std::exception>(liberspy, "Issue");
  
  py::class_<ers::AnyIssue, ers::Issue>(liberspy, "AnyIssue")
    // constructor 1
    .def(py::init<const std::string &, const ers::Context &, 
	 const std::string &>())
    // constructor 2
    .def(py::init<const std::string &,
	 ers::Severity, const ers::Context &,
	 const system_clock::time_point &,const std::string &,
	 const std::vector<std::string> &, const std::map<std::string,
	 std::string> &, const ers::Issue *>())    
    .def("get_class_name", &ers::AnyIssue::get_class_name)
    .def("raise", &ers::AnyIssue::raise)
    .def("what", &ers::AnyIssue::what)
    .def("cause", &ers::AnyIssue::cause)
    .def("severity", &ers::AnyIssue::severity)
    .def("qualifiers",&ers::AnyIssue::qualifiers)
    .def("parameters", &ers::AnyIssue::parameters);

  // Same concept as mentioned in line 18 applies

  py::class_<ers::Context>(liberspy, "Context");

  // And now some concrete classes deriving from Context
  py::class_<ers::LocalContext, ers::Context>(liberspy, "LocalContext")
    .def(py::init<const char *, const char *, int, const char *, bool>())
    .def("process_id", &ers::LocalContext::process_id)
    .def("cwd", &ers::LocalContext::cwd)
    .def("file_name", &ers::LocalContext::file_name)
    .def("function_name", &ers::LocalContext::function_name)
    .def("host_name", &ers::LocalContext::host_name)
    .def("line_number", &ers::LocalContext::line_number)
    .def("package_name", &ers::LocalContext::package_name)
    .def("process_id", &ers::LocalContext::process_id)
    .def("thread_id", &ers::LocalContext::thread_id)
    .def("stack_symbols", &ers::LocalContext::stack_symbols)
    .def("line_number", &ers::LocalContext::stack_size)
    .def("user_id", &ers::LocalContext::user_id)
    .def("user_name", &ers::LocalContext::user_name)
    .def("application_name", &ers::LocalContext::application_name);
    //   .def("resetProcessContext", &ers::LocalContext::resetProcessContext);
  
  py::class_<ers::RemoteProcessContext>(liberspy, "RemoteProcessContext")
    .def(py::init<const std::string &, int, int, const std::string &, int, const std::string &, const std::string &>());

  py::class_<ers::RemoteContext, ers::Context>(liberspy, "RemoteContext")
    .def(py::init<const std::string &, const std::string &, int, const std::string &, ers::RemoteProcessContext &>());

  // Severity includes both a class and and enum. 
  py::class_<ers::Severity>(liberspy,"Severity")
    .def(py::init<ers::severity &, int &>())
    .def_readwrite("type", &ers::Severity::type)
    .def_readwrite("rank", &ers::Severity::rank);
  py::enum_<ers::severity>(liberspy,"severity")
    .value("Debug",ers::severity::Debug)
    .value("Log",ers::severity::Log)
    .value("Information",ers::severity::Information)
    .value("Warning",ers::severity::Warning)
    .value("Error",ers::severity::Error)
    .value("Fatal",ers::severity::Fatal)
    .export_values();

  // Bind the various logging calls
  liberspy.def("debug", &ers::debug, "sends issue to the debug stream", py::arg("issue"), py::arg("level") );
  liberspy.def("log", &ers::log, "sends issue to the log stream", py::arg("issue"));
  liberspy.def("info", &ers::info, "sends issue to the information stream", py::arg("issue"));
  liberspy.def("warning", &ers::warning,  "sends issue to the warning stream", py::arg("issue"));
  liberspy.def("error", &ers::error, "sends issue to the error stream", py::arg("issue"));
  liberspy.def("fatal", &ers::fatal, "sends issue to the fatal stream", py::arg("issue"));
  liberspy.def("debug_level", &ers::debug_level, "returns current debug level for ERS");
  liberspy.def("verbosity_level", &ers::verbosity_level, "returns current verbosity level for ERS");
  liberspy.def("enable_core_dump", &ers::enable_core_dump, "does what the name implies");
}
