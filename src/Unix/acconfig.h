/* acconfig.h
   This file is in the public domain.

   Descriptive text for the C preprocessor macros that
   the distributed Autoconf macros can define.
   No software package will use all of them; autoheader copies the ones
   your configure.in uses into your configuration header file templates.

   The entries are in sort -df order: alphabetical, case insensitive,
   ignoring punctuation (such as underscores).  Although this order
   can split up related entries, it makes it easier to check whether
   a given entry is in the file.

   Leave the following blank line there!!  Autoheader needs it.  */


/* Define to 'off_t' if <sys/types.h> doesn't define. */
#undef loff_t

/* Define if using native 68k mode. */
#undef ENABLE_NATIVE_M68K

/* Define if pthreads are available. */
#undef HAVE_PTHREADS

/* Define if using video enabled on SEGV signals */
#undef ENABLE_VOSF

/* Define if your system requires signals to be reinstalled */
#undef SIGNAL_NEED_REINSTALL

/* Define if your system requires sigactions to be reinstalled */
#undef SIGACTION_NEED_REINSTALL

/* Define if your system support extended signals */
#undef HAVE_SIGINFO_T

/* Define if we know a hack to replace siginfo_t::si_addr member */
#undef HAVE_SIGCONTEXT_SUBTERFUGE


/* Leave that blank line there!!  Autoheader needs it.
   If you're adding to this file, keep in mind:
   The entries are in sort -df order: alphabetical, case insensitive,
   ignoring punctuation (such as underscores).  */
