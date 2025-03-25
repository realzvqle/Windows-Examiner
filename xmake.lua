set_project("winexamine")

set_xmakever("2.6.0")

set_languages("c99")

target("winexamine")
    set_kind("binary")  
    add_files("src/*.c")
    add_files("src/**/*.c") 
    add_files("winexamine.rc")
    add_includedirs("src") 
    add_linkdirs("lib") 
    add_links("raylibdll", "raylib") 
    add_syslinks("gdi32", "winmm", "user32", "shell32", "ntdll", "powrprof", "advapi32", "psapi") 
    before_build(function (target)
        local result = os.run("rc winexamine.rc")     
    end)