# JRuby desktop executable
Compile with Jruby to get class file of the program:

jrubyc test.rb

Check it runs:

java -cp jruby.jar;. test // the ;. is needed!

Create a manifest.txt containing:

Main-Class: test
Class-Path: jruby.jar


Then create a jar with the class file and jruby.jar as so:

jar cvfm test.jar manifest.txt test.class jruby.jar

Check it runs:

java -jar test.jar

Create a custom vm with JLink:

java --list-modules (Use to check modules available)

jlink --output runtime --add-modules java.base,java.desktop,java.logging,java.management,jdk.unsupported,jdk.unsupported.desktop --strip-debug --no-header-files --no-man-pages

Check it runs with custom vm

runtime\bin\java.exe -jar test.jar

Create a launcher without wrapping the jar with launch4j, fiddle with the heap settings to get the memory consumption at sensible numbers. Use the custom VM as the VM.

Now use Enigma VM protector x86 to create a ciphered exe. Have to buy it to remove the splash screen.

