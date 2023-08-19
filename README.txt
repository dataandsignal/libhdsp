libhdsp - Handy DSP routines

INSTALL

sudo apt-get update
sudo apt-get -y install automake autoconf
autoreconf --install
autoconf
automake --add-missing
./configure
make					(for debug: make CFLAGS="-ggdb -O0")
make test
make test check			(for autotesting)
sudo make install		(will install lib to /usr/local/lib)
sudo ldconfig

Piotr Gregor, piotr@dataandsignal.com
Roche, St Austell, UK
July 2023
