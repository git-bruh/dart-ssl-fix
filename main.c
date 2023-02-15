#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CERT_FILE "/etc/ssl/cert.pem"

static int (*real_main)(int, char **, char **);

static int fixed_main(int argc, char **argv, char **envp) {
  char *last_slash = strrchr(argv[0], '/');

  if (!strcmp(last_slash ? last_slash + 1 : argv[0], "dart")) {
    char **new_argv = malloc((argc + 1) * sizeof(*new_argv));

    new_argv[0] = *argv++;
    new_argv[1] = "--root-certs-file=" CERT_FILE;

    while (*argv != NULL) {
      *new_argv++ = *argv++;
    }

    argv = new_argv;
  }

  return real_main(argc, argv, envp);
}

int __libc_start_main(int (*main)(int, char **, char **), int argc, char **argv,
                      __typeof__(main) init, void (*fini)(void),
                      void (*rtld_fini)(void), void *stack_end) {
  real_main = main;
  __typeof__(&__libc_start_main) start_main =
      dlsym(RTLD_NEXT, "__libc_start_main");

  return start_main(fixed_main, argc, argv, init, fini, rtld_fini, stack_end);
}
