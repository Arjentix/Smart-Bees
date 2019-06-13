import socket
import sys
import threading
import logging

class Server:
	def __init__(self, port):
		logging.info('Server started')
		self.sock = socket.socket();
		self.sock.bind(('', port))
		self.sock.listen(5)

	def __del__(self):
		if hasattr(self, 'sock'):
			self.sock.close()

	def accepting(self, gates):
		def accept():
			while (True):
				new_client, client_addr = self.sock.accept()
				logging.info('Connected with Gate: %s', client_addr[0])

				serial = self.recv(new_client)
				logging.info('Getted serial: %s', serial)

				if user != '':
					gates[serial] = new_client;

		logging.info('Server accepting started')
		threading.Thread(target=accept).start()

	def send(self, client_sock, message):
		client_sock.send(message.encode())

	def recv(self, client_sock):
		message = client_sock.recv(512)
		return message.decode()
