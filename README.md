Race!
=====
`3,2,1... Go!!!`


### How to run:
#### UNIX
```
git clone https://github.com/gto76/race.git
cd race
./run
```

#### Windows

* Download and install [wget](http://sourceforge.net/projects/gnuwin32/files/wget/1.11.4-1/wget-1.11.4-1-setup.exe/download)
* Open command prompt
* Go to the directory where you want Cygwin installed
* Install Cygwin by pasting this commands:

>```bat
:: set path of wget
set PATH=%PATH%;C:\Program Files (x86)\GnuWin32\bin
:: download cygwin
wget --no-check-certificate https://cygwin.com/setup-x86_64.exe
:: install cygwin
mkdir cygwin
setup-x86_64.exe --quiet-mode --no-shortcuts --site http://mirror.nexcess.net --root %cd%\cygwin -P git
::
```

* Download and run the game by pasting this commands:

>```bash
cygwin\Cygwin.bat
# clone and run
cd
git clone https://github.com/gto76/race.git
cd race
./run

```
