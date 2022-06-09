#include "ers/AnyIssue.hpp"
#include "ers/Issue.hpp"
#include "ers/IssueFactory.hpp"
#include "ers/Context.hpp"
#include "ers/LocalContext.hpp"
#include "ers/Severity.hpp"

#include <exception>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;


PYBIND11_MODULE(_daq_ers_py, librespy) {

  
  // Class inheritence to be included
  // std::exception -> ers::Issue -> ers::AnyIssue
  // Abstract classes (like ers::Issue) cannot be called with
  // pybind11. Line 24 is only a decleration of ers::Issue.
  
  py::class_<std::exception>(librespy, "Exception");
  
  py::class_<ers::Issue, std::exception>(librespy, "Issue");
  
  py::class_<ers::AnyIssue, ers::Issue>(librespy, "AnyIssue")
    // constructor 1
    .def(py::init<const std::string &, const ers::Context &, 
	 const std::string &>())
    // constructor 2
    .def(py::init<const std::string &,
	 ers::Severity, const ers::Context &,
	 const system_clock::time_point &,const std::string &,
	 const std::vector<std::string> &, const std::map<std::string,
	 std::string> &, const ers::Issue *>())    
    .def("get_class_name", &ers::AnyIssue::get_class_name);

  // Same concept as mentioned in line 18 applies

  py::class_<ers::Context>(librespy, "Context");

  py::class_<ers::LocalContext, ers::Context>(librespy, "LocalContext")
    .def(py::init<const char *, const char *, int, const char *, bool>());
}
