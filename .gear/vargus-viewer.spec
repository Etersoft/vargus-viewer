Name: vargus-viewer
Version: 0.1
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

%install
install VargusViewer -D %buildroot%_bindir/VargusViewer
mkdir -p %buildroot%_datadir/%name/images
cp images/* %buildroot%_datadir/%name/images/

%files
#%doc README
%_bindir/VargusViewer
%_datadir/%name/

%changelog
* Wed Apr 11 2012 Konstantin Kondratyuk <kondratyuk@etersoft.ru> 0.1-alt1
- initial build
