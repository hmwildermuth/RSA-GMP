# RSA-GMP

Lightweight command-line utility demonstrating RSA cryptography implemented in C using the GNU MP (GMP) multiple-precision arithmetic library.

## Features

* 512-bit RSA key generation (key size can be tweaked via the `KEY_SIZE` constant).
* Encryption / signing of arbitrary files using the private exponent \(d\).
* Outputs ciphertext to **stdout** or to a specified file.
* Only one dependency: GMP.

> **DISCLAIMER** – This project is for educational and experimentation purposes **only**. It omits proper padding (e.g. OAEP / PKCS#1 v1.5) and other hardening measures. **Do not use it in production or to protect real secrets.**

## Prerequisites

• GCC or Clang (any C99-compatible compiler)  
• GMP library and development headers

Ubuntu / Debian:
```bash
sudo apt-get install build-essential libgmp-dev
```
macOS (Homebrew):
```bash
brew install gmp
```

## Build

```bash
gcc -o rsa rsa.c -lgmp
```

If you renamed the file or want optimisations:
```bash
gcc -O2 -std=c99 -Wall -o rsa rsa.c -lgmp
```

## Command-line usage

```
./rsa -g [-f <input>] [-o <output>]               # generate new keypair, optionally sign <input>
./rsa -n <modulus> -e <private_exp> -f <input> [-o <output>]  # use existing private key to sign <input>
```

Options:

* `-g`                Generate a fresh keypair and print it to the console.
* `-n <modulus>`      Decimal representation of RSA modulus **n**.
* `-e <private_exp>`  Decimal representation of private exponent **d**.
* `-f <input>`        Path to the file to encrypt / sign.
* `-o <output>`       Write ciphertext to this file instead of stdout.

### Examples

Generate keys only:
```bash
./rsa -g
```

Generate keys and sign `message.txt`, writing signature to `message.sig`:
```bash
./rsa -g -f message.txt -o message.sig
```

Sign a file with an **existing** key:
```bash
./rsa \
  -n 226521996797410017036868813... \
  -e 137728934413467637133271... \
  -f message.txt -o message.sig
```

## Changing the key size

Adjust the line at the top of `rsa.c`:
```c
#define KEY_SIZE 512  // bits
```
Increase the value (e.g. 1024 or 2048) for stronger but slower keys.

## How it works (briefly)

1. Two random probable primes **p** and **q** (each `KEY_SIZE / 2` bits) are generated.
2. Compute modulus **n = p × q**.
3. Public exponent **e** is initialised to 65 537 and, if necessary, incremented until gcd(λ(n), e) = 1.
4. Private exponent **d** is calculated as the modular inverse of **e** modulo λ(n).
5. Input file bytes are imported as an integer **m**; ciphertext **c = mᵈ mod n** is computed.
6. The ciphertext integer is exported back to bytes and written out.

Again, **no padding** is performed – the plaintext must be **smaller than n** and leaks structure. Use proper cryptographic padding schemes for any real-world application.

## License

MIT License
