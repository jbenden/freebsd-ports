--- ui/zinf/unix/src/GTKFileSelector.cpp.orig	2003-09-16 17:36:23 UTC
+++ ui/zinf/unix/src/GTKFileSelector.cpp
@@ -25,7 +25,8 @@ ________________________________________
 #include <unistd.h>
 #include <dirent.h>
 
-#if defined(solaris)                                                          
+#if defined(solaris) || defined(__FreeBSD__)                                                         
+#include <stdlib.h>
 #include <sys/types.h>                                                        
 #include <sys/param.h>         /* for MAXPATHLEN */                           
 #include <sys/stat.h>                                                         
