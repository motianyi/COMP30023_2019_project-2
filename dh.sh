make
scp -i id_rsa dh.c tianyim@172.26.37.44:dh.c
openssl sha256 dh.c
./dh 172.26.37.44 7800