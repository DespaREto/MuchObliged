<h1 align="center">MuchObliged</h1>
<h4 align="center">a shrug2 decryptor</h4>
<div align="center">
	<a href="https://github.com/DespaREto/MuchObliged/blob/master/LICENSE">
		<img src="https://img.shields.io/github/license/DespaREto/MuchObliged.svg"/>
	</a>
</div>
<div align="center">
	saves you from bad ransomware.
</div>

## Information
MuchObliged only decrypts individual files, and the focus is not to neuter the ransomware in an infected computer. A tool for that is being programmed by another fellow DespaREto member.

By the way, if you can contribute to this tool in any way, please share your edits with us!

## Usage
The decryption process will only work in an infected machine, but if you have the Key and IV for your machine you can do it in a normal machine by adding Key and IV respectively to "crykey" and "cryiv" REG_SZ values inside ``Computer\HKEY_CURRENT_USER\ShrugTwo`` in the Windows Registry.
```
Computer
|-- HKEY_CURRENT_USER
...  |-- ShrugTwo
          +- crykey
          +- cryiv
          ...
```

To start decrypting a file, just go on MuchObliged's folder (or add it to your path) and execute it with a filename as the first argument. Files must have the .SHRUG2 as last extension.

Example:
```
...\MuchObliged\Release> .\MuchObliged.exe .\background.png.SHRUG2
``` 
## Authors
* DespaREto
* N* Buddies

## Thanks
* CryptoPP
* MBAM Team
