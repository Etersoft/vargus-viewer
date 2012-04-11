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

%description
Qt4 client for vargus server

%prep
%setup

%build
%make_build

%install
%makeinstall_std
#install %name -D %buildroot%_sbindir/%name

%files
%doc README
%_bindir/%name

%changelog
* Wed Apr 11 2012 Konstantin Kondratyuk <kondratyuk@etersoft.ru> 0.1-alt1
- initial build
