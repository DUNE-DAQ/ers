#include "ers/AnyIssue.hpp"
#include "ers/Issue.hpp"
#include "ers/IssueFactory.hpp"
#include "ers/Context.hpp"
#include "ers/LocalContext.hpp"
#include "ers/RemoteContext.hpp"
#include "ers/Severity.hpp"
#include "ers/ers.hpp"
#include "ers/OutputStream.hpp"

#include <exception>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;


PYBIND11_MODULE(_daq_ers_py, module) {

  
  // Class inheritence to be included
  // std::exception -> ers::Issue -> ers::AnyIssue
  // Abstract classes (like ers::Issue) cannot be called with
  // pybind11. Line below is only a decleration of ers::Issue.
  
  py::class_<std::exception>(module, "STDException");
  
  py::class_<ers::Issue, std::exception>(module, "Issue");

  py::class_<ers::OutputStream>(module, "OutputStream");
  
  // Concrete Issue class 
  py::class_<ers::AnyIssue, ers::Issue>(module, "AnyIssue")
   // constructor 1
   .def(py::init<const std::string &, const ers::Context &, 
        const std::string &>())
   // constructor 2
   .def(py::init<const std::string &,
        ers::Severity, const ers::Context &,
	const system_clock::time_point &,
	const std::string &,
	const std::vector<std::string> &,
	const std::vector<std::string> &,
	const std::map<std::string, std::string> &,
	const ers::Issue *>())    
   .def("get_class_name", &ers::AnyIssue::get_class_name)
   .def("raise", &ers::AnyIssue::raise)
   .def("what", &ers::AnyIssue::what)
   .def("cause", &ers::AnyIssue::cause)
   .def("severity", &ers::AnyIssue::severity)
   .def("qualifiers",&ers::AnyIssue::qualifiers)
   .def("parameters", &ers::AnyIssue::parameters);

  // Bindings for Context base class and derived classes

  py::class_<ers::Context>(module, "Context")
    .def("cwd", &ers::Context::cwd)
    .def("function_name", &ers::Context::function_name)
    .def("file_name", &ers::Context::file_name)
    .def("host_name", &ers::Context::host_name)
    .def("package_name", &ers::Context::package_name)
    .def("user_name", &ers::Context::user_name)
    .def("application_name", &ers::Context::application_name)
    .def("user_id", &ers::Context::user_id)
    .def("user_name", &ers::Context::user_name)
    .def("stack_size", &ers::Context::stack_size)
    .def("stack_symbols", &ers::Context::stack_symbols)
    .def("line_number", &ers::Context::line_number)
    .def("process_id", &ers::Context::process_id)
    .def("thread_id", &ers::Context::thread_id);

  // And now some concrete classes deriving from Context
  py::class_<ers::LocalContext, ers::Context>(module, "LocalContext")
    .def(py::init<const char *, const char *, int, const char *, bool>());
  
  py::class_<ers::RemoteProcessContext>(module, "RemoteProcessContext")
    .def(py::init<const std::string &, int, int, const std::string &, int, const std::string &, const std::string &>());

  py::class_<ers::RemoteContext, ers::Context>(module, "RemoteContext")
    .def(py::init<const std::string &, const std::string &, int, const std::string &, ers::RemoteProcessContext &>());

  // Severity includes both a class and and enum. 
  py::class_<ers::Severity>(module,"Severity")
    .def(py::init<ers::severity &, int &>())
    .def_readwrite("type", &ers::Severity::type)
    .def_readwrite("rank", &ers::Severity::rank);
  py::enum_<ers::severity>(module,"severity")
    .value("Debug",ers::severity::Debug)
    .value("Log",ers::severity::Log)
    .value("Information",ers::severity::Information)
    .value("Warning",ers::severity::Warning)
    .value("Error",ers::severity::Error)
    .value("Fatal",ers::severity::Fatal)
    .export_values();

  // Bind the various logging calls
  module.def("debug", &ers::debug, "sends issue to the debug stream", py::arg("issue"), py::arg("level") );
  module.def("log", &ers::log, "sends issue to the log stream", py::arg("issue"));
  module.def("info", &ers::info, "sends issue to the information stream", py::arg("issue"));
  module.def("warning", &ers::warning,  "sends issue to the warning stream", py::arg("issue"));
  module.def("error", &ers::error, "sends issue to the error stream", py::arg("issue"));
  module.def("fatal", &ers::fatal, "sends issue to the fatal stream", py::arg("issue"));
  module.def("debug_level", &ers::debug_level, "returns current debug level for ERS");
  module.def("verbosity_level", &ers::verbosity_level, "returns current verbosity level for ERS");
  module.def("enable_core_dump", &ers::enable_core_dump, "does what the name implies");
}
