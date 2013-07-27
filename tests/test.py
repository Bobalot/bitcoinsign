import bitcoinsign
import unittest
import random
import string

class BitcoinSignTest(unittest.TestCase):

	# Don't import this private key into your wallet, thinking he-he someone might have 
	# left some coins in there that I can steal.
	# Many popular services such as blockchain.info will set the latest 
	# added address in your wallet as the change address for the next transaction.
	# So the next time you send some funds the change will end up in this address 
	# and someone else will almost certainly be watching it and take the coins before you do.
	priv_key = "5KBXt56X5DTzczziU7pqKB6g7iE7HcUCePeTEgF3N2mhhf7ENug"
	address = "1JTBWx4TQuPntUa4LDShg6xsbfEVFWbG9x"

	def test_sign(self):
		self.assertTrue(bitcoinsign.sign_message(self.priv_key, "Hello World!"))

	def test_verify(self):
		self.assertTrue(bitcoinsign.verify_message(self.address, "HB3xxxBADKHQr4CgFS3wMYvYuJZDt+oNahe6cWUBU3QYhEGgn7b/iS9zo0WFrbsYtZyhAVmRYbuDfNhs7/mL5Ro=", "Hello World!"))

	def test_sign_and_verify(self):
		message = "Hello World!"

		signature = bitcoinsign.sign_message(self.priv_key, message)
		
		# Ensure the signature is not false
		self.assertTrue(signature)
		self.assertTrue(bitcoinsign.verify_message(self.address, signature, message))

	def test_sign_and_verify_random(self):
		for i in range(100):
			message = ''.join(random.choice(string.ascii_uppercase + string.digits) for x in range(20))
			signature = bitcoinsign.sign_message(self.priv_key, message)
			
			self.assertTrue(signature)
			self.assertTrue(bitcoinsign.verify_message(self.address, signature, message))


if __name__ == '__main__':
    unittest.main()
