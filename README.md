# httpsSSLv23-webserver
this is an webserver for https using SSLv23 using openssl1.1 libraries

## Getting started
### First step:
After running make, you are ready to go.
### Second Step in Terminal:
```bash
cd https-webserver/server/sslkeys
nano (or vim) genkeys.ssh # On company/CN=, modify it by using your public IP or you CN name```
chmod +x genkeys.sh
./genkeys.sh # this will generate public and private keys (READ troubleshooting 1)
cd ../ # server folder
sudo ./main
```
## Reqs
- openssl (libssl-dev) libssl1.1

## Troubleshooting
1. When running "genkeys.sh", you might face something like
```bash
Can't load <$HOME>/.rnd into RNG
```
To solve this: /etc/ssl/openssl.cnf and comment this line: RANDFILE = $ENV::HOME/.rnd.
src: [https://stackoverflow.com/questions/63893662/](https://stackoverflow.com/questions/63893662/cant-load-root-rnd-into-rng)

## Rev:
0.
	1. the header fields are comparing the header field length and the last and the last two letters, the initial idea was to use hash functions as follows:
	```C
	char hash(headerfield = "Host") {
		return (ASCII('H') + ASCII('o') + ASCII('s') + ASCII('t')) % HASH_ARRAY_LENGTH
	}
	```
	This created too many collitions, and hashing for a 10 to 11 headers fields that are send from FireFox or Chrome just didn't make sense.