default: keygen otp_enc otp_enc_d otp_dec otp_dec_d

keygen: keygen.c
	gcc -o keygen keygen.c

otp_enc: otp_enc.c
	gcc -o otp_enc otp_enc.c

otp_enc_d: otp_enc_d.c
	gcc -o otp_enc_d otp_enc_d.c

otp_dec: otp_dec.c
	gcc -o otp_dec otp_dec.c

otp_dec_d: otp_dec_d.c
	gcc -o otp_dec_d otp_dec_d.c

kill:
	pkill otp_enc_d
	pkill otp_dec_d
	
clean:
	rm otp_dec otp_dec_d otp_enc otp_enc_d keygen
