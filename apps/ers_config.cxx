/*
 *  config.cxx
 *
 *  Created by Serguei Kolos on 24.01.05.
 *  Copyright 2005 CERN. All rights reserved.
 *
 */

#include <ers/ers.hpp>
#include <string.h>

/** \file config.cxx
 * Prints current configuration of all ERS streams,
 * taking into account environment variables.
 */

void
print_description()
{
  std::cout << "Description:" << std::endl;
  std::cout << "\tPrints ERS streams configuration in the current shell." << std::endl;
}

void
print_usage()
{
  std::cout << "Usage: ers_pc [-h]|[--help]" << std::endl;
  std::cout << "Options/Arguments:" << std::endl;
  std::cout << "\t[-h]|[--help]\tprints this help screen." << std::endl;
}

int
main(int argc, char** argv)
{
  if (argc > 1) {
    if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
      print_description();
    } else {
      print_usage();
    }
  } else {
    std::cout << ers::StreamManager::instance();
  }
  return 0;
}
