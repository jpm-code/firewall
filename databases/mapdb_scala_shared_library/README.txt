##
## Native Image shared library using scala - Liberica Native Image 22 (Windows Tested)
##

Have to run it with a main function first to get the native image config json files! For example:

scalac -cp mapdb-3.0.9-jar-with-dependencies.jar LibFunc.scala -d LibFunc.jar

##################
#Compile scala
scalac -cp mapdb-3.0.9-jar-with-dependencies.jar LibFunc.scala

#Compile wrapper using scala library
javac -cp mapdb-3.0.9-jar-with-dependencies.jar;scala-library-2.10.3.jar;. LibFuncWrapper.java

## Add scala-library jar and run native-image as below
native-image -Djava.awt.headless=false -H:ReflectionConfigurationFiles=META-INF\native-image\reflect-config.json -H:ResourceConfigurationFiles=META-INF\native-image\resource-config.json -cp mapdb-3.0.9-jar-with-dependencies.jar;scala-library-2.10.3.jar;. -H:Name=LibFuncWrapper --shared

## Compile with VS cl tool
cl.exe /I .\ main.c LibFuncWrapper.lib




