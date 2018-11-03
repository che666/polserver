#ifndef CLIB_STREAMSAVER_H
#define CLIB_STREAMSAVER_H

#include <boost/noncopyable.hpp>
#include <iosfwd>
#include <memory>
#include <string>
#include <thread>

#include <format/format.h>
#include "message_queue.h"
#if 0
#include "timer.h"
#endif

namespace Pol
{
namespace Clib
{
class StreamWriter : boost::noncopyable
{
public:
  StreamWriter();
  virtual ~StreamWriter() = default;
  fmt::Writer& operator()();
  virtual void init( const std::string& filepath ) = 0;
  virtual void flush() = 0;
  virtual void flush_file() = 0;

protected:
  std::unique_ptr<fmt::Writer> _writer;
};

class FMTStreamWriter final : public StreamWriter
{
public:
  FMTStreamWriter() = default;
  virtual ~FMTStreamWriter() = default;
  virtual void init( const std::string& ) override{};  // argument is not used
  virtual void flush() override{};
  virtual void flush_file() override{};
};

class OFStreamWriter final : public StreamWriter
{
public:
  OFStreamWriter();
  OFStreamWriter( std::ofstream* stream );
  virtual ~OFStreamWriter();
  virtual void init( const std::string& filepath ) override;
  virtual void flush() override;
  virtual void flush_file() override;

private:
  std::ofstream* _stream;
#if 0
      Tools::HighPerfTimer::time_mu _fs_time;
#endif
  std::string _stream_name;
};

class OStreamWriter final : public StreamWriter
{
public:
  OStreamWriter();
  OStreamWriter( std::ostream* stream );
  virtual ~OStreamWriter();
  virtual void init( const std::string& filepath ) override;
  virtual void flush() override;
  virtual void flush_file() override;

private:
  std::ostream* _stream;
};


class ThreadedOFStreamWriter final : public StreamWriter
{
  typedef std::unique_ptr<fmt::Writer> WriterPtr;
  typedef message_queue<WriterPtr> writer_queue;

public:
  ThreadedOFStreamWriter();
  ThreadedOFStreamWriter( std::ofstream* stream );
  virtual ~ThreadedOFStreamWriter();
  virtual void init( const std::string& filepath ) override;
  virtual void flush() override;
  virtual void flush_file() override;

private:
  void start_worker();
  std::ofstream* _stream;
  writer_queue _msg_queue;
  std::thread _writethread;
  std::list<WriterPtr> _writers_hold;
  std::string _stream_name;
};
}
}
#endif  // CLIB_STREAMSAVER_H
