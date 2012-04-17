Name: vargus-viewer
Version: 0.3
Release: alt1

Summary: Qt4 client for vargus server

Group: Video
License: GPL
Url: http://git.etersoft.ru/projects/other/vargus-viewer.git

Packager: Konstantin Kondratyuk <kondratyuk@etersoft.ru>

# Source-git: http://git.etersoft.ru/projects/other/vargus-viewer.git
Source: %name-%version.tar

# Automatically added by buildreq on Thu Apr 12 2012
# optimized out: fontconfig libqt4-core libqt4-devel libqt4-gui libqt4-network libstdc++-devel
BuildRequires: gcc-c++ libvlc-devel libqt4-devel

%description
Qt4 client for vargus server

%prep
%setup

%build
qmake-qt4
%make_build
lrelease-qt4 lang/lang_ru.ts

%install
install VargusViewer -D %buildroot%_bindir/VargusViewer
mkdir -p %buildroot%_datadir/%name/images
cp images/* %buildroot%_datadir/%name/images/
mkdir -p %buildroot%_datadir/%name/lang
cp lang/lang_ru.qm %buildroot%_datadir/%name/lang/

%files
#%doc README
%_bindir/VargusViewer
%_datadir/%name/

%changelog
* Tue Apr 17 2012 Konstantin Kondratyuk <kondratyuk@etersoft.ru> 0.3-alt1
- add icons
- add trayIcon
- add russian translation
* Mon Apr 16 2012 Konstantin Kondratyuk <kondratyuk@etersoft.ru> 0.2-alt1
- add icons
- new parser of input parameters
- some bug fixes
* Wed Apr 11 2012 Konstantin Kondratyuk <kondratyuk@etersoft.ru> 0.1-alt1
- initial build
