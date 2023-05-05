In the classes directory created by maven. Do the following.

mkdir META-INF\native-image

java -agentlib:native-image-agent=config-output-dir=META-INF/native-image com.demo.slidemanagerdemo

native-image -Djava.awt.headless=false -H:ReflectionConfigurationFiles=META-INF\native-image\reflect-config.json -H:ResourceConfigurationFiles=META-INF\native-image\resource-config.json com.demo.slidemanagerdemo

# Use WinRun4J's tool to inject an icon file. The filename must be the package.
RCEDIT64.exe /I com.demo.slidemanagerdemo.exe smile.ico

#Make an GUI application.
EDITBIN /SUBSYSTEM:WINDOWS com.demo.slidemanagerdemo.exe