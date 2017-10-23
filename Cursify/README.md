# Cursify
Recursively adding inotify watch to all files under given path.
The program will first traverse the given path in the way just like ls -R do,
and then get into recursive function to add inotify watch to the files.

after the recursions' done, input 1 to start listening messages from files notified, 0 to exit the program.
