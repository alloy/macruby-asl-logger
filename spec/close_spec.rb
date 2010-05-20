require File.expand_path('../spec_helper', __FILE__)

describe "Logger#close" do
  before do
    @path = tmp("test_log.log")
    @log_file = File.open(@path, "w+")
    @logger = Logger.new(@path)
  end

  after do
    @log_file.close unless @log_file.closed?
    File.unlink(@path) if File.exists?(@path)
  end

  it "closes the logging device" do
    @logger.close
    lambda { @logger.add(nil, "Foo") }.should complain('log writing failed. closed stream')
  end
end
