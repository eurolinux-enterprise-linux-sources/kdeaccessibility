Summary: KDE Accessibility
Name:    kdeaccessibility
Epoch:   1
Version: 4.10.5
Release: 2%{?dist}

License: GPLv2
Group:   User Interface/Desktops
URL:     http://accessibility.kde.org/
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildArch: noarch

BuildRequires:  kde-filesystem

Requires: kaccessible >= %{version}
Requires: kmag >= %{version}
%if 0%{?fedora}
Requires: kmousetool >= %{version}
Requires: kmouth >= %{version}
Requires: jovie >= %{version}
%endif

Obsoletes: kdeaccessibility-libs < 1:4.7.80
Provides:  kdeaccessibility-libs = %{epoch}:%{version}-%{release}


%description
Kdeaccessibility metapackage, to ease migration to split applications.


%prep


%build


%install


%files
# empty


%changelog
* Fri Dec 27 2013 Daniel Mach <dmach@redhat.com> - 1:4.10.5-2
- Mass rebuild 2013-12-27

* Wed Jul 03 2013 Than Ngo <than@redhat.com> - 1:4.10.5-1
- 4.10.5

* Sat Jun 01 2013 Rex Dieter <rdieter@fedoraproject.org> - 1:4.10.4-1
- 4.10.4

* Tue May 07 2013 Rex Dieter <rdieter@fedoraproject.org> - 1:4.10.3-1
- 4.10.3

* Sun Mar 31 2013 Rex Dieter <rdieter@fedoraproject.org> - 1:4.10.2-1
- 4.10.2

* Sat Mar 02 2013 Rex Dieter <rdieter@fedoraproject.org> - 1:4.10.1-1
- 4.10.1

* Fri Feb 01 2013 Rex Dieter <rdieter@fedoraproject.org> - 1:4.10.0-1
- 4.10.0

* Thu Jan 24 2013 Than Ngo <than@redhat.com> - 1:4.9.98-2
- rhel/fedora condition

* Tue Jan 22 2013 Rex Dieter <rdieter@fedoraproject.org> - 1:4.9.98-1
- 4.9.98

* Fri Jan 04 2013 Rex Dieter <rdieter@fedoraproject.org> - 1:4.9.97-1
- 4.9.97

* Thu Dec 20 2012 Rex Dieter <rdieter@fedoraproject.org> - 1:4.9.95-1
- 4.9.95

* Tue Dec 04 2012 Rex Dieter <rdieter@fedoraproject.org> - 1:4.9.90-1
- 4.9.90

* Sat Nov 03 2012 Rex Dieter <rdieter@fedoraproject.org> - 1:4.9.3-1
- 4.9.3

* Sat Sep 29 2012 Rex Dieter <rdieter@fedoraproject.org> - 1:4.9.2-1
- 4.9.2

* Wed Sep 05 2012 Rex Dieter <rdieter@fedoraproject.org> - 1:4.9.1-1
- 4.9.1

* Fri Jul 27 2012 Rex Dieter <rdieter@fedoraproject.org> - 1:4.9.0-1
- 4.9.0

* Thu Jul 19 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1:4.8.97-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_18_Mass_Rebuild

* Wed Jul 11 2012 Rex Dieter <rdieter@fedoraproject.org> - 1:4.8.97-1
- 4.8.97

* Thu Jun 28 2012 Rex Dieter <rdieter@fedoraproject.org> - 1:4.8.95-1
- 4.8.95

* Sat Jun 09 2012 Rex Dieter <rdieter@fedoraproject.org> - 1:4.8.90-1
- 4.8.90

* Mon May 07 2012 Than Ngo <than@redhat.com> - 1:4.8.3-2
- add fedora/rhel condition

* Mon Apr 30 2012 Jaroslav Reznik <jreznik@redhat.com> - 1:4.8.3-1
- 4.8.3

