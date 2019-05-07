import Server
import logging

class GateProcessor:
	def __init__(self, port):
		# Список вида: user_id : gate_sock
		logging.info('GateProcessor started')
		self.user_gate = {}
		self.server = Server.Server(port)
		self.server.accepting(self.user_gate)

	def has_gate_for(self, user_id):
		logging.info('Calling to has_gate_for()')
		return 'all' in self.user_gate

	def send_tokens(self, user_id, tokens):
		if 'all' in self.user_gate:
			self.server.send(self.user_gate['all'], ' '.join(tokens))

	def recv_answer(self, user_id):
		if 'all' in self.user_gate:
			answer = self.server.recv(self.user_gate['all'])
			return answer
