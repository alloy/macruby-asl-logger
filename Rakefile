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