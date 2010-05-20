require File.expand_path('../spec_helper', __FILE__)

describe "Logger#add" do
  before do
    @path = tmp("test_log.log")
    @log_file = File.open(@path, "w+")
    # @logger = Logger.new(@path)
    @logger = Logger.new(@log_file)
  end
  
  after do
    # @logger.close
    @log_file.close unless @log_file.closed?
    File.unlink(@path) if File.exists?(@path)
  end
  
  it "writes a new message to the logger" do
    @logger.add(Logger::WARN, "Test")
    @log_file.rewind
    message = @log_file.readlines.last
    strip_metadata(message).should == "<Warning>: Test\n"
  end
  
  it "receives a severity" do
    @logger.log(Logger::INFO,  "Info message")
    @logger.log(Logger::DEBUG, "Debug message")
    @logger.log(Logger::WARN,  "Warn message")
    @logger.log(Logger::ERROR, "Error message")
    @logger.log(Logger::FATAL, "Fatal message")
    
    @log_file.rewind
    
    info, debug, warn, error, fatal = @log_file.readlines
    
    strip_metadata(info).should == "<Info>: Info message\n"
    strip_metadata(debug).should == "<Debug>: Debug message\n"
    strip_metadata(warn).should == "<Warning>: Warn message\n"
    strip_metadata(error).should == "<Error>: Error message\n"
    # strip_metadata(fatal).should == "<Fatal>: Fatal message\n"
    strip_metadata(fatal).should == "<Critical>: Fatal message\n"
  end
  
  it "receives a message" do
    @logger.log(nil, "test")
    @log_file.rewind
    # strip_metadata(@log_file.readline).should == "ANY -- : test\n"
    strip_metadata(@log_file.readline).should == "<Alert>: test\n"
  end
  
  # it "receives a program name" do
  #   @logger.log(nil, "test", "TestApp")
  #   @log_file.rewind
  #   LoggerSpecs::strip_date(@log_file.readline).should == "ANY -- TestApp: test\n"
  # end
  # 
  # it "receives a block" do
  #   lambda {
  #     @logger.log(nil, "test", "TestApp") do
  #       1+1
  #     end
  #   }.should_not raise_error
  # end
  # 
  # it "calls the block if message is nil" do
  #   temp = 0
  #   lambda {
  #     @logger.log(nil, nil, "TestApp") do
  #       temp = 1+1
  #     end
  #   }.should_not raise_error
  #   temp.should == 2
  # end
  # 
  # it "ignores the block if the message is not nil" do
  #   temp = 0
  #   lambda {
  #     @logger.log(nil, "not nil", "TestApp") do
  #       temp = 1+1
  #     end
  #   }.should_not raise_error
  #   temp.should == 0
  # end
end
