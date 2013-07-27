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
	
	import bitcoinsign
	# Don't import this private key into your wallet, thinking he-he someone might have 
	# left some coins in there that I can steal.
	# Many popular services such as blockchain.info will set the latest 
	# added address in your wallet as the change address for the next transaction.
	# So the next time you send some funds the change will end up in this address 
	# and someone else will almost certainly be watching it and take the coins before you do.
	priv_key = "5KBXt56X5DTzczziU7pqKB6g7iE7HcUCePeTEgF3N2mhhf7ENug"
	address = "1JTBWx4TQuPntUa4LDShg6xsbfEVFWbG9x"
	message = "Hello World!"

	signature = bitcoinsign.sign_message(priv_key, "Hello World!")
	verified = bitcoinsig.verify_message(address, signature, message)

	assert(verified is True)


## Security

These tools may not protect in-memory data as well as bitcoin-qt does (from leaking after free, or being paged-out.) Secrets key are being handled so you should be only use this for signing messages on systems you believe to be secure.

Remember that arguments passed on a process' command line are visible to local users in most UNIX systems, so make sure you trust all local users enough, or adjust the programs to take arguments another way.

## Performance

It's faster than any implementation done in pure python. Implementations in pure python have taken 600ms+ in my tests.

	%timeit -n 10 -r 10 sig = bitcoinsign.sign_message(priv_key, "Hello World!")
	10 loops, best of 10: 2.71 ms per loop

	%timeit -n 10 -r 10 bitcoinsign.verify_message(address, sig, "Hello World!")
	10 loops, best of 10: 669 us per loop
