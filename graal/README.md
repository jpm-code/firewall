# Ruby and Java
You need to install graalvm and version which supports truffle ruby. It isn't available for Windows (Ruby).

You can install it with:
gu available
gu ruby

Or get it from the github site.

To use Java inside of Ruby you need to pass --jvm to the interpreter:

ruby --jvm myfile.rb

If no file is given it will start irb. The included irb doesn't have jvm support seemingly.

All of this doesn't use the native compiled Truffle Ruby. To use that you need to pass --polygot like this:

ruby --polyglot --jvm

You access Java as described here: 

https://github.com/oracle/truffleruby/blob/master/doc/user/jruby-migration.md

For instance:

Java.import 'java.lang.System'
System.out.println("Hello World")
Java.import 'java.util.ArrayList'
cars = ArrayList.new
cars.add "red"
cars.add "blue"
puts "#{cars}"

