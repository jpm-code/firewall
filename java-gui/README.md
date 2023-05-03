== SWT System Tray and GraalVM ==
If you use SWT rather than AWT and Swing the binary is very small ~13mb, and memory consumption running is also around 2Mb. Which is nothing. 
There is no OpenJDK binary either in the process list.

Alternative is to create a Jar and wrap it in an exe using Launch4j. Another is to create a binary and JVM with JPackage.
