# One-time Pad Encryption Server and Client Programs

Includes five small programs that encrypt and decrypt inormation using a one-tyime pad-like system. Please read [this](http://en.wikipedia.org/wiki/One-time_pad) article for more information on one-time pad encryption.

The program include an encryption and a decryption server, which the encryption client and decription clients can use to encrypt and decrypt plaintext messages.

## Getting Started


### Prerequisites

You will need to run this program using a UNIX machine with a gcc compiler.
The plaintext to be used as the messages can only contain uppercase letters.

### Set Up

First, you're required to compile the programs using the following command.

```
$ make
```

Generating the key.

```
$ keygen [key length] > mykey
```

## Running the Programs

### Encryption

Running the encryption server. (Can be ran in the background if using the same terminal as the client)

```
$ otp_enc_d [port1]
```

To run the above command in the background, use the following command instead.

```
$ otp_enc_d [port1] &
```

Running the encryption client. This sends a plaintext message to the server at a designated port. The server then encrypts the message and returns it to the client. In this case, the encrypted message is saved as cipher.txt.

```
$ otp_enc [plaintext file] mykey [port1] > cipher.txt
```

### Decryption

Running the decryption server. (Can also be run in the background)

```
$ otp_dec_d [port2]
```

Running the decryption client. This essentially does the same thing as the encryption flow, but instead takes the encrypted messages and outputs a decrypted message as plaintext.

```
$ otp_dec cipher.txt mykey [port2] > plain.txt
```

The original (decrypted) message can be found in plain.txt.

### Clean Up

If you ran the server(s) in the background, please run the following command to kill the process(es).

```
make kill
```

Please run the following command to clean up any generated files after completion.

```
make clean
```

