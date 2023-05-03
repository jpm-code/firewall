==================
Git
==================
To commit changes:

git commit -a -m "Some message."

Incase you mis-typed a message:

git commit -a --amend -m "New commit message"

To initialize:

git init

To add:

git add somefile.cpp

git add --all

==================
EXE Compiling
==================

Windows:
- Use Ruby and Ocra - it will spawn a unique exe process and separate a ruby interpreter process, hardly noticable, files are small
  > ocra --windows --no-dep-run --dll ruby_builtin_dlls\libgmp-10.dll --add-all-core main.rb
- Use JRuby and Java, package in exe with Launch4J - can extracted easily with WinZip.

==================
Sofware Protection
==================
-Use Crystal Lang or Native Java to implement the network processor. Possibly MRuby (seemingly easy to break).
-Use tools: 
 > https://www.oreans.com/themida.php
 > https://enigmaprotector.com/en/downloads.html
 > https://vmpsoft.com/



