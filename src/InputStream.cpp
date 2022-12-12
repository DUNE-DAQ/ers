/*
 *  InputStream.cxx
 *  ers
 *
 *  Created by Serguei Kolos on 01.08.05.
 *  Copyright 2004 CERN. All rights reserved.
 *
 */

#include <ers/InputStream.hpp>

namespace {
struct DummyReceiver : public ers::IssueReceiver
{
  void receive(const ers::Issue&) override { ; }
};

DummyReceiver dummy_receiver;
}

ers::InputStream::InputStream()
  : m_receiver(&dummy_receiver)
{
  ;
}

void
ers::InputStream::receive(const Issue& issue)
{
  m_receiver->receive(issue);
}
