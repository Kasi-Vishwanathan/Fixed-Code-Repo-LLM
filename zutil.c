/* Original K&R-style */
void z_error (format, a1, a2, a3, a4, a5, a6)
    char *format;
    a1, a2, a3, a4, a5, a6;
{ ... }

/* Corrected ANSI C */
void z_error(char *format, int a1, int a2, int a3, int a4, int a5, int a6)
{ ... }