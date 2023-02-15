# dart-ssl-fix

```sh
${CC:-cc} main.c -shared -o preload.so
LD_PRELOAD="$PWD/preload.so" flutter <...>
```
