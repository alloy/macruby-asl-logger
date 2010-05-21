desc "Build the ext"
task :build do
  sh "cd ext && macruby extconf.rb && make"
end

desc "Clean"
task :clean do
  rm "ext/Makefile"
  rm "ext/logger.o"
  rm "ext/logger.bundle"
end

desc "Run the specs"
task :spec do
  sh "macruby -r #{FileList['spec/*_spec.rb'].join(' -r ')} -e ''"
end

begin
  require 'jeweler'
  Jeweler::Tasks.new do |gemspec|
    gemspec.name = "macruby-asl-logger"
    gemspec.summary = gemspec.description = "A MacRuby wrapper of the Apple System Log facility"
    gemspec.homepage = "http://github.com/alloy/macruby-asl-logger"
    
    gemspec.authors = ["Eloy Duran"]
    gemspec.email = "eloy.de.enige@gmail.com"
    
    gemspec.required_ruby_version = ::Gem::Requirement.new("~> 1.9")
    gemspec.files.delete '.gitignore'
  end
rescue LoadError
end