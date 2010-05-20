require 'rubygems'
require 'bacon'

Bacon.summary_on_exit

require File.expand_path('../../ext/logger', __FILE__)

require 'tempfile'

class Bacon::Context
  def tmp(name)
    File.join(Dir.tmpdir, name)
  end
  
  def strip_metadata(log_line)
    log_line.sub(/^.+macruby\[\d+\]\s/, '')
  end
  
  def complain(str)
    lambda do |proc|
      begin
        stderr, $stderr = $stderr, IOStub.new
        verbose, $VERBOSE = $VERBOSE, false
        
        proc.call
        $stderr.include?(str)
      ensure
        $stderr = stderr
        $VERBOSE = verbose
      end
    end
  end
end

class IOStub < String
  def write(*str)
    self << str.join
  end

  def print(*str)
    write(str.join + $\.to_s)
  end

  def puts(*str)
    write(str.collect { |s| s.to_s.chomp }.concat([nil]).join("\n"))
  end

  def printf(format, *args)
    self << sprintf(format, *args)
  end

  def flush
    self
  end
end