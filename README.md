# bitcoinsign

Python c extensions for signing Bitcoin messages.

Massive amount of credit to https://github.com/scintill/bitcoin-signature-tools for the inital verifysignature tool.

## Dependencies and Building

This currently depends on OpenSSL and Python.  On Ubuntu or similar systems, the following should be sufficient to install dependencies and build:

    sudo apt-get install build-essential libssl-dev python-dev
    python setup.py build    
    python setup.py install

## Usage

Please see [tests/test.py](https://github.com/bobalot/bitcoinsign/tree/master/tests/test.py) for examples of how to use the sign and verify module in python.

## Security

These tools may not protect in-memory data as well as bitcoin-qt does (from leaking after free, or being paged-out.) Secrets key are being handled so you should be only use this for signing messages on systems you believe to be secure.

Remember that arguments passed on a process' command line are visible to local users in most UNIX systems, so make sure you trust all local users enough, or adjust the programs to take arguments another way.

## Performance

It's faster than any implementation done in pure python, I'll create some performance tests eventually. But it can average around 1ms for a sign/verify function. Other implementations in pure-python have taken 600ms+ in my tests.