* Fri Mar 30 2012 Rex Dieter <rdieter@fedoraproject.org> - 1:4.8.2-1
- 4.8.2

* Mon Mar 05 2012 Radek Novacek <rnovacek@redhat.com> - 1:4.8.1-1
- 4.8.1

* Sun Jan 22 2012 Rex Dieter <rdieter@fedoraproject.org> - 1:4.8.0-1
- 4.8.0

* Wed Jan 04 2012 Radek Novacek <rnovacek@redhat.com> - 1:4.7.97-1
- 4.7.97

* Wed Dec 21 2011 Radek Novacek <rnovacek@redhat.com> - 1:4.7.95-1
- 4.7.95

* Tue Dec 20 2011 Rex Dieter <rdieter@fedoraproject.org> - 1:4.7.90-1
- 4.7.90 (metapackage)

* Mon Oct 31 2011 Rex Dieter <rdieter@fedoraproject.org> 4.7.3-1
- 4.7.3

* Wed Oct 05 2011 Rex Dieter <rdieter@fedoraproject.org> 4.7.2-2
- KMouth has wrong (truncated) description (#742751)

* Tue Oct 04 2011 Rex Dieter <rdieter@fedoraproject.org> 4.7.2-1
- 4.7.2

* Tue Sep 06 2011 Than Ngo <than@redhat.com> - 4.7.1-1
- 4.7.1

* Wed Aug 03 2011 Radek Novacek <rnovacek@redhat.com> - 4.7.0-2
- Fix jovie build with Qt-4.8

* Tue Jul 26 2011 Jaroslav Reznik <jreznik@redhat.com> - 4.7.0-1
- 4.7.0

* Fri Jul 08 2011 Jaroslav Reznik <jreznik@redhat.com> - 4.6.95-1
- 4.6.95 (rc2)

* Mon Jun 27 2011 Than Ngo <than@redhat.com> - 4.6.90-1
- 4.6.90 (rc1)

* Mon Jun 06 2011 Jaroslav Reznik <jreznik@redhat.com> - 4.6.80-1
- 4.6.80 (beta1)
- move mono-icon-theme

* Wed Apr 06 2011 Than Ngo <than@redhat.com> - 4.6.2-1
- 4.6.2

* Mon Feb 28 2011 Rex Dieter <rdieter@fedoraproject.org> 4.6.1-1
- 4.6.1

* Mon Feb 07 2011 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1:4.6.0-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_15_Mass_Rebuild

* Fri Jan 21 2011 Jaroslav Reznik <jreznik@redhat.com> 4.6.0-1
- 4.6.0

* Wed Jan 05 2011 Jaroslav Reznik <jreznik@redhat.com> 4.5.95-1
- 4.5.95 (4.6rc2)

* Wed Dec 22 2010 Rex Dieter <rdieter@fedoraproject.org> 4.5.90-1
- 4.5.90 (4.6rc1)

* Sat Dec 04 2010 Thomas Janssen <thomasj@fedoraproject.org> 4.5.85-1
- 4.5.85 (4.6beta2)

* Sun Nov 21 2010 Rex Dieter <rdieter@fedoraproject.org> -  4.5.80-1
- 4.5.80 (4.6beta1)

* Fri Oct 29 2010 Than Ngo <than@redhat.com> - 4.5.3-1
- 4.5.3

* Sat Oct 02 2010 Rex Dieter <rdieter@fedoraproject.org> - 4.5.2-1
- 4.5.2

* Fri Aug 27 2010 Jaroslav Reznik <jreznik@redhat.com> - 4.5.1-1
- 4.5.1

* Tue Aug 03 2010 Than Ngo <than@redhat.com> - 4.5.0-1
- 4.5.0

* Sat Jul 25 2010 Rex Dieter <rdieter@fedoraproject.org> - 4.4.95-1
- 4.5 RC3 (4.4.95)

* Wed Jul 07 2010 Rex Dieter <rdieter@fedoraproject.org> - 4.4.92-1
- 4.5 RC2 (4.4.92)

* Fri Jun 25 2010 Jaroslav Reznik <jreznik@redhat.com> - 4.4.90-1
- 4.5 RC1 (4.4.90)

* Thu Jun 10 2010 Rex Dieter <rdieter@fedoraproject.org> - 4.4.85-2
- mono-icon-theme subpkg
- kttsd->jovie rename

* Mon Jun 07 2010 Jaroslav Reznik <jreznik@redhat.com> - 4.4.85-1
- 4.5 Beta 2 (4.4.85)

* Fri May 21 2010 Jaroslav Reznik <jreznik@redhat.com> - 4.4.80-1
- 4.5 Beta 1 (4.4.80)

* Fri Apr 30 2010 Jaroslav Reznik <jreznik@redhat.com> - 4.4.3-1
- 4.4.3

* Mon Mar 29 2010 Lukas Tinkl <ltinkl@redhat.com> - 4.4.2-1
- 4.4.2

* Sat Feb 27 2010 Rex Dieter <rdieter@fedoraproject.org> - 4.4.1-1
- 4.4.1

* Fri Feb 05 2010 Than Ngo <than@redhat.com> - 1:4.4.0-1
- 4.4.0

* Sun Jan 31 2010 Rex Dieter <rdieter@fedoraproject.org> - 4.3.98-1
- KDE 4.3.98 (4.4rc3)

* Wed Jan 20 2010 Lukas Tinkl <ltinkl@redhat.com> - 4.3.95-1
- KDE 4.3.95 (4.4 rc2)

* Wed Jan 06 2010 Rex Dieter <rdieter@fedoraproject.org> - 4.3.90-1
- kde-4.3.90 (4.4rc1)

* Fri Dec 18 2009 Rex Dieter <rdieter@fedoraproject.org> - 4.3.85-1
- kde-4.3.85 (4.4beta2)

* Wed Dec 16 2009 Jaroslav Reznik <jreznik@redhat.com> - 4.3.80-2
- Repositioning the KDE Brand (#547361)

* Tue Dec  1 2009 Lukáš Tinkl <ltinkl@redhat.com> - 4.3.80-1
- KDE 4.4 beta1 (4.3.80)

* Sun Nov 22 2009 Ben Boeckel <MathStuf@gmail.com> - 4.3.75-0.1.svn1048496
- Update to 4.3.75 snapshot

* Wed Nov 11 2009 Than Ngo <than@redhat.com> - 4.3.3-2
- rhel cleanup, drop BR on flite-devel

* Sat Oct 31 2009 Rex Dieter <rdieter@fedoraproject.org> - 4.3.3-1
- 4.3.3

* Sun Oct 04 2009 Than Ngo <than@redhat.com> - 4.3.2-1
- 4.3.2

* Fri Aug 28 2009 Than Ngo <than@redhat.com> - 4.3.1-1
- 4.3.1

* Sun Aug 02 2009 Rex Dieter <rdieter@fedoraproject.org> 1:4.3.0-3
- include epoch's in -libs-related Requires

* Sat Aug 01 2009 Rex Dieter <rdieter@fedoraproject.org> 1:4.3.0-2
- -libs subpkg: Multilib conflicts for index.cache.bz2 (#515085)
- %%check: desktop-file-validate

* Thu Jul 30 2009 Than Ngo <than@redhat.com> - 1:4.3.0-1
- 4.3.0

* Fri Jul 24 2009 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1:4.2.98-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_12_Mass_Rebuild

* Tue Jul 21 2009 Than Ngo <than@redhat.com> - 4.2.98-1
- 4.3rc3

* Thu Jul 09 2009 Than Ngo <than@redhat.com> - 4.2.96-1
- 4.3rc2

* Thu Jun 25 2009 Than Ngo <than@redhat.com> - 4.2.95-1
- 4.3 rc1

* Thu Jun 04 2009 Lorenzo Villani <lvillani@binaryhelix.net> - 1:4.2.90-1
- KDE 4.3 beta 2

* Wed May 13 2009 Lukáš Tinkl <ltinkl@redhat.com> - 4.2.85-1
- KDE 4.3 beta 1

* Wed Apr 22 2009 Rex Dieter <rdieter@fedoraproject.org> - 4.2.2-2
- Requires: festival (for kttsd)

* Mon Mar 30 2009 Lukáš Tinkl <ltinkl@redhat.com> - 4.2.2-1
- KDE 4.2.2

* Mon Mar 23 2009 Rex Dieter <rdieter@fedoraproject.org> - 4.2.1-2
- scriptlet optimization

* Fri Feb 27 2009 Than Ngo <than@redhat.com> - 4.2.1-1
- 4.2.1

* Wed Feb 25 2009 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 1:4.2.0-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_11_Mass_Rebuild

* Thu Jan 22 2009 Than Ngo <than@redhat.com> - 4.2.0-1
- 4.2.0

* Fri Jan 09 2009 Lorenzo Villani <lvillani@binaryhelix.net> - 1:4.1.96-2
- rebuilt
- fix file list

* Wed Jan 07 2009 Than Ngo <than@redhat.com> - 4.1.96-1
- 4.2rc1

* Thu Dec 11 2008 Than Ngo <than@redhat.com> -  4.1.85-1
- 4.2beta2

* Thu Dec 04 2008 Kevin Kofler <Kevin@tigcc.ticalc.org> 4.1.80-4
- rebuild for fixed kde-filesystem (macros.kde4) (get rid of rpaths)

* Thu Dec 04 2008 Rex Dieter <rdieter@fedoraproject.or> 4.1.80-3
- drop (Build)Requires: kdebase-workspace(-devel)
- BR: plasma-devel
- better, versioned Obsoletes: kdeaccessibility-devel ...

* Thu Nov 20 2008 Than Ngo <than@redhat.com> 4.1.80-2
- remove duplicated BR on cmake, it's already done in kdelibs-devel

* Wed Nov 19 2008 Lorenzo Villani <lvillani@binaryhelix.net> - 1:4.1.80-1
- 4.1.80
- BR cmake >= 2.6.2
- make install/fast

* Tue Nov 11 2008 Than Ngo <than@redhat.com> 4.1.3-1
- KDE 4.1.3

* Mon Sep 29 2008 Rex Dieter <rdieter@fedoraproject.org> 4.1.2-2
- make VERBOSE=1
- respin against new(er) kde-filesystem

* Fri Sep 26 2008 Rex Dieter <rdieter@fedoraproject.org> 4.1.2-1
- 4.1.2

* Fri Aug 29 2008 Than Ngo <than@redhat.com> 4.1.1-1
- 4.1.1

* Wed Jul 23 2008 Than Ngo <than@redhat.com> 4.1.0-1
- 4.1.0

* Fri Jul 18 2008 Rex Dieter <rdieter@fedoraproject.org> 4.0.99-1
- 4.0.99

* Fri Jul 11 2008 Rex Dieter <rdieter@fedoraproject.org> 4.0.98-1
- 4.0.98

* Sun Jul 06 2008 Rex Dieter <rdieter@fedoraproject.org> 4.0.85-1
- 4.0.85

* Fri Jun 27 2008 Rex Dieter <rdieter@fedoraproject.org> 4.0.84-1
- 4.0.84

* Thu Jun 19 2008 Than Ngo <than@redhat.com> 4.0.83-1
- 4.0.83 (beta2)

* Sun Jun 15 2008 Rex Dieter <rdieter@fedoraproject.org> 4.0.82-1
- 4.0.82

* Mon May 26 2008 Than Ngo <than@redhat.com> 4.0.80-1
- Beta 1

* Wed May 07 2008 Kevin Kofler <Kevin@tigcc.ticalc.org> 4.0.72-1
- update to 4.0.72

* Thu Apr 03 2008 Kevin Kofler <Kevin@tigcc.ticalc.org> 4.0.3-3
- rebuild (again) for the fixed %%{_kde4_buildtype}

* Mon Mar 31 2008 Kevin Kofler <Kevin@tigcc.ticalc.org> 4.0.3-2
- rebuild for NDEBUG and _kde4_libexecdir

* Fri Mar 28 2008 Than Ngo <than@redhat.com> 4.0.3-1
- 4.0.3

* Thu Feb 28 2008 Than Ngo <than@redhat.com> 4.0.2-1
- 4.0.2

* Thu Jan 31 2008 Rex Dieter <rdieter@fedoraproject.org> 1:4.0.1-1
- 4.0.1

* Tue Jan 08 2008 Kevin Kofler <Kevin@tigcc.ticalc.org> 1:4.0.0-1
- update to 4.0.0
- update file list

* Fri Dec 07 2007 Rex Dieter <rdieter[AT]fedoraproject.org> 1:3.97.0-2
- fix %%files (unpackaged mono/index.theme)

* Wed Dec 05 2007 Rex Dieter <rdieter[AT]fedoraproject.org> 1:3.97.0-1
- kde-3.97.0

* Tue Dec 04 2007 Rex Dieter <rdieter[AT]fedoraproject.org> 1:3.96.2-2
- minor touchups
- Provides: mono-icon-theme
- drop -libs,-devel subpkgs

* Fri Nov 30 2007 Sebastian Vahl <fedora@deadbabylon.de> 1:3.96.2-1
- kde-3.96.2

* Sat Nov 24 2007 Sebastian Vahl <fedora@deadbabylon.de> 1:3.96.1-1
- kde-3.96.1
- added epoch in changelog (also backwards)

* Thu Nov 22 2007 Sebastian Vahl <fedora@deadbabylon.de> 1:3.96.0-4
- libs subpkg

* Fri Nov 16 2007 Sebastian Vahl <fedora@deadbabylon.de> 1:3.96.0-3
- BR: kde-filesystem >= 4

* Fri Nov 16 2007 Sebastian Vahl <fedora@deadbabylon.de> 1:3.96.0-2
- BR: libXtst-devel
- fix copy&paste error in devel package

* Fri Nov 16 2007 Sebastian Vahl <fedora@deadbabylon.de> 1:3.96.0-1
- Initial version for Fedora

* Tue Oct 25 2007 Rex Dieter <rdieter[AT]fedoraproject.org> - 1:3.5.8-3
- Obsoletes: %%name ... to help out multilib upgrades

* Sat Oct 13 2007 Rex Dieter <rdieter[AT]fedoraproject.org> - 1:3.5.8-2
- Obsoletes: %%name-devel

* Sat Oct 13 2007 Rex Dieter <rdieter[AT]fedoraproject.org> - 1:3.5.8-1
- kde-3.5.8
- omit -devel subpkg

* Mon Aug 20 2007 Rex Dieter <rdieter[AT]fedoraproject.org> - 1:3.5.7-3
- License: GPLv2
- Provides: kdeaccessibility3(-devel)
- (Build)Requires: kdelibs3(-devel)

* Sat Jun 16 2007 Rex Dieter <rdieter[AT]fedoraproject.org> - 1:3.5.7-2
- portability (rhel)
- omit .desktop patch (doesn't apply)

* Sat Jun 16 2007 Rex Dieter <rdieter[AT]fedoraproject.org> - 1:3.5.7-1
- cleanup

* Thu Jun 07 2007 Than Ngo <than@redhat.com> - 1:3.5.7-0.1.fc7
- 3.5.7

* Mon Mar 12 2007 Than Ngo <than@redhat.com> 1:3.5.6-3.fc7
- fix broken dependencies

* Mon Feb 26 2007 Than Ngo <than@redhat.com> - 1:3.5.6-2.fc7
- cleanup specfile

* Wed Feb 07 2007 Than Ngo <than@redhat.com> 1:3.5.6-1.fc7
- 3.5.6

* Thu Aug 10 2006 Than Ngo <than@redhat.com> 1:3.5.4-1
- rebuild

* Mon Jul 24 2006 Than Ngo <than@redhat.com> 1:3.5.4-0.pre1
- prerelease of 3.5.4 (from the first-cut tag)

* Mon Jul 17 2006 Than Ngo <than@redhat.com> 1:3.5.3-2
- rebuild

* Wed Jul 12 2006 Jesse Keating <jkeating@redhat.com> - 1:3.5.3-1.1
- rebuild

* Fri Jun 02 2006 Than Ngo <than@redhat.com> 1:3.5.3-1
- update to 3.5.3

* Tue Apr 04 2006 Than Ngo <than@redhat.com> 1:3.5.2-1
- update to 3.5.2

* Wed Mar 01 2006 Karsten Hopp <karsten@redhat.de> 3.5.1-2
- BuildRequires: libXtst-devel

* Fri Feb 10 2006 Jesse Keating <jkeating@redhat.com> - 1:3.5.1-1.2
- bump again for double-long bug on ppc(64)

* Tue Feb 07 2006 Jesse Keating <jkeating@redhat.com> - 1:3.5.1-1.1
- rebuilt for new gcc4.1 snapshot and glibc changes

* Fri Feb 03 2006 Than Ngo <than@redhat.com> 1:3.5.1-1
- 3.5.1

* Fri Dec 09 2005 Jesse Keating <jkeating@redhat.com>
- rebuilt

* Thu Dec 01 2005 Than Ngo <than@redhat.com> 1:3.5.0-1
- 3.5

* Tue Oct 25 2005 Than Ngo <than@redhat.com> 1:3.4.92-1
- update to 3.5 beta2

* Mon Oct 10 2005 Than Ngo <than@redhat.com> 1:3.4.91-1
- update to 3.5 beta 1

* Mon Aug 08 2005 Than Ngo <than@redhat.com> 1:3.4.2-1
- update to 3.4.2

* Tue Jun 28 2005 Than Ngo <than@redhat.com> 1:3.4.1-1
- 3.4.1
- fix gcc4 build problem

* Wed Mar 16 2005 Than Ngo <than@redhat.com> 1:3.4.0-1
- KDE 3.4.0 final

* Sun Feb 27 2005 Than Ngo <than@redhat.com> 1:3.4.0-0.rc1.1
- KDE 3.4.0 rc1

* Sun Feb 20 2005 Than Ngo <than@redhat.com> 1:3.3.92-0.1
- KDE 3.4 beta2

* Sun Dec 05 2004 Than Ngo <than@redhat.com> 3.3.2-0.1
- update to 3.3.2

* Fri Jun 04 2004 Than Ngo <than@redhat.com> 1:3.2.3-0.1
- update to 3.2.3

* Mon Apr 12 2004 Than Ngo <than@redhat.com> 1:3.2.2-0.1
- 3.2.2 release

* Sat Mar 06 2004 Than Ngo <than@redhat.com> 1:3.2.1-0.1
- 3.2.1 release

* Fri Feb 06 2004 Than Ngo <than@redhat.com> 1:3.2.0-0.2
- rebuilt against qt 3.3.0

* Mon Feb 02 2004 Than Ngo <than@redhat.com> 1:3.2.0-0.1
- 3.2.0 release

* Sun Dec 07 2003 Than Ngo <than@redhat.com> 6:3.1.94-0.1
- KDE 3.2 Beta 2

* Sat Nov 15 2003 Than Ngo <than@redhat.com> 6:3.1.93-0.1
- initial rpm for Fedora
