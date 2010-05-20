require File.expand_path('../spec_helper', __FILE__)

describe "Logger.new" do
  before do
    @file_path = tmp("test_log.log")
    @log_file = File.open(@file_path, "w+")
  end
  
  after do
    @log_file.close unless @log_file.closed?
    File.unlink(@file_path) if File.exists?(@file_path)
  end
  
  it "receives a logging device as first argument" do
    l = Logger.new(@log_file)
    l.add(Logger::WARN, "Test message")
    
    @log_file.rewind
    strip_metadata(@log_file.readline).should == "<Warning>: Test message\n"
    # l.close
  end
end