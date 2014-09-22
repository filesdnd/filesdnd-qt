Files Drag & Drop - Qt Version
===========

[![Build Status](https://travis-ci.org/filesdnd/filesdnd-qt.svg?branch=master)](https://travis-ci.org/filesdnd/filesdnd-qt)

Files Drag & Drop allows an easier file transfer between your devices and your computer.
You now can transfer your data on all your devices, quickly, simply and without any required knowledge. Your device is detected, and there is no interaction needed. Any kind of data can be transfered : a file, a folder, a link or text.
Files Drag & Drop allows the transfer between multiple computer or Android devices.

When connected to a Wi-Fi network, your device announce itself on the network in order to be detected by a distant computer. Thereby, you need only to be connected to a local wifi network, without any internet connection.

Visit our [website][1] for more details

## Download

#### Desktop client
Files Drag & Drop is created with modern and portable technology which work on every famous operating system, like Windows, Linux, and Mac OS X.
Find the release you need on the [download page of the website][2]. For Linux, you will need to use the GitHub repository.

#### Android application
Download the [Android application][3] in order to fully use Files Drag & Drop and transfer your files on your devices.

## Build the project

### Linux version
 1. Download the Linux version of  Qt 5.3 at least on the [Qt website][4].
 2. Install the required lib **libavahi** (for example *libavahi-compat-libdnssd-dev* under Ubuntu).
 3. Download/fork the repo, and open the .pro in the source folder. 
 4. Build your Linux app !

Some tray icon issue can appear on Linux due to Qt bug. There is some fix, depending on your GUI provider. For example on Unity (Ubuntu), follow the steps :
 - sudo apt-get install dconf-tools
 - Press Alt + F2 and type 'dconf-editor'.
 - Navigate to desktop > unity
 - Set the systray-whitelist item to ['all'] value
 - Log out and log back in for the changes to take effect. 

### Windows version
 1. Download the Windows version of  Qt 5.3 **for Windows 32-bit** at least on the [Qt website][4]. **Take care to choose the VS (2013 recommended) compiler !**
 3. Download/fork the repo, and open the .pro in the source folder. 
 4. Build your Windows app !

### Mac OS X version
 1. Download the Mac version of  Qt 5.3 at least on the [Qt website][4].
 3. Download/fork the repo, and open the .pro in the source folder. 
 4. Build your Mac OS X app !
 
## User interface
You will be able to find many screenshots on the [Files Drag & Drop gallery][5].

## Dependencies

This project is built with **Qt 5.3** and MSVC 2013 compiler.

 - Qt is under LGPL v2.1 license. See [Qt licensing][6].
 - You can find the Qt 5 documentation [there][7].

Files Drag & Drop uses the Apple Bonjour protocole.

 - Lean what is bonjour on the [Apple website][8].
 - Bonjour is under license Apache 2.0. See [Bonjour licensing][9].

## Credits
 - Thomas Coulange ([nitrog42@gmail.com][10]) 
 - Kevin Renella ([kevin.renella@gmail.com][11])

Thanks to our graphist  ([Alexandre Mottet][12])

## License
Files Drag & Drop is released under [LGPL v2.1][13] license. See the bundled LICENSE file for details.


  [1]: http://www.filesdnd.com/
  [2]: http://www.filesdnd.com/download
  [3]: https://play.google.com/store/apps/details?id=com.filesdnd
  [4]: http://qt-project.org/downloads
  [5]: http://www.filesdnd.com/gallery
  [6]: https://qt-project.org/products/licensing
  [7]: http://qt-project.org/doc/qt-5/classes.html
  [8]: http://www.apple.com/support/bonjour/
  [9]: https://developer.apple.com/softwarelicensing/agreements/bonjour.php
  [10]: mailto:nitrog42@gmail.com
  [11]: mailto:kevin.renella@gmail.com
  [12]: https://github.com/Aleanar
  [13]: http://www.gnu.org/licenses/lgpl-2.1.html
