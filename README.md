# tinky-winkey

## Compilation
```
nmake all # Run it under tinky/ and winky/
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
