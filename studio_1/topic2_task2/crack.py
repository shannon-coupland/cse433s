from Crypto import Random
from Crypto.Cipher import AES

plaintext = "255044462d312e350a25d0d4c5d80a34".decode("hex")
ciphertext = "d06bf9d0dab8e8ef880660d2af65aa82"
iv = "09080706050403020100A2B2C2D2E2F2".lower().decode("hex")
result = "0000"
f = open("keys.txt", "r")
key = ""
encrypted = ""

while (ciphertext is not result):
	#print("Ciphertext is " + ciphertext + ", result is " + result)
	key = f.readline()[:-1]
	print("key: " + key)
	key = key.decode("hex")
	print("key: " + key)
	if key == "":
		print("nope :(")
		break

	cipher = AES.new(key, AES.MODE_CBC, iv)
	encrypted = cipher.encrypt(plaintext)
	result = encrypted.encode('hex')
	#print("result: " + result)

print("key is " + key)

