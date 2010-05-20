require File.expand_path('../spec_helper', __FILE__)

describe "Logger::Severity" do
  it "defines Logger severity constants" do
    Logger::EMERGENCY.should == 0
    Logger::ALERT.should == 1
    Logger::CRITICAL.should == 2
    Logger::ERROR.should == 3
    Logger::WARNING.should == 4
    Logger::NOTICE.should == 5
    Logger::INFO.should == 6
    Logger::DEBUG.should == 7
  end
  
  it "aliases Ruby’s Logger constants to the compatible ones defined by ASL" do
    Logger::DEBUG.should == Logger::DEBUG
    Logger::INFO.should == Logger::INFO
    Logger::WARN.should == Logger::WARNING
    Logger::ERROR.should == Logger::ERROR
    Logger::FATAL.should == Logger::CRITICAL
    Logger::UNKNOWN.should == Logger::ALERT
  end
end