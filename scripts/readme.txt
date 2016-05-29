generate_gui_xml.sh - linux
generate_gui_xml.bat - Windows
generate_gui_xml.ps1 - Windows, it is called by generate_gui_xml.bat, because PowerShell script 
		       execution on another PC is a bit problematic (some safety issues, protections).

generate_gui_xml.* script creates directory with sprites, downloads good sprites, and also
creates gui.xml file in tests directory. PowerShell was used, because there is wget.
