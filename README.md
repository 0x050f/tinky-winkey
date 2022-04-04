# tinky-winkey
Tinky-winkey is a keylogger running under a service using win32api.  
`svc.exe` is the executable running the service named `tinky` and `winkey.exe` is the keylogger, it will store every keystroke using low-level hook under a file named `winkey.log` where the executable stand. 

## Compilation
```
nmake {all,re,clean,fclean,test} # Run it under both \tinkey and \winkey
```
## Quick install
You can run `build.bat` at the root of the repository to compile both tinky and winkey and copy executables.
```
.\build.bat {all,re,clean,fclean,test}
```
## Execution
`svc.exe` and `winkey.exe` must be in the same directory. Then running as administrator:
```
.\svc.exe install # Install the service
.\svc.exe delete  # Delete the service
.\svc.exe start   # Start the service and run the keylogger
.\svc.exe stop    # Stop the service and the keylogger
```
The file `winkey.log` contains keylogger logs.

## VM install
 + Download Windows VM: https://developer.microsoft.com/en-us/windows/downloads/virtual-machines/
 + Download vim: https://www.vim.org/download.php
 + Download git: https://git-scm.com/download/win
 + Setup openssh-server: https://docs.microsoft.com/en-us/windows-server/administration/openssh/openssh_install_firstuse
 + Install visual studio build tools: https://visualstudio.microsoft.com/downloads/?q=build+tools
