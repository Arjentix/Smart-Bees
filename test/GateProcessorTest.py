import time
import sys
sys.path.insert(0, '../src')

import GateProcessor

def main():
	gateproc = GateProcessor.GateProcessor(4551)
	user_id = '1451t45adgfga'

	time.sleep(5)
	if gateproc.has_gate_for(user_id):
		print('Sending tokens\n');

		tokens = ['Лампа', 'синий', 'цвет']
		gateproc.send_tokens(user_id, tokens)

		answer = gateproc.recv_answer(user_id)
		print('Answer:', answer, '\n')
	else:
		print('No Gate connected\n')

	return

if __name__ == '__main__':
	main()
