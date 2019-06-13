import Server
import DbHandler
import logging

class GateProcessor:
	def __init__(self, port):
		# Список вида: user_id : gate_sock
		logging.info('GateProcessor started')
		# gates -- contains serial of gate and socket
		self.gates = {}
		self.server = Server.Server(port)
		self.server.accepting(self.gates)

		self.db_handler = DbHandler.DbHandler()

	def get_gate_for(self, user_id):
		logging.info('Calling to get_gate_for()')
		return self.db_handler.get_gate_from_db(user_id)

	def is_gate_connected(self, gate_serial):
		return gate_serial in self.gates.keys()

	def send_tokens(self, user_id, tokens):
		self.server.send(self.gates['all'], ' '.join(tokens))

	def recv_answer(self, user_id):
		answer = self.server.recv(self.gates['all'])
		return answer
