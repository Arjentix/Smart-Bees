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

	def accepting(self, user_gate):
		def accept():
			while (True):
				new_client, client_addr = self.sock.accept()
				logging.info('Connected with Gate: %s', client_addr[0])

				'''
				Будующий код:
				user_id = get_user_from_db(client_addr)
				if user != '':
					user_gate[user_id] = new_client
				'''

				# MVP код:
				user_gate['all'] = new_client

		logging.info('Server accepting started')
		threading.Thread(target=accept).start()

	def send(self, client_sock, message):
		client_sock.send(message.encode())

	def recv(self, client_sock):
		message = client_sock.recv(512)
		return message.decode()
