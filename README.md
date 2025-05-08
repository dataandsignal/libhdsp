# libhdsp - Handy DSP routines

### BUILD, INSTALL

1. Install dependencies:

```
rnnoise: https://github.com/xiph/rnnoise
```

2. Then
```
./autogen.sh
./configure
make
sudo make install
```

for debug:
    `make CFLAGS="-ggdb -O0")`

### TEST

```
make check
```

Piotr Gregor, piotr@dataandsignal.com

Roche, St Austell, UK

July 2023
