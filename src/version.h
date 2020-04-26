#ifndef MAJOR_VERS
#define MAJOR_VERS 2
#endif

#ifndef MINOR_VERS
#define MINOR_VERS 0
#endif

#ifndef VERSION

#ifndef BETA_VERS
#define VERSION "$VER: iGame v" STR(MAJOR_VERS) "." STR(MINOR_VERS) " for " STR(CPU_VERS) " (2019-05-03)"
#else
#define VERSION "$VER: iGame v" STR(MAJOR_VERS) "." STR(MINOR_VERS) "b" STR(BETA_VERS) " for " STR(CPU_VERS) " (2019-05-03)"
#endif

#endif
