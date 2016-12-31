Test disk read/write speed via `hdparm` and `dd`.

## Install

```sh
git clone https://github.com/weakish/disk-speed.git
cd disk-speed
make
make install
```

Edit `config.mk` to change installation prefix (default: `/usr/local`) and compiler (default: clang).

The Makefile is compatible with GNU make, BSD make, and smake.

### Uninstall

```sh
cd disk-speed
make uninstall
```

## Usage

`cd` into a directory on the disk to test,
and run `disk-speed` as root.
It requires 1 GB of free space on the disk to test.

## Contributing

Fork and send pull requests at [GitHub](https://github.com/weakish/disk-speed/).

## License

0BSD
