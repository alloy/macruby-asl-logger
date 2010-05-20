require 'rubygems'
require 'bacon'

Bacon.summary_on_exit

require File.expand_path('../../ext/logger', __FILE__)

require 'tempfile'
class Bacon::Context
  def tmp(name)
    File.join(Dir.tmpdir, name)
  end
